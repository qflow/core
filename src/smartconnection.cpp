#include "smartconnection.h"
#include <QDebug>

namespace QFlow{

SmartConnection::SmartConnection(QObject *parent) :
    SignalSlotConnection(parent)
{
    setConnectionType(SignalSlotConnection::Direct);
}
SmartConnection::~SmartConnection()
{
    clean();
}

void SmartConnection::onReady()
{
    bool success = false;
    if(!source())
    {
        qWarning() << "MediaConnection: source is null";
    }
    if(!destination())
    {
        qWarning() << "MediaConnection: destination is null";
    }

    const QMetaObject* sourceMeta = source()->metaObject();
    const QMetaObject* destMeta = destination()->metaObject();
    for(int i=0; i<sourceMeta->methodCount(); i++)
    {
        QMetaMethod sourceMethod = sourceMeta->method(i);
        QString sourceName = sourceMethod.name();
        if(sourceMethod.methodType() == QMetaMethod::Signal &&
                sourceName != "destroyed" &&
                sourceName != "objectNameChanged")
        {
            for(int j=0; j<destMeta->methodCount(); j++)
            {
                QMetaMethod destMethod = destMeta->method(j);
                QString destName = destMethod.name();
                if(destMethod.methodType() == QMetaMethod::Slot &&
                        destName != "deleteLater" &&
                        destName != "_q_reregisterTimers" &&
                        destMethod.parameterCount() == sourceMethod.parameterCount())
                {
                    bool isCompatible = QMetaObject::checkConnectArgs(sourceMethod, destMethod);
                    if(isCompatible)
                    {
                        success = true;
                        setSignal(sourceMethod.methodSignature());
                        setSlot(destMethod.methodSignature());
                        qDebug() << QString("SmartConnection connected %1 and %2").arg((QString)sourceMethod.methodSignature())
                                    .arg((QString)destMethod.methodSignature());
                        break;
                    }
                }
            }
        }
        if(success) break;
    }
    if(!success)
    {
        qWarning() << QString("SmartConnection: incompatible objects: %1 %2").arg(source()->metaObject()->className())
                    .arg(destination()->metaObject()->className());
    }

}

SmartConnection* SmartConnection::qmlAttachedProperties(QObject *obj)
{
    SmartConnection* con = new SmartConnection(obj);
    con->setSource(obj);
    return con;
}
}
