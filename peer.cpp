#include "peer.h"

Peer::Peer(bool isDynamic, QObject* owner, QMetaMethod method, QString name, QObject* methodOwner, int index) :
    _isDynamic(isDynamic), _owner(owner), _method(method), _name(name), _methodOwner(methodOwner), _index(index)
{

}
Peer::Peer()
{

}
bool Peer::operator ==(const Peer &other)
{
    if(_name == other._name && _owner == other._owner) return true;
    return false;
}

bool Peer::isDynamic()
{
    return _isDynamic;
}

QObject* Peer::owner()
{
    return _owner;
}

QString Peer::name()
{
    if(_isDynamic && _methodOwner) return _methodOwner->objectName();
    return _name;
}
QMetaMethod Peer::metaMethod()
{
    return _method;
}
QObject* Peer::methodOwner()
{
    if(!_isDynamic) return _owner;
    return _methodOwner;
}
int Peer::index()
{
    return _index;
}

Slot::Slot()
{

}

Slot::Slot(bool isDynamic, QObject* owner, QMetaMethod method, QString name, QObject* methodOwner, int index) :
    Peer(isDynamic, owner, method, name, methodOwner, index)
{
}
Signal::Signal()
{

}

Signal::Signal(bool isDynamic, QObject* owner, QMetaMethod method, QString name, QObject* methodOwner, int index) :
    Peer(isDynamic, owner, method, name, methodOwner, index)
{

}
