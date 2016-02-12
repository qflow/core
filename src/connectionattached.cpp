#include "connectionattached.h"

namespace QFlow{

ConnectionAttached::ConnectionAttached(QObject *parent) : QObject(parent)
{

}
ConnectionAttached::~ConnectionAttached()
{

}

QObject* ConnectionAttached::dest() const
{
    return _dest;
}
void ConnectionAttached::setDest(QObject* value)
{
    _dest = value;
    emit destChanged();
}
}
