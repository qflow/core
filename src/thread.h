#ifndef THREAD_H
#define THREAD_H

#include <QThread>
#include <QQmlListProperty>
#include <QQmlParserStatus>
#include <QTimer>

namespace QFlow{

class ThreadPrivate;
class Thread : public QObject, public QQmlParserStatus
{
    Q_OBJECT
    Q_INTERFACES(QQmlParserStatus)
    Q_PROPERTY(QQmlListProperty<QObject> elements READ elements)
    Q_CLASSINFO("DefaultProperty", "elements")

public:
    explicit Thread(QObject *parent = 0);
    ~Thread();
    void classBegin();
    void componentComplete();
    QQmlListProperty<QObject> elements();

Q_SIGNALS:

public Q_SLOTS:
    void test();
private:
    static void append(QQmlListProperty<QObject> *list, QObject *obj);
    static int count(QQmlListProperty<QObject> *list);
    static QObject* at(QQmlListProperty<QObject> *list, int i);
    static void clear(QQmlListProperty<QObject> *list);
    const QScopedPointer<ThreadPrivate> d_ptr;
    Q_DECLARE_PRIVATE(Thread)
};
}
#endif // THREAD_H
