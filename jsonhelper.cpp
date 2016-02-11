#include "jsonhelper.h"
#include "symbols.h"
#include <QJsonValue>
#include <QVariant>
#include <QJsonObject>
#include <QMetaProperty>
#include <QQmlProperty>
#include <QQmlListReference>
#include <QJsonArray>

namespace QFlow{

JsonHelper::JsonHelper(QObject *parent) : QObject(parent)
{

}

QJSValue JsonHelper::jsonToJS(const QJsonValue& jsonVal)
{
    if(jsonVal.isNull()) return QJSValue(QJSValue::NullValue);
    if(jsonVal.isDouble()) return QJSValue(jsonVal.toDouble());
    if(jsonVal.isBool()) return QJSValue(jsonVal.toBool());
    if(jsonVal.isString()) return QJSValue(jsonVal.toString());
    return QJSValue();
}
QJsonValue JsonHelper::jsToJson(const QJSValue& jsVal)
{
    if(jsVal.isNumber()) return QJsonValue(jsVal.toNumber());
    if(jsVal.isBool()) return QJsonValue(jsVal.toBool());
    if(jsVal.isString()) return QJsonValue(jsVal.toString());
    if(jsVal.isVariant())
    {
        QVariant var = jsVal.toVariant();
        if(var.canConvert<QJsonValue>())
        {
            QJsonValue res = var.value<QJsonValue>();
            return res;
        }
    }
    return QJsonValue();
}
QJsonObject JsonHelper::generateSchema(QObject* obj)
{
    QJsonObject jsonObj;
    const QMetaObject* meta = obj->metaObject();
    QJsonObject properties;
    for(int i=0;i<meta->propertyCount();i++)
    {
        QMetaProperty prop = meta->property(i);
        QVariant value = obj->property(prop.name());
        QString propName = prop.name();

        QQmlProperty qmlProp(obj, propName);
        QJsonObject property;
        if(value.canConvert<QObject*>())
        {
            QObject* childObj = value.value<QObject*>();
            QJsonObject jsonChild = generateSchema(childObj);
            properties.insert(propName, jsonChild);
        }
        else if(value.canConvert<QVariantList>())
        {
            property.insert(KEY_TYPE, KEY_ARRAY);
            properties.insert(propName, property);
        }
        else if(value.canConvert<QVariantMap>()) //TODO: handle variantmap
        {
            property.insert(KEY_TYPE, KEY_ARRAY);
            //properties.insert(propName, property);
        }
        else if(qmlProp.propertyTypeCategory() == QQmlProperty::List)
        {
            QQmlListReference ref(obj, propName.toLatin1());
            QJsonArray arr;
            for(int j=0;j<ref.count();j++)
            {
                QObject* childObj = ref.at(j);
                QJsonObject jsonChild = generateSchema(childObj);
                arr.insert(j, jsonChild);
            }
            property.insert(KEY_TYPE, KEY_ARRAY);
            property.insert("items", arr);
            properties.insert(propName, property);
        }
        else
        {
            QJsonValue val = QJsonValue::fromVariant(value);
            QString type = KEY_STRING;
            if(val.isBool()) type = KEY_BOOLEAN;
            else if(val.isDouble()) type = KEY_NUMBER;
            else if(val.isString()) type = KEY_STRING;
            property.insert(KEY_TYPE, type);
            properties.insert(propName, property);
        }
    }
    jsonObj.insert("properties", properties);
    QJsonObject methods;
    for(int i=0;i<meta->methodCount();i++)
    {
        QMetaMethod method = meta->method(i);
        if(method.access() != QMetaMethod::Public || method.methodType() != QMetaMethod::Slot ||
                method.name() == "deleteLater") continue;
        QJsonObject methodJson;
        QJsonArray params;
        methodJson.insert("returns", method.typeName());
        for(int j=0;j<method.parameterCount();j++)
        {
            QJsonObject param;
            QVariant var(method.parameterType(j), NULL);
            QJsonValue val = QJsonValue::fromVariant(var);
            QString type;
            if(val.isBool()) type = KEY_BOOLEAN;
            else if(val.isDouble()) type = KEY_NUMBER;
            else if(val.isString()) type = KEY_STRING;
            else if(var.canConvert<QString>()) type = KEY_STRING;
            param.insert(KEY_TYPE, type);
            param.insert("name", (QString)method.parameterNames()[j]);
            params.insert(j, param);
        }
        methodJson.insert("params", params);
        QString methodName = method.name();
        methods.insert(methodName, methodJson);
    }
    jsonObj.insert("methods", methods);
    jsonObj.insert(KEY_TYPE, KEY_OBJECT);
    return jsonObj;
}
}
