#include "connection.h"
#include "acceptsconnection.h"
#include <QQmlEngine>
#include <QPointer>
#include <QDebug>


Q_GLOBAL_STATIC(QFlow::ConnectionList, connections)

namespace QFlow{

class ConnectionPrivate
{
public:
    QPointer<QObject> _source;
    QPointer<QObject> _destination;
    QPointer<Connection> _upstream;
    ConnectionList _downstream;
    bool _live;
    ConnectionPrivate() : _live(false)
    {

    }
    ~ConnectionPrivate();
};
ConnectionPrivate::~ConnectionPrivate()
{

}

Connection::Connection(QObject* parent) : QObject(parent), d_ptr(new ConnectionPrivate())
{
    connections()->append(this);
}
void Connection::clean()
{
    //setLive(false);
    Q_D(Connection);
    if(connections()->contains(this)) connections()->removeOne(this);
    if(d->_upstream)
    {
        d->_upstream->d_ptr->_downstream.removeOne(this);
    }
}

Connection::~Connection()
{
    clean();
}
ConnectionList Connection::siblings() const
{
    Q_D(const Connection);
    if(d->_upstream)
        return d->_upstream->d_ptr->_downstream;
    return QList<Connection*>();
}
void Connection::update()
{
    Q_D(Connection);
    if(d->_source && d->_destination)
    {
        AcceptsConnection* acd = dynamic_cast<AcceptsConnection*>(d->_destination.data());
        AcceptsConnection* acs = dynamic_cast<AcceptsConnection*>(d->_source.data());
        if(acd) acd->acceptConnection(this);
        if(acs) acs->acceptConnection(this);

        foreach(Connection* con, *connections)
        {
            if(con->destination() == d->_source)
            {
                d->_upstream = con;
                if(!con->d_ptr->_downstream.contains(this)) con->d_ptr->_downstream.append(this);
            }
            if(con->source() == d->_destination)
            {
                con->d_ptr->_upstream = this;
                if(!d->_downstream.contains(con)) d->_downstream.append(con);
            }
        }
        setLive(true);
        onReady();
    }
}
QObject* Connection::source() const
{
    Q_D(const Connection);
    return d->_source;
}
void Connection::setSource(QObject* source)
{
    Q_D(Connection);
    d->_source = source;
    update();
}
QObject* Connection::destination() const
{
    Q_D(const Connection);
    return d->_destination;
}
void Connection::setDestination(QObject* destination)
{
    Q_D(Connection);
    d->_destination = destination;
    update();
}
void Connection::setLive(bool value)
{
    Q_D(Connection);
    bool updateUpstream = true;
    if(!value && d->_upstream)
    {

        foreach (Connection* con, siblings()) {
            if(con != this && con->live()) updateUpstream = false;
        }
    }
    if(updateUpstream && d->_upstream)
    {
        d->_upstream->setLive(value);
    }

    onLiveChanging(value);
    d->_live = value;
    emit liveChanged(d->_live);
}
bool Connection::live() const
{
    Q_D(const Connection);
    return d->_live;
}
void Connection::onLiveChanging(bool /*value*/)
{

}
void Connection::onReady()
{

}

Connection *Connection::qmlAttachedProperties(QObject *obj)
{
    Connection* att = new Connection(obj);
    return att;
}
}
