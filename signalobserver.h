#ifndef SIGNALOBSERVER_H
#define SIGNALOBSERVER_H

#include "coreplugin_global.h"
#include <QObject>
#include <QVariant>

namespace QFlow{

class SignalObserverPrivate;
class COREPLUGIN_EXPORT SignalObserver : public QObject
{
    Q_OBJECT
public:
    explicit SignalObserver(QObject *obj, QString signal, bool enabled = true);
    ~SignalObserver();
    QString signal() const;
    void setEnabled(bool value);
    bool enabled() const;
Q_SIGNALS:
    void signalEmitted(QVariantList args);
private:
    const QScopedPointer<SignalObserverPrivate> d_ptr;
    Q_DECLARE_PRIVATE(SignalObserver)
};
}
#endif // SIGNALOBSERVER_H
