#include "Dynamic.h"

namespace QFlow{

void HasIndex::setIndex(int index)
{
    _index = index;
}
int HasIndex::index()
{
    return _index;
}
QMetaMethod IDynamicSignal::metaMethod()
{
    for(int i=0;i<this->metaObject()->methodCount();i++)
    {
        QMetaMethod method = this->metaObject()->method(i);
        QString sig = QString(method.methodSignature());
        if(sig.startsWith("out(")) return method;
    }
    return QMetaMethod();
}
template<typename T>
void DynamicSignal<T>::send(T data)
{
    emit out(data);
}
template<typename T>
void DynamicSignal<T>::operator()(T data)
{
    send(data);
}
template<typename T>
DynamicSignal<T>::DynamicSignal(QString name)
{
    setObjectName(name);
}
template<typename T>
SlotArgs<T>::SlotArgs(IDynamicSlot* slot, const T& data) : _slot(slot), _data(data)
{

}
template<typename T>
IDynamicSlot* SlotArgs<T>::slot()
{
    return _slot;
}
template<typename T>
T SlotArgs<T>::data()
{
    return _data;
}
QMetaMethod IDynamicSlot::outSignal()
{
    QMetaMethod method;
    for(int i=0;i<this->metaObject()->methodCount();i++)
    {
        method = this->metaObject()->method(i);
        QString sig = QString(method.methodSignature());
        if(sig.startsWith("outArgs("))
        {
            break;
        }
    }
    return method;
}
QMetaMethod IDynamicSlot::metaMethod()
{
    QMetaMethod method;
    for(int i=0;i<this->metaObject()->methodCount();i++)
    {
        method = this->metaObject()->method(i);
        QString sig = QString(method.methodSignature());
        if(sig.startsWith("in(")) break;
    }
    return method;
}
void IDynamicSlot::registerHandler(QObject* owner, QString handler)
{
    _owner = owner;
    const QMetaObject* meta = _owner->metaObject();
    for(int i=0;i<meta->methodCount();i++)
    {
        _handler = meta->method(i);
        QString sig = QString(_handler.methodSignature());
        if(sig.startsWith(handler))
        {
            break;
        }
    }
    QMetaMethod signal = outSignal();
    //QString signalT = QString("2%1").arg(signal.signature());
    //QString slotT = QString("1%1").arg(_handler.signature());
    QObject::connect(this, signal, _owner, _handler, Qt::DirectConnection);
}
IDynamicSlot::IDynamicSlot(QString name)
{
    _owner = NULL;
    setObjectName(name);
}
IDynamicSlot::IDynamicSlot(QObject* parent) : QObject(parent)
{
    _owner = NULL;
}
template<typename T>
DynamicSlot<T>::DynamicSlot(QString name) : IDynamicSlot(name)
{

}
template<typename T>
DynamicSlot<T>::DynamicSlot(QObject* parent) : IDynamicSlot(parent)
{

}
TopologyChangeArgs::TopologyChangeArgs(TopologyChange change) : _change(change)
{

}
TopologyChange TopologyChangeArgs::getChange()
{
    return _change;
}
void TopologyUpdater::UpdateTopology(TopologyChangeArgs args)
{
    emit TopologyUpdated(args);
}

TopologyUpdater* Dynamic::updater()
{
    return &_updater;
}

void Dynamic::addSlot(IDynamicSlot* slot)
{
    _slots.push_back(slot);
    int index = _slots.indexOf(slot);
    slot->setIndex(index);
    TopologyChangeArgs args(SlotAdded);
    args.param1 = slot;
    _updater.UpdateTopology(args);
}
IDynamicSlot* Dynamic::removeLastSlot()
{
    if(!_slots.isEmpty())
    {
        IDynamicSlot* last = _slots.last();
        _slots.removeLast();
        TopologyChangeArgs args(SlotRemoved);
        args.param1 = last;
        _updater.UpdateTopology(args);
        return last;
    }
    else return NULL;
}
IDynamicSignal* Dynamic::removeLastSignal()
{
    if(!_signals.isEmpty())
    {
        IDynamicSignal* last = _signals.last();
        _signals.removeLast();
        TopologyChangeArgs args(SignalRemoved);
        args.param1 = last;
        _updater.UpdateTopology(args);
        return last;
    }
    else return NULL;
}
void Dynamic::removeAllSignals()
{
    while(removeLastSignal());
}
void Dynamic::removeAllSlots()
{
    while(removeLastSlot());
}

void Dynamic::addSignal(IDynamicSignal* signal)
{
    _signals.push_back(signal);
    int index = _signals.indexOf(signal);
    signal->setIndex(index);
    TopologyChangeArgs args(SignalAdded);
    args.param1 = signal;
    _updater.UpdateTopology(args);
}
void Dynamic::addSlot(IDynamicSlot *slot, QObject* owner, QString handler)
{
    addSlot(slot);
    slot->registerHandler(owner, handler);
}

IDynamicSlot* Dynamic::slot(int i)
{
    if(_slots.length() > i)
        return _slots[i];
    return NULL;
}
IDynamicSignal* Dynamic::signal(int i)
{
    if(_signals.length() > i)
    {
        return _signals[i];
    }
    return NULL;
}
template<typename T> void Dynamic::emitDynamicSignal(int i, T data)
{
    DynamicSignal<T>* signal = (DynamicSignal<T>*)_signals[i];
    signal->send(data);
}
int Dynamic::slotCount()
{
    return _slots.count();
}
int Dynamic::signalCount()
{
    return _signals.count();
}
bool Dynamic::slotExists(QString name)
{
    foreach(IDynamicSlot* slot, _slots)
    {
        if(slot->objectName() == name) return true;
    }
    return false;
}
bool Dynamic::signalExists(QString name)
{
    foreach(IDynamicSignal* signal, _signals)
    {
        if(signal->objectName() == name) return true;
    }
    return false;
}
void DynamicObject::appendInput(QQmlListProperty<IDynamicSlot> *property, IDynamicSlot *value)
{
    DynamicObject *obj = qobject_cast<DynamicObject*>(property->object);
    if(obj)
    {
        obj->_slots.append(value);
        value->setIndex(obj->_slots.count() - 1);
        value->setParent(obj);
        value->registerHandler(obj, "received");
    }

}
int DynamicObject::inputCount(QQmlListProperty<IDynamicSlot> *property)
{
    DynamicObject *obj = qobject_cast<DynamicObject*>(property->object);
    if(obj)
    {
        return obj->_slots.count();
    }
    return 0;
}
IDynamicSlot* DynamicObject::inputAt(QQmlListProperty<IDynamicSlot> *property, int index)
{
    DynamicObject *obj = qobject_cast<DynamicObject*>(property->object);
    if(obj)
    {
        return obj->_slots[index];
    }
    return NULL;
}
void DynamicObject::clearInputs(QQmlListProperty<IDynamicSlot> *property)
{
    DynamicObject *obj = qobject_cast<DynamicObject*>(property->object);
    if(obj)
    {
        obj->_slots.clear();
    }
}
DynamicObject::DynamicObject(QObject* parent) : QObject(parent)
{

}


QQmlListProperty<IDynamicSlot> DynamicObject::getInputs()
{
    return QQmlListProperty<IDynamicSlot>(this, NULL, &DynamicObject::appendInput,
                                          &DynamicObject::inputCount,
                                          &DynamicObject::inputAt,
                                          &DynamicObject::clearInputs);
}

void DynamicObject::addSlot(IDynamicSlot *slot, QObject* owner, QString handler)
{
    Dynamic::addSlot(slot, owner, handler);
}
void DynamicObject::addSlot(IDynamicSlot *slot, QString handler)
{
    addSlot(slot, this, handler);
}
void DynamicObject::addSlot(IDynamicSlot* slot)
{
    Dynamic::addSlot(slot);
}
void DynamicObject::addSignal(IDynamicSignal* signal)
{
    Dynamic::addSignal(signal);
}
bool DynamicObject::signalExists(QString name)
{
    return Dynamic::signalExists(name);
}
bool DynamicObject::slotExists(QString name)
{
    return Dynamic::slotExists(name);
}
QObject* DynamicObject::slotObject(int i)
{
    return Dynamic::slot(i);
}
QObject* DynamicObject::signalObject(int i)
{
    return Dynamic::signal(i);
}

typedef Timestamped<double> TimestampedDouble;
template class DynamicSlot<double>;
template class DynamicSlot<quint64>;
template class SlotArgs<quint64>;
template class SlotArgs<double>;
template class DynamicSlot<Waveform>;
template class SlotArgs<TimestampedDouble>;
template class SlotArgs<Waveform>;
template class DynamicSlot<TimestampedDouble>;
}
