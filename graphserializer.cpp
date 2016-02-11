#include "graphserializer.h"
#include "Dynamic.h"
#include "CustomTypes.h"

GraphSerializer::GraphSerializer() : doc("Graph")
{
}

QDomElement GraphSerializer::variantToDom(QVariant value)
{
    QDomElement domValue = doc.createElement("value");
    if(value.canConvert(QVariant::String))
    {
       QDomText text = doc.createTextNode(value.toString());
       domValue.appendChild(text);
       return domValue;
    }
    if(value.userType() == filePathTypeId())
    {
        FilePathPropertyType path = value.value<FilePathPropertyType>();
        QDomText text = doc.createTextNode(path.toString());
        domValue.appendChild(text);
        return domValue;
    }
    if(value.userType() == newFileTypeId())
    {
        NewFilePropertyType path = value.value<NewFilePropertyType>();
        QDomText text = doc.createTextNode(path.toString());
        domValue.appendChild(text);
        return domValue;
    }
    if(value.userType() == scriptTypeId())
    {
        ScriptPropertyType script = value.value<ScriptPropertyType>();
        QDomText text = doc.createTextNode(script.toString());
        domValue.appendChild(text);
        return domValue;
    }
    if(value.type() == QVariant::PointF)
    {
        QPointF point = value.toPointF();
        QDomElement xDom = doc.createElement("x");
        QDomElement yDom = doc.createElement("y");
        QDomText textX = doc.createTextNode(QString::number(point.x()));
        QDomText textY = doc.createTextNode(QString::number(point.y()));
        domValue.appendChild(xDom);
        domValue.appendChild(yDom);
        xDom.appendChild(textX);
        yDom.appendChild(textY);
        return domValue;
    }
    if(value.type() == QVariant::Point)
    {
        QPoint point = value.toPoint();
        QDomElement xDom = doc.createElement("x");
        QDomElement yDom = doc.createElement("y");
        QDomText textX = doc.createTextNode(QString::number(point.x()));
        QDomText textY = doc.createTextNode(QString::number(point.y()));
        domValue.appendChild(xDom);
        domValue.appendChild(yDom);
        xDom.appendChild(textX);
        yDom.appendChild(textY);
        return domValue;
    }
    if(value.type() == QVariant::Size)
    {
        QSize size = value.toSize();
        QDomElement widthDom = doc.createElement("width");
        QDomElement heightDom = doc.createElement("height");
        QDomText textWidth = doc.createTextNode(QString::number(size.width()));
        QDomText textHeight = doc.createTextNode(QString::number(size.height()));
        domValue.appendChild(widthDom);
        domValue.appendChild(heightDom);
        widthDom.appendChild(textWidth);
        heightDom.appendChild(textHeight);
        return domValue;
    }
    return QDomElement();
}
QVariant GraphSerializer::domToVariant(QDomElement domValue, QString propTypeName)
{
    QVariant::Type type = QVariant::nameToType(propTypeName.toUtf8().constData());
    QVariant strVar(QVariant::String);
    if(strVar.canConvert(type))
    {
        strVar.setValue(domValue.text());
        bool res = strVar.convert(type);
        Q_UNUSED(res)
        return strVar;
    }
    if(type == QVariant::PointF)
    {
        QVariant var(QVariant::PointF);
        QDomElement xDom = domValue.childNodes().at(0).toElement();
        QDomElement yDom = domValue.childNodes().at(1).toElement();
        QPointF point(xDom.text().toDouble(), yDom.text().toDouble());
        var.setValue(point);
        return var;
    }
    if(type == QVariant::Point)
    {
        QVariant var(QVariant::Point);
        QDomElement xDom = domValue.childNodes().at(0).toElement();
        QDomElement yDom = domValue.childNodes().at(1).toElement();
        QPoint point(xDom.text().toInt(), yDom.text().toInt());
        var.setValue(point);
        return var;
    }
    if(type == QVariant::Size)
    {
        QVariant var(QVariant::Size);
        QDomElement widthDom = domValue.childNodes().at(0).toElement();
        QDomElement heightDom = domValue.childNodes().at(1).toElement();
        QSize size(widthDom.text().toInt(), heightDom.text().toInt());
        var.setValue(size);
        return var;
    }
    if(propTypeName == "FilePathPropertyType")
    {
        QVariant var;
        FilePathPropertyType path(domValue.text());
        var.setValue(path);
        return var;
    }
    if(propTypeName == "NewFilePropertyType")
    {
        QVariant var;
        NewFilePropertyType path(domValue.text());
        var.setValue(path);
        return var;
    }
    if(propTypeName == "ScriptPropertyType")
    {
        QVariant var;
        ScriptPropertyType script(domValue.text());
        var.setValue(script);
        return var;
    }
    return QVariant();
}
QDomElement GraphSerializer::getProperties(QObject* object)
{
    QDomElement domProperties = doc.createElement("properties");
    for(int i=0; i<object->metaObject()->propertyCount(); i++)
    {
        QMetaProperty prop = object->metaObject()->property(i);
        QVariant value = object->property(prop.name());
        if(prop.isWritable())
        {
            QDomElement domProperty = doc.createElement("property");
            domProperty.setAttribute("type",value.typeName());
            domProperty.setAttribute("name",prop.name());
            domProperties.appendChild(domProperty);
            QDomElement domValue = variantToDom(value);
            domProperty.appendChild(domValue);
        }
    }
    foreach(QString name, object->dynamicPropertyNames())
    {
        QVariant value = object->property(name.toUtf8().constData());

        QDomElement domProperty = doc.createElement("property");
        domProperty.setAttribute("type",value.typeName());
        domProperty.setAttribute("name",name);
        domProperty.setAttribute("isDynamic", "true");
        domProperties.appendChild(domProperty);
        QDomElement domValue = variantToDom(value);
        domProperty.appendChild(domValue);
    }
    return domProperties;
}

