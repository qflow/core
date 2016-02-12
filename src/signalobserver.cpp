#include "signalobserver.h"
#include <QMetaMethod>
#include <QVector>
#include <QDebug>

namespace QFlow{

class SignalObserverPrivate : public QObject
{
public:
    SignalObserverPrivate(SignalObserver* parent, QObject *obj, QString signal, bool enabled) : QObject(),
        _sig(signal), _observedObject(obj), _enabled(false), q_ptr(parent)
    {
        if (!obj) {
            qWarning("SignalObserver: Cannot spy on a null object");
            return;
        }
        const QMetaObject * const mo = obj->metaObject();
        const int sigIndex = mo->indexOfMethod(signal.toLatin1());
        if (sigIndex < 0) {
            qWarning() << "SignalObserver: No such signal: " + signal;
            return;
        }
        setEnabled(enabled);
        initArgs(mo->method(sigIndex), obj);
    }

    ~SignalObserverPrivate()
    {

    }

    QString _sig;
    QVector<int> _args;
    QObject* _observedObject;
    bool _enabled;
    void initArgs(const QMetaMethod &member, QObject *obj)
    {
        _args.reserve(member.parameterCount());
        for (int i = 0; i < member.parameterCount(); ++i) {
            int tp = member.parameterType(i);
            if (tp == QMetaType::UnknownType && obj) {
                void *argv[] = { &tp, &i };
                QMetaObject::metacall(const_cast<QObject*>(obj),
                                      QMetaObject::RegisterMethodArgumentMetaType,
                                      member.methodIndex(), argv);
                if (tp == -1)
                    tp = QMetaType::UnknownType;
            }
            if (tp == QMetaType::UnknownType) {
                qWarning("SignalObserver: Unable to handle parameter '%s' of type '%s' of method '%s',"
                         " use qRegisterMetaType to register it.",
                         member.parameterNames().at(i).constData(),
                         member.parameterTypes().at(i).constData(),
                         member.name().constData());
            }
            _args << tp;
        }
    }
    QList<QVariant> appendArgs(void **a)
    {
        QList<QVariant> list;
        list.reserve(_args.count());
        for (int i = 0; i < _args.count(); ++i) {
            const QMetaType::Type type = static_cast<QMetaType::Type>(_args.at(i));
            if (type == QMetaType::QVariant)
                list << *reinterpret_cast<QVariant *>(a[i + 1]);
            else
                list << QVariant(type, a[i + 1]);
        }
        return list;
    }
    void setEnabled(bool value)
    {
        if(value == _enabled) return;
        static const int memberOffset = QObject::staticMetaObject.methodCount();
        const int sigIndex = _observedObject->metaObject()->indexOfMethod(_sig.toLatin1());
        if(value)
        {
            if (!QMetaObject::connect(_observedObject, sigIndex, this, memberOffset,
                                      Qt::DirectConnection, Q_NULLPTR)) {
                qWarning("SignalObserver: QMetaObject::connect returned false. Unable to connect.");
                return;
            }
        }
        else
        {
            QMetaObject::disconnect(_observedObject, sigIndex, this, memberOffset);
        }
        _enabled = value;
    }

    int qt_metacall(QMetaObject::Call call, int methodId, void **a)
    {
        methodId = QObject::qt_metacall(call, methodId, a);
        if (methodId < 0)
            return methodId;

        if (call == QMetaObject::InvokeMetaMethod) {
            if (methodId == 0) {
                QList<QVariant> currentArgs = appendArgs(a);
                emit q_ptr->signalEmitted(currentArgs);
            }
            --methodId;
        }
        return methodId;
    }
private:
    SignalObserver* q_ptr;
    Q_DECLARE_PUBLIC(SignalObserver)
};

SignalObserver::SignalObserver(QObject *obj, QString signal, bool enabled) : QObject(obj), d_ptr(new SignalObserverPrivate(this, obj, signal, enabled))
{

}
SignalObserver::~SignalObserver()
{

}
QString SignalObserver::signal() const
{
    Q_D(const SignalObserver);
    return d->_sig;
}
void SignalObserver::setEnabled(bool value)
{
    Q_D(SignalObserver);
    d->setEnabled(value);
}
bool SignalObserver::enabled() const
{
    Q_D(const SignalObserver);
    return d->_enabled;
}
}
