#ifndef JSONHELPER_H
#define JSONHELPER_H

#include "coreplugin_global.h"
#include <QJSValue>
#include <QObject>

namespace QFlow{

class COREPLUGIN_EXPORT JsonHelper : public QObject
{
    Q_OBJECT
public:
    explicit JsonHelper(QObject *parent = 0);
    static QJSValue jsonToJS(const QJsonValue& jsonVal);
    static QJsonValue jsToJson(const QJSValue& jsVal);
    static QJsonObject generateSchema(QObject* obj);
Q_SIGNALS:

public Q_SLOTS:
};
}
#endif // JSONHELPER_H
