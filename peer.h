#ifndef PEER_H
#define PEER_H

#include <QMetaMethod>

class Peer
{
public:
    Peer(bool isDynamic, QObject* owner, QMetaMethod method, QString name, QObject* methodOwner = NULL, int index=-1);
    Peer();
    QString name();
    QMetaMethod metaMethod();
    QObject* methodOwner();
    QObject* owner();
    bool isDynamic();
    int index();
    bool operator==(const Peer &other);
protected:
    bool _isDynamic;
    QObject* _owner;
    QMetaMethod _method;
    QString _name;
    QObject* _methodOwner;
    int _index;
};

class Slot : public Peer
{
public:
    Slot();
    Slot(bool isDynamic, QObject* owner, QMetaMethod method, QString name, QObject* methodOwner = NULL, int index=-1);
};

class Signal : public Peer
{
public:
    Signal();
    Signal(bool isDynamic, QObject* owner, QMetaMethod method, QString name, QObject* methodOwner = NULL, int index=-1);
};

#endif // PEER_H
