#ifndef GRAPHSERIALIZER_H
#define GRAPHSERIALIZER_H

#include <QUuid>
#include <QHash>
#include <QtXml>
#include <QVariant>
#include "Graph.h"

class GraphSerializer
{
protected:
    QDomDocument doc;
    QDomElement variantToDom(QVariant value);
    QVariant domToVariant(QDomElement domValue, QString propTypeName);
    QDomElement getXml(QObject* object, QString guid);
    QDomElement getProperties(QObject* object);
    void setProperties(QDomElement domProperties, QObject* object);
public:
    GraphSerializer();
    QDomDocument serialize(Graph* graph);
    ErrorInfo deserialize(QDomDocument document, Graph* graph);
};

#endif // GRAPHSERIALIZER_H
