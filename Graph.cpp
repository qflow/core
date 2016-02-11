/*
 * Graph.cpp
 *
 *  Created on: Feb 5, 2011
 *      Author: michal
 */

#include "Graph.h"
#include <qobjectdefs.h>
#include "initiable.h"
#include "blocking.h"
#include <QMdiSubWindow>
#include <QFormLayout>
#include <QSizeGrip>
#include <exception>

BlobConnection::BlobConnection(Signal signal, Slot slot) :
        _signal(signal), _slot(slot)
{

}
bool BlobConnection::operator ==(const BlobConnection& other)
{
    if(_signal == other._signal && this->_slot == other._slot) return true;
    return false;
}

Signal BlobConnection::signal()
{
    return _signal;
}
Slot BlobConnection::slot()
{
    return _slot;
}

QList<LibraryInfo*> Graph::libraries() const
{
    return _libraries;
}

ErrorInfo Graph::lastError()
{
    return _lastError;
}

Graph::Graph() : _mainWin(NULL),
    _lastError(ErrorInfo())
{

    LibraryLoader loader;
    _lastError = loader.loadLibraries(&_libraries);
    foreach(LibraryInfo* info, _libraries)
    {
        foreach(const QMetaObject* meta, *info->Modules)
        {
            this->classRegister_Module.insert(meta->className(), ClassInfo(*meta));
        }
    }

}
void Graph::setMainWin(QWidget *mainWin)
{
    _mainWin = mainWin;
}

void Graph::clear()
{
    foreach(QObject* obj, _modules)
    {
        delete obj;
    }
    _modules.clear();
    _connections.clear();
}

Graph::~Graph() {
    clear();
    foreach(LibraryInfo* info, _libraries)
    {
        delete info->Plugin;
    }
}

int Graph::objectCount(QString className)
{
    return classRegister_Module[className].count();
}

void Graph::addObject(QObject* obj)
{
    _modules.push_back(obj);
    classRegister_Module[obj->metaObject()->className()].incrementCount();
}

QObject* Graph::createObject(QString className)
{
    if(classRegister_Module.contains(className))
    {
        QMetaObject meta = classRegister_Module[className].meta();
        QObject* obj = meta.newInstance();
        return obj;
    }
    else return NULL;
}
void Graph::removeObject(QObject* obj)
{
    _modules.removeAt(_modules.indexOf(obj));
    _connections.remove(obj);
    classRegister_Module[obj->metaObject()->className()].decrementCount();
}
bool Graph::connect(Signal signal, Slot slot)
{
    QString signature = QString(signal.metaMethod().methodSignature());
    QString signalT = QString("2%1").arg(signature);
    signature = QString(slot.metaMethod().methodSignature());
    QString slotT = QString("1%1").arg(signature);
    bool res = QObject::connect(signal.methodOwner(), signal.metaMethod(),
                                slot.methodOwner(), slot.metaMethod(), Qt::DirectConnection);
    if(!res) return false;
    BlobConnection con(signal, slot);
    _connections.insert(signal.owner(), con);
    return true;
}
bool Graph::isConnected(Slot slot)
{
    foreach(BlobConnection con, _connections.values())
    {
        if(con.slot().name() == slot.name()) return true;
    }
    return false;
}

bool Graph::disconnect(Signal signal, Slot slot)
{
    //QString signalT = QString("2%1").arg(signal.metaMethod().signature());
    //QString slotT = QString("1%1").arg(slot.metaMethod().signature());
    bool res = QObject::disconnect(signal.methodOwner(), signal.metaMethod(),
                                   slot.methodOwner(), slot.metaMethod());
    _connections.remove(signal.owner(), BlobConnection(signal, slot));
    return res;
}

QMultiHash<QObject*, BlobConnection> Graph::connections() const
{
    return _connections;
}
ModuleList Graph::modules() const
{
    return _modules;
}

void Graph::closeWidgets()
{
    foreach(QObject* obj, _modules)
    {
        QWidget* widget = qobject_cast<QWidget*>(obj);
        if(widget)
        {
            widget->close();
        }
    }
}
void Graph::startGraph()
{
    _lastError = ErrorInfo();
    QList<Initiable*> initialized;
    foreach(QObject* obj, _modules)
    {
        Initiable* ini = dynamic_cast<Initiable*>(obj);
        if(ini)
        {
            ErrorInfo err = ini->init();
            if(err.errorCode() != 0)
            {
                _lastError = err;
                break;
            }
            else
            {
                initialized.push_back(ini);
            }
        }
    }
    if(_lastError.errorCode() != 0)
    {
        foreach(Initiable* ini, initialized)
        {
            ini->deinit();
        }
        return;
    }

    if(_mainWin)
    {
        foreach(QObject* obj, _modules)
        {
            QWidget* widget = qobject_cast<QWidget*>(obj);
            if(widget)
            {
                widget->setParent(_mainWin);
                widget->show();
            }
        }
    }

    foreach(QObject* obj, _modules)
    {
        Startable* startable = dynamic_cast<Startable*>(obj);
        if(startable)
        {
            WaitHandle handle = startable->startModule();
            runningModules.insert(startable,handle);
        }
    }
    start();
}

void Graph::run()
{
    QMutexLocker locker(&runFinished);
    foreach(WaitHandle handle, runningModules.values())
    {
        handle.wait();
    }
    runningModules.clear();
    foreach(QObject* obj, _modules)
    {
        Initiable* ini = dynamic_cast<Initiable*>(obj);
        if(ini)
        {
            ErrorInfo err = ini->deinit();
            _lastError = err;
            if(_lastError) return;
        }
    }
    emit finished();
}

void Graph::stop()
{
    foreach(QObject* obj, _modules)
    {
        Blocking* blo = dynamic_cast<Blocking*>(obj);
        if(blo)
        {
            blo->unblock();
        }
    }
    foreach(Startable* startable, runningModules.keys())
    {
        startable->stopModule();
    }
    runFinished.lock();
}
