#ifndef DYNAMIC_H
#define DYNAMIC_H

#include <QObject>
#include <QVariant>
#include "blockingqueue.h"
//#include <QtConcurrentRun>
#include "Sync.h"
#include <QMetaMethod>
#include "waveform.h"
#include <QQmlListProperty>

namespace QFlow{

class COREPLUGIN_EXPORT HasIndex
{
protected:
    int _index;
public:
    void setIndex(int index);
    int index();
};

class COREPLUGIN_EXPORT IDynamicSignal : public QObject, public HasIndex
{
    Q_OBJECT
public:
    QMetaMethod metaMethod();
};

class Dynamic;

class IDynamicSlot;

template<typename T>
class COREPLUGIN_EXPORT DynamicSignal : public IDynamicSignal
{
    friend class Dynamic;
protected:
    void send(T data);
    void operator()(T data);
    DynamicSignal(QString name);

public:
    virtual void out(T data) = 0;
};

template<typename T>
class DynamicSlot;

template<typename T>
class COREPLUGIN_EXPORT SlotArgs
{
private:
    IDynamicSlot* _slot;
    T _data;
public:
    SlotArgs(IDynamicSlot* slot, const T& data);
    IDynamicSlot* slot();
    T data();
};

class COREPLUGIN_EXPORT IDynamicSlot : public QObject, public HasIndex
{
    Q_OBJECT
protected:
    QObject* _owner;
    QMetaMethod _handler;
    QMetaMethod outSignal();
public:
    QMetaMethod metaMethod();
    void registerHandler(QObject* owner, QString handler);
    IDynamicSlot(QString name);
    IDynamicSlot(QObject* parent = NULL);
};

template<typename T>
class COREPLUGIN_EXPORT DynamicSlot : public IDynamicSlot
{
    typedef SlotArgs<T> ArgsType;
public:
    virtual void in(T data) = 0;
    virtual void out(T data) = 0;
    virtual void outArgs(ArgsType data) = 0;
    DynamicSlot<T>(QString name);
    DynamicSlot<T>(QObject* parent = NULL);
};

enum TopologyChange {SignalAdded, SignalRemoved, SlotAdded, SlotRemoved, DisplayTextChanged, PropertiesAdded};
class COREPLUGIN_EXPORT TopologyChangeArgs
{
protected:
    TopologyChange _change;
public:
    TopologyChangeArgs(TopologyChange change);
    TopologyChange getChange();
    QObject* param1;
    QVariant param2;
};

class COREPLUGIN_EXPORT TopologyUpdater : public QObject
{
    Q_OBJECT
public:
    void UpdateTopology(TopologyChangeArgs args);
signals:
    void TopologyUpdated(TopologyChangeArgs args);
};

class COREPLUGIN_EXPORT Dynamic
{
    friend class IDynamicSlot;
protected:
    QList<IDynamicSlot*> _slots;
    QList<IDynamicSignal*> _signals;
    TopologyUpdater _updater;
public:
    TopologyUpdater* updater();
    virtual void addSlot(IDynamicSlot* slot);
    IDynamicSlot* removeLastSlot();
    IDynamicSignal* removeLastSignal();
    void removeAllSignals();
    void removeAllSlots();

    virtual void addSignal(IDynamicSignal* signal);
    virtual void addSlot(IDynamicSlot *slot, QObject* owner, QString handler);

    virtual IDynamicSlot* slot(int i);
    virtual IDynamicSignal* signal(int i);
    template<typename T> void emitDynamicSignal(int i, T data);
    int slotCount();
    int signalCount();
    virtual bool slotExists(QString name);
    virtual bool signalExists(QString name);
};

class COREPLUGIN_EXPORT DynamicObject : public QObject, public Dynamic
{
    Q_OBJECT
    Q_PROPERTY(QQmlListProperty<QFlow::IDynamicSlot> inputs READ getInputs)
    Q_CLASSINFO("DefaultProperty", "inputs")
    static void appendInput(QQmlListProperty<IDynamicSlot> *property, IDynamicSlot *value);
    static int inputCount(QQmlListProperty<IDynamicSlot> *property);
    static IDynamicSlot* inputAt(QQmlListProperty<IDynamicSlot> *property, int index);
    static void clearInputs(QQmlListProperty<IDynamicSlot> *property);
public:
    Q_INVOKABLE DynamicObject(QObject* parent = NULL);
    QQmlListProperty<IDynamicSlot> getInputs();

    Q_INVOKABLE void addSlot(IDynamicSlot *slot, QObject* owner, QString handler);
    Q_INVOKABLE void addSlot(IDynamicSlot *slot, QString handler);
    Q_INVOKABLE void addSlot(IDynamicSlot* slot);
    Q_INVOKABLE void addSignal(IDynamicSignal* signal);
    Q_INVOKABLE bool signalExists(QString name);
    Q_INVOKABLE bool slotExists(QString name);
    Q_INVOKABLE QObject* slotObject(int i);
    Q_INVOKABLE QObject* signalObject(int i);
};
}
#endif // DYNAMIC_H
