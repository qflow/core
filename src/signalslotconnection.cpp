#include "signalslotconnection.h"
#include <QMetaMethod>
#include <QJSValueIterator>

#include <qqmlcontext.h>
#include <qqmlinfo.h>
#include "acceptsconnection.h"

namespace QFlow{

class SignalSlotConnectionPrivate
{
public:
    QString _signal, _slot;
    QMetaObject::Connection _internalConnection;
    SignalSlotConnection::ConnectionType _connectionType;
    SignalSlotConnectionPrivate() : _connectionType(SignalSlotConnection::Auto)
    {

    }
    ~SignalSlotConnectionPrivate();
};
SignalSlotConnectionPrivate::~SignalSlotConnectionPrivate()
{
}

QObject* SignalSlotConnection::findById(QQmlContext *context, QObject *obj, QString id)
{
    if(id == context->nameForObject(obj)) return obj;
    for(QObject* child: obj->children())
    {
        if(id == context->nameForObject(child)) return child;
    }
    if(obj->parent()) return findById(context, obj->parent(), id);
    return NULL;
}


SignalSlotConnection::SignalSlotConnection(QObject *parent) :
    Connection(parent), d_ptr(new SignalSlotConnectionPrivate())
{
}
SignalSlotConnection::~SignalSlotConnection()
{
    clean();
}

QString SignalSlotConnection::getSignal()
{
    Q_D(SignalSlotConnection);
    return d->_signal;
}
void SignalSlotConnection::setSignal(QString signal)
{
    Q_D(SignalSlotConnection);
    d->_signal = signal;
    setLive(true);
}
QString SignalSlotConnection::getSlot()
{
    Q_D(SignalSlotConnection);
    return d->_slot;
}

void SignalSlotConnection::setSlot(QString slot)
{
    Q_D(SignalSlotConnection);
    d->_slot = slot;
    setLive(true);
}

QMetaMethod SignalSlotConnection::findMethod(QObject *owner, QString signature)
{
    const QMetaObject* metaObject = owner->metaObject();
    int index = metaObject->indexOfMethod(signature.toUtf8().constData());
    return metaObject->method(index);
}

void SignalSlotConnection::tryConnect()
{
    Q_D(SignalSlotConnection);
    if(source() && destination() && !d->_signal.isNull() && !d->_slot.isNull())
    {
        QMetaMethod slot = findMethod(destination(), d->_slot);
        QMetaMethod signal = findMethod(source(), d->_signal);
        d->_internalConnection = QObject::connect(source(), signal,
                                                       destination(), slot,
                         (Qt::ConnectionType)d->_connectionType);
        bool success = d->_internalConnection;
        if(!success)
        {
            qDebug() << QString("DirectConnection: could not connect %1 with %2").arg(d->_signal).arg(d->_slot);
        }
    }
}
void SignalSlotConnection::tryDisconnect()
{
    Q_D(SignalSlotConnection);
    if(source() && destination() && !d->_signal.isNull() && !d->_slot.isNull())
    {
        QMetaMethod slot = findMethod(destination(), d->_slot);
        QMetaMethod signal = findMethod(source(), d->_signal);
        bool success = QObject::disconnect(source(), signal,
                                           destination(), slot);
        if(success)
        {
            d->_internalConnection = QMetaObject::Connection();
            qDebug() << QString("Disconnected: %1 -> %2").arg(source()->metaObject()->className()).
                        arg(destination()->metaObject()->className());
        }
        if(!success)
        {
            qDebug() << QString("DirectConnection: could not disconnect %1 with %2").arg(d->_signal).arg(d->_slot);
        }
    }
}
void SignalSlotConnection::onLiveChanging(bool value)
{
    Q_D(SignalSlotConnection);
    if(value && !d->_internalConnection)
    {
        tryConnect();
    }
    if(!value && d->_internalConnection)
    {
        tryDisconnect();
    }
}
SignalSlotConnection::ConnectionType SignalSlotConnection::connectionType() const
{
    Q_D(const SignalSlotConnection);
    return d->_connectionType;
}
void SignalSlotConnection::setConnectionType(ConnectionType value)
{
    Q_D(SignalSlotConnection);
    d->_connectionType = value;
    if(d->_internalConnection)
    {
        tryDisconnect();
        tryConnect();
    }
}
}
