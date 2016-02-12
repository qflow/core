#ifndef CONNECTIONATTACHED_P_H
#define CONNECTIONATTACHED_P_H

#include <QObject>
#include <QPointer>
#include "coreplugin_global.h"

namespace QFlow{

class COREPLUGIN_EXPORT ConnectionAttached : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QObject* dest READ dest WRITE setDest NOTIFY destChanged FINAL)
public:
    explicit ConnectionAttached(QObject *parent = 0);
    virtual ~ConnectionAttached();
    QObject* dest() const;
    void setDest(QObject* value);
signals:
    void destChanged();
private:
    QPointer<QObject> _dest;
};
}

#endif // CONNECTIONATTACHED_P_H
