#ifndef SMARTCONNECTION_H
#define SMARTCONNECTION_H

#include <QObject>

#include <QObject>
#include "signalslotconnection.h"

namespace QFlow{

class SmartConnection : public SignalSlotConnection
{
    Q_OBJECT
public:
    explicit SmartConnection(QObject *parent = 0);
    ~SmartConnection();
    static SmartConnection *qmlAttachedProperties(QObject *obj);
    virtual void onReady();
Q_SIGNALS:

public Q_SLOTS:

};
}
QML_DECLARE_TYPEINFO(QFlow::SmartConnection, QML_HAS_ATTACHED_PROPERTIES)
#endif // SMARTCONNECTION_H