QDomElement GraphSerializer::getXml(QObject* object, QString guid)
{
    QDomElement domObject = doc.createElement("object");
    domObject.setAttribute("type",object->metaObject()->className());
    domObject.setAttribute("id",guid);
    QDomElement domProperties = getProperties(object);
    domObject.appendChild(domProperties);
    return domObject;
}
QDomDocument GraphSerializer::serialize(Graph* graph)
{
    QDomElement root = doc.createElement("objects");
    doc.appendChild(root);
    QHash<QObject*,QUuid> guids;
    foreach(QObject* object, graph->modules())
    {
        QUuid guid = QUuid::createUuid();
        guids[object] = guid;
    }

    foreach(QObject* object, graph->modules())
    {
        QDomElement domObject = getXml(object, guids[object].toString());
        QDomElement connectionsElement = doc.createElement("connections");
        domObject.appendChild(connectionsElement);
        foreach(BlobConnection con, graph->connections().values(object))
        {
            QDomElement connectionElement = doc.createElement("connection");
            QDomElement signalElement = doc.createElement("signal");
            connectionElement.appendChild(signalElement);
            signalElement.setAttribute("name", con.signal().name());
            signalElement.setAttribute("isDynamic", con.signal().isDynamic());
            if(con.signal().isDynamic()) signalElement.setAttribute("index",con.signal().index());
            QDomElement slotElement = doc.createElement("slot");
            connectionElement.appendChild(slotElement);
            slotElement.setAttribute("name", con.slot().name());
            slotElement.setAttribute("isDynamic",con.slot().isDynamic());
            if(con.slot().isDynamic()) slotElement.setAttribute("index",con.slot().index());
            slotElement.setAttribute("owner", guids[con.slot().owner()].toString());
            connectionsElement.appendChild(connectionElement);
        }
        Dynamic* dynamic = dynamic_cast<Dynamic*>(object);
        if(dynamic)
        {
            QDomElement domSignals = doc.createElement("signals");
            domObject.appendChild(domSignals);
            for(int i=0;i<dynamic->signalCount();i++)
            {
                IDynamicSignal* signal = dynamic->signal(i);
                QDomElement domSignal = getXml(signal,QString::number(signal->index()));
                domSignals.appendChild(domSignal);
            }
            QDomElement domSlots = doc.createElement("slots");
            domObject.appendChild(domSlots);
            for(int i=0;i<dynamic->slotCount();i++)
            {
                IDynamicSlot* slot = dynamic->slot(i);
                QDomElement domSlot = getXml(slot,QString::number(slot->index()));
                domSlots.appendChild(domSlot);
            }
        }


        root.appendChild(domObject);
    }
    return doc;
}
void GraphSerializer::setProperties(QDomElement domProperties, QObject *obj)
{
    for(int j=0;j<(int)domProperties.childNodes().length();j++)
    {
        QDomElement domProperty = domProperties.childNodes().at(j).toElement();
        QString propTypeName = domProperty.attribute("type");
        QString propName = domProperty.attribute("name");
        QDomElement domValue = domProperty.firstChildElement();
        QVariant value = domToVariant(domValue, propTypeName);
        bool res = obj->setProperty(propName.toUtf8().constData(), value);
        Q_UNUSED(res)
    }
}

