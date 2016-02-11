#ifndef CONNECTION_H
#define CONNECTION_H

#include "coreplugin_global.h"
#include <QObject>
#include <QQmlParserStatus>
#include <qqml.h>
#include "connectionattached.h"

namespace QFlow{

class Connection;
typedef QList<Connection*> ConnectionList;

class ConnectionPrivate;
class COREPLUGIN_EXPORT Connection : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QObject* source READ source WRITE setSource)
    Q_PROPERTY(QObject* destination READ destination WRITE setDestination)
    Q_PROPERTY(bool live READ live WRITE setLive NOTIFY liveChanged)

    void update();
protected:
    virtual void onLiveChanging(bool value);
    virtual void onReady();
    void clean();
public:
    explicit Connection(QObject* parent = NULL);
    virtual ~Connection();
    QObject* source() const;
    void setSource(QObject* source);
    QObject* destination() const;
    void setDestination(QObject* destination);
    bool live() const;
    void setLive(bool value);
    ConnectionList siblings() const;
    static Connection *qmlAttachedProperties(QObject *obj);
Q_SIGNALS:
    void liveChanged(bool value);
private:
    const QScopedPointer<ConnectionPrivate> d_ptr;
    Q_DECLARE_PRIVATE(Connection)
};
}
QML_DECLARE_TYPEINFO(QFlow::Connection, QML_HAS_ATTACHED_PROPERTIES)

#endif // CONNECTION_H
