#include "canvas.h"
#include "startable.h"

namespace QFlow{

class CanvasPrivate
{
public:
    QList<QObject*> _data;
    CanvasPrivate()
    {

    }
    ~CanvasPrivate()
    {

    }
};

Canvas::Canvas(QObject *parent) : QObject(parent), QQmlParserStatus(), d_ptr(new CanvasPrivate())
{
}

Canvas::~Canvas()
{
}

QQmlListProperty<QObject> Canvas::getData()
{
    Q_D(Canvas);
    return QQmlListProperty<QObject>(this, d->_data);
}

void Canvas::classBegin()
{

}
void Canvas::componentComplete()
{
    emit completed();
}
}
