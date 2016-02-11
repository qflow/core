#ifndef INITIABLE_H
#define INITIABLE_H
#include "coreplugin_global.h"
#include "errorinfo.h"
#include <QObject>
#include <QQmlParserStatus>

namespace QFlow{

class Initiable
{
public:
    virtual ErrorInfo init() = 0;
    virtual ErrorInfo deinit() = 0;
};
class COREPLUGIN_EXPORT QmlInitiable: public Initiable, public QQmlParserStatus
{
    Q_INTERFACES(QQmlParserStatus)
public:
    void classBegin();
    void componentComplete();
    virtual ErrorInfo deinit();
};
}
#endif // INITIABLE_H