ErrorInfo GraphSerializer::deserialize(QDomDocument document, Graph* graph)
{
    doc = document;
    QDomElement domObjects = doc.firstChildElement();
    QHash<QUuid, QObject*> guid_Object;
    for(int i=0;i<(int)domObjects.childNodes().length();i++)
    {
        QDomElement domObject = domObjects.childNodes().at(i).toElement();
        QUuid guid(domObject.attribute("id"));
        QString typeName = domObject.attribute("type");
        QObject* obj = graph->createObject(typeName);
        if(!obj)
        {
            ErrorInfo info("Could not instatiate module " + typeName, -91);
            return info;
        }
        guid_Object[guid] = obj;
        QDomElement domProperties = domObject.firstChildElement();
        setProperties(domProperties, obj);
        graph->addObject(obj);
        Dynamic* dynamic = dynamic_cast<Dynamic*>(obj);
        if(dynamic)
        {
            QDomElement domSignals = domObject.elementsByTagName("signals").at(0).toElement();
            for(int j=0;j<(int)domSignals.childNodes().length();j++)
            {
                QDomElement domSignal = domSignals.childNodes().at(j).toElement();
                QDomElement domProperties = domSignal.firstChildElement();
                int index = domSignal.attribute("id").toInt();
                IDynamicSignal* signal = dynamic->signal(index);
                if(signal) setProperties(domProperties, signal);
            }
            QDomElement domSlots = domObject.elementsByTagName("slots").at(0).toElement();
            for(int j=0;j<(int)domSlots.childNodes().length();j++)
            {
                QDomElement domSlot = domSlots.childNodes().at(j).toElement();
                QDomElement domProperties = domSlot.firstChildElement();
                int index = domSlot.attribute("id").toInt();
                IDynamicSlot* slot = dynamic->slot(index);
                if(slot) setProperties(domProperties, slot);
            }
        }
    }
    for(int i=0;i<(int)domObjects.childNodes().length();i++)
    {
        QDomElement domObject = domObjects.childNodes().at(i).toElement();
        QUuid fromGuid(domObject.attribute("id"));
        QObject* fromObject = guid_Object[fromGuid];
        QDomElement domProperties = domObject.firstChildElement();
        QDomElement domConnections = domProperties.nextSiblingElement();
        for(int j=0;j<(int)domConnections.childNodes().length();j++)
        {
            QDomElement domConnection = domConnections.childNodes().at(j).toElement();
            QDomElement signalElement = domConnection.childNodes().at(0).toElement();
            QDomElement slotElement = domConnection.childNodes().at(1).toElement();

            QUuid toGuid(slotElement.attribute("owner"));
            QObject* toObject = guid_Object[toGuid];

            bool isDynamic = QVariant(signalElement.attribute("isDynamic")).toBool();
            int index = -1;
            QString name = signalElement.attribute("name");
            if(isDynamic) index = QVariant(signalElement.attribute("index")).toInt();
            QMetaMethod method;
            QObject* methodOwner = NULL;
            if(isDynamic)
            {
                Dynamic* dyn = dynamic_cast<Dynamic*>(fromObject);
                if(dyn)
                {
                    IDynamicSignal* signal = dyn->signal(index);
                    methodOwner = signal;
                    if(methodOwner) method = signal->metaMethod();
                }
            }
            else
            {
                int i = fromObject->metaObject()->indexOfMethod(name.toUtf8().constData());
                method = fromObject->metaObject()->method(i);
            }


            Signal signal(isDynamic,fromObject,method,name,methodOwner,index);

            isDynamic = QVariant(slotElement.attribute("isDynamic")).toBool();
            index = -1;
            methodOwner = NULL;
            if(isDynamic) index = QVariant(slotElement.attribute("index")).toInt();
            name = slotElement.attribute("name");
            if(isDynamic)
            {
                Dynamic* dyn = dynamic_cast<Dynamic*>(toObject);
                IDynamicSlot* slot = dyn->slot(index);
                methodOwner = slot;
                if(slot) method = slot->metaMethod();
            }
            else
            {
                if(toObject)
                {
                    int i = toObject->metaObject()->indexOfMethod(name.toUtf8().constData());
                    method = toObject->metaObject()->method(i);
                }
            }
            Slot slot(isDynamic,toObject,method,name,methodOwner,index);
            bool res = graph->connect(signal,slot);
            Q_UNUSED(res)

        }
    }
    return ErrorInfo();
}
