/*
 * Graph.h
 *
 *  Created on: Feb 5, 2011
 *      Author: michal
 */

#ifndef GRAPH_H_
#define GRAPH_H_

#include <QObject>
#include <QHash>
#include "LibraryLoader.h"
#include <QDomDocument>
#include <QUuid>
#include "peer.h"
#include "startable.h"
#include <QWidget>
#include <QMdiArea>
#include <QThread>
#include <QMutex>

class BlobConnection
{
private:
        Signal _signal;
        Slot _slot;
	QString _toSlot;
public:
        BlobConnection(Signal signal, Slot slot);
        Signal signal();
        Slot slot();
        bool operator==(const BlobConnection &other);
};

typedef QList<BlobConnection> ConnectionList;
typedef QList<QObject*> ModuleList;

class ClassInfo
{
    QMetaObject _metaObject;
    int _objectCount;
public:
    ClassInfo(QMetaObject meta) : _metaObject(meta), _objectCount(0)
    {

    }
    ClassInfo() : _objectCount(0)
    {

    }

    void incrementCount()
    {
        _objectCount++;
    }
    void decrementCount()
    {
        _objectCount--;
    }
    ClassInfo(const ClassInfo &b)
    {
        _metaObject = b._metaObject;
        _objectCount = b._objectCount;
    }
    QMetaObject meta()
    {
        return _metaObject;
    }
    int count()
    {
        return _objectCount;
    }
};

class Graph : public QThread {
    Q_OBJECT
private:
    QHash<QString, ClassInfo> classRegister_Module;
    QHash<QString, ClassInfo> classRegister_Slot;
    QHash<QString, ClassInfo> classRegister_Signal;
    QList<LibraryInfo*> _libraries;
    QMultiHash<QObject*, BlobConnection> _connections;
    QList<QObject*> _modules;
    QMap<Startable*,WaitHandle> runningModules;
    QWidget* _mainWin;
    ErrorInfo _lastError;
    QMutex runFinished;
    void run();
public:
    Graph();
    virtual ~Graph();
    void clear();
    QObject* createObject(QString className);
    bool connect(Signal signal, Slot slot);
    void removeObject(QObject* obj);
    QList<LibraryInfo*> libraries() const;
    QMultiHash<QObject*, BlobConnection> connections() const;
    ModuleList modules() const;
    void addObject(QObject* obj);
    bool disconnect(Signal signal, Slot slot);
    void startGraph();
    void stop();
    void closeWidgets();
    void setMainWin(QWidget* mainWin);
    ErrorInfo lastError();
    int objectCount(QString className);
    bool isConnected(Slot slot);
signals:
    void finished();
};

#endif /* GRAPH_H_ */
