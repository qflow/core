#ifndef RADIXTREENODE_H
#define RADIXTREENODE_H

#include "coreplugin_global.h"
#include <QExplicitlySharedDataPointer>
#include <QObject>
#include <QHash>
#include <QVariant>

namespace QFlow{

template<typename T>
class RadixTreeNode;

template<typename T>
using RadixTreeNodeList = QList<RadixTreeNode<T> >;

template<typename T>
class RadixTreeNodePrivate : public QSharedData
{
public:
    T _data;
    QString _key;
    QHash<QString, RadixTreeNode<T> > _children;
    bool _valid;
    RadixTreeNode<T> _parent;
    RadixTreeNodePrivate() : QSharedData(), _valid(false)
    {

    }
    RadixTreeNodePrivate(const RadixTreeNodePrivate<T> &other) : QSharedData(other), _data(other._data),
        _key(other._key), _children(other._children)
    {

    }

    ~RadixTreeNodePrivate()
    {

    }
    static QString path(QStringList elements)
    {
        QString result = elements[0];
        for(int i=1;i<elements.count();i++)
        {
            result += "." + elements[i];
        }
        return result;
    }

};

template<typename T>
class COREPLUGIN_EXPORT RadixTreeNode
{
private:
    void add(QString key, RadixTreeNode<T> node);
public:
    RadixTreeNode();
    RadixTreeNode(QString key, T data);
    RadixTreeNode(QString key);
    ~RadixTreeNode();
    RadixTreeNode(const RadixTreeNode<T>& other);
    RadixTreeNode<T> &operator=(const RadixTreeNode<T> &other);
    bool operator==(const RadixTreeNode<T> &other);
    T data() const;
    QString key() const;
    RadixTreeNode<T> find(QString key) const;
    RadixTreeNode<T> parent() const;
    RadixTreeNode<T> child(int index) const;
    int childCount() const;
    bool exists(QString key) const;
    void add(RadixTreeNode<T> node);
    bool isValid() const;
    QList<RadixTreeNode<T> > descendants() const;
private:
    QExplicitlySharedDataPointer<RadixTreeNodePrivate<T> > d;
};
template<typename T>
RadixTreeNode<T>::RadixTreeNode() : d(new RadixTreeNodePrivate<T>())
{

}
template<typename T>
RadixTreeNode<T>::RadixTreeNode(QString key) : d(new RadixTreeNodePrivate<T>())
{
    d->_key = key;
    d->_valid = true;
}
template<typename T>
RadixTreeNode<T>::RadixTreeNode(QString key, T data) : d(new RadixTreeNodePrivate<T>())
{
    d->_key = key;
    d->_data = data;
    d->_valid = true;
}
template<typename T>
RadixTreeNode<T>::~RadixTreeNode()
{

}
template<typename T>
RadixTreeNode<T>::RadixTreeNode(const RadixTreeNode<T> &other) : d(other.d)
{

}
template<typename T>
RadixTreeNode<T> &RadixTreeNode<T>::operator=(const RadixTreeNode<T> & other)
{
    if (this == &other)
    return *this;
    d = other.d;
    return *this;
}
template<typename T>
bool RadixTreeNode<T>::operator ==(const RadixTreeNode<T> & other)
{
    return d->_key == other.key();
}

template<typename T>
T RadixTreeNode<T>::data() const
{
    return d->_data;
}
template<typename T>
QString RadixTreeNode<T>::key() const
{
    return d->_key;
}
template<typename T>
RadixTreeNode<T> RadixTreeNode<T>::parent() const
{
    return d->_parent;
}
template<typename T>
RadixTreeNode<T> RadixTreeNode<T>::find(QString key) const
{
    QStringList elements = key.split('.');
    if(d->_children.contains(elements[0]))
    {
        RadixTreeNode<T> child = d->_children[elements[0]];
        if(elements.count() == 1) return child;
        elements.removeFirst();
        return child.find(RadixTreeNodePrivate<T>::path(elements));
    }
    else return RadixTreeNode<T>();
}
template<typename T>
bool RadixTreeNode<T>::exists(QString key) const
{
    return find(key).isValid();
}
template<typename T>
void RadixTreeNode<T>::add(QString key, RadixTreeNode<T> node)
{
    QStringList elements = key.split('.');
    if(elements.count() == 1)
    {
        d->_children[elements[0]] = node;
        node.d->_parent = *this;
        return;
    }
    if(!d->_children.contains(elements[0]))
    {
        QString childKey = d->_key;
        if(!childKey.isEmpty()) childKey += ".";
        childKey += elements[0];
        RadixTreeNode<T> child(childKey);
        d->_children[elements[0]] = child;
        child.d->_parent = *this;
    }
    RadixTreeNode<T> child = d->_children[elements[0]];
    elements.removeFirst();
    QString newKey = RadixTreeNodePrivate<T>::path(elements);
    child.add(newKey, node);
}
template<typename T>
void RadixTreeNode<T>::add(RadixTreeNode<T> node)
{
    add(node.key(), node);
}

template<typename T>
bool RadixTreeNode<T>::isValid() const
{
    return d->_valid;
}
template<typename T>
QList<RadixTreeNode<T> > RadixTreeNode<T>::descendants() const
{
    QList<RadixTreeNode<T> > list;
    for(RadixTreeNode<T> child: d->_children) {
        list.append(child);
        list.append(child.descendants());
    }
    return list;
}
template<typename T>
RadixTreeNode<T> RadixTreeNode<T>::child(int index) const
{
    return d->_children.values()[index];
}
template<typename T>
int RadixTreeNode<T>::childCount() const
{
    return d->_children.count();
}
typedef RadixTreeNode<QVariantList> TreeItemBase;
}
#endif // RADIXTREENODE_H
