#ifndef DIRECTCONNECTION_H
#define DIRECTCONNECTION_H

#include "connection.h"
#include <QObject>
#include <QJSValue>
#include <qqml.h>

namespace QFlow{

class SignalSlotConnectionPrivate;
class COREPLUGIN_EXPORT SignalSlotConnection : public Connection
{
    Q_OBJECT
    Q_ENUMS(ConnectionType)
    Q_PROPERTY(QString signal READ getSignal WRITE setSignal)
    Q_PROPERTY(QString slot READ getSlot WRITE setSlot)
    Q_PROPERTY(ConnectionType connectionType READ connectionType WRITE setConnectionType)

    void tryConnect();
    void tryDisconnect();
    QMetaMethod findMethod(QObject* owner, QString signature);
    QObject* findById(QQmlContext* context, QObject* obj, QString id);
protected:
    void onLiveChanging(bool value);
public:
    enum ConnectionType {Auto = Qt::AutoConnection, Queued = Qt::QueuedConnection, Direct = Qt::DirectConnection, BlockingQueued = Qt::BlockingQueuedConnection};
    explicit SignalSlotConnection(QObject *parent = 0);
    virtual ~SignalSlotConnection();
    QString getSignal();
    void setSignal(QString signal);
    QString getSlot();
    void setSlot(QString slot);
    ConnectionType connectionType() const;
    void setConnectionType(ConnectionType value);
private:
    const QScopedPointer<SignalSlotConnectionPrivate> d_ptr;
    Q_DECLARE_PRIVATE(SignalSlotConnection)
};
}
#endif // DIRECTCONNECTION_H

