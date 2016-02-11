#ifndef CANVAS_H
#define CANVAS_H

#include <QObject>
#include <QQmlListProperty>
#include <QQmlParserStatus>

namespace QFlow{

class CanvasPrivate;
class Canvas : public QObject, public QQmlParserStatus
{
    Q_OBJECT
    Q_INTERFACES(QQmlParserStatus)
    Q_PROPERTY(QQmlListProperty<QObject> data READ getData)
    Q_CLASSINFO("DefaultProperty", "data")
public:
    explicit Canvas(QObject *parent = 0);
    QQmlListProperty<QObject> getData();
    ~Canvas();
    void classBegin();
    void componentComplete();
signals:
    void completed();
public slots:
private:
    const QScopedPointer<CanvasPrivate> d_ptr;
    Q_DECLARE_PRIVATE(Canvas)
};
}
#endif // CANVAS_H
