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
    QString _fullUri;
    QHash<QString, RadixTreeNode<T> > _children;
    bool _valid;
    bool _genuine;
    QExplicitlySharedDataPointer<RadixTreeNodePrivate<T> > _parent;
    RadixTreeNodePrivate() : QSharedData(), _valid(false), _genuine(false)
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
class RadixTreeNode
{
    using RadixTreeNodeList = QList<RadixTreeNode<T>>;
public:
    RadixTreeNode();
    RadixTreeNode(QString key, T data);
    RadixTreeNode(QString key, RadixTreeNode<T> parent);
    ~RadixTreeNode();
    RadixTreeNode(const RadixTreeNode<T>& other);
    RadixTreeNode<T> &operator=(const RadixTreeNode<T> &other);
    bool operator==(const RadixTreeNode<T> &other);
    T data() const;
    QString key() const;
    QString uri() const;
    RadixTreeNode<T> find(QString key) const;
    RadixTreeNode<T> parent() const;
    RadixTreeNode<T> child(int index) const;
    int childCount() const;
    bool exists(QString key) const;
    bool isValid() const;
    bool isLeaf() const;
    RadixTreeNodeList descendants() const;
    RadixTreeNodeList children() const;
    RadixTreeNode<T> add(QString uri);
    RadixTreeNode<T> add(QString uri, T data);
    bool remove(QString uri);
    bool containsGenuine(QString uri) const;
    void setData(T data);
    T findData(QString uri) const;
    QStringList genuineUris() const;
    QStringList childrenKeys(QString uri) const;
private:
    RadixTreeNode(QExplicitlySharedDataPointer<RadixTreeNodePrivate<T>> other);
    QList<RadixTreeNode<T>> addInternal(QString uri);
    void prune(RadixTreeNode<T> node);
    QExplicitlySharedDataPointer<RadixTreeNodePrivate<T> > d;
};

template<typename T>
RadixTreeNode<T>::RadixTreeNode(QExplicitlySharedDataPointer<RadixTreeNodePrivate<T>> other): d(other)
{

}

template<typename T>
RadixTreeNode<T>::RadixTreeNode() : d(new RadixTreeNodePrivate<T>())
{

}
template<typename T>
RadixTreeNode<T>::RadixTreeNode(QString key, RadixTreeNode<T> parent) : d(new RadixTreeNodePrivate<T>())
{
    d->_key = key;
    d->_valid = true;
    d->_parent = parent.d;
    d->_parent->_children[key] = *this;
    if(d->_parent->_fullUri.isEmpty())
    {
        d->_fullUri = key;
    }
    else
    {
        d->_fullUri = QString("%1.%2").arg(d->_parent->_fullUri).arg(key);
    }
}
template<typename T>
void RadixTreeNode<T>::setData(T data)
{
    d->_data = data;
}
template<typename T>
bool RadixTreeNode<T>::containsGenuine(QString uri) const
{
    RadixTreeNode<T> node = find(uri);
    return node.d->_genuine;
}
template<typename T>
QStringList RadixTreeNode<T>::genuineUris() const
{
    RadixTreeNodeList desc = descendants();
    QStringList list;
    for(RadixTreeNode<T> node: desc)
    {
        if(node.d->_genuine) list << node.uri();
    }
    return list;
}
template<typename T>
T RadixTreeNode<T>::findData(QString uri) const
{
    RadixTreeNode<T> node = find(uri);
    return node.data();
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
QString RadixTreeNode<T>::uri() const
{
    return d->_fullUri;
}
template<typename T>
RadixTreeNode<T> RadixTreeNode<T>::parent() const
{
    return RadixTreeNode<T>(d->_parent);
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
bool RadixTreeNode<T>::isLeaf() const
{
    return d->_children.isEmpty();
}
template<typename T>
QList<RadixTreeNode<T>> RadixTreeNode<T>::addInternal(QString uri)
{
    QList<RadixTreeNode<T>> newList;
    QStringList elements = uri.split('.');
    if(elements.count() == 1)
    {
        RadixTreeNode<T> newItem(uri, *this);
        newList << newItem;
        return newList;
    }
    if(!d->_children.contains(elements[0]))
    {
        QString childKey = elements[0];
        RadixTreeNode<T> newChild(childKey, *this);
        newList << newChild;
    }
    RadixTreeNode<T> child = d->_children[elements[0]];
    elements.removeFirst();
    QString newKey = RadixTreeNodePrivate<T>::path(elements);
    return newList << child.addInternal(newKey);
}
template<typename T>
RadixTreeNode<T> RadixTreeNode<T>::add(QString uri)
{
    QList<RadixTreeNode<T>> items = addInternal(uri);
    for(RadixTreeNode<T> item: items)
    {
        if(item.uri() == uri)
        {
            item.d->_genuine = true;
            return item;
        }
    }
    return RadixTreeNode<T>();
}
template<typename T>
RadixTreeNode<T> RadixTreeNode<T>::add(QString uri, T data)
{
    RadixTreeNode<T> node = add(uri);
    if(node.isValid()) node.setData(data);
    return node;
}
template<typename T>
void RadixTreeNode<T>::prune(RadixTreeNode<T> node)
{
    if(node.isLeaf() && !node.d->_genuine && node.isValid())
    {
        node.parent().d->_children.remove(node.key());
        if(node.parent().isValid()) prune(node.parent());
    }
}

template<typename T>
bool RadixTreeNode<T>::remove(QString uri)
{
    RadixTreeNode<T> item = find(uri);
    if(!item.isValid()) return false;
    item.parent().d->_children.remove(item.key());
    prune(item.parent());
    return true;
}
template<typename T>
bool RadixTreeNode<T>::isValid() const
{
    return d->_valid;
}
template<typename T>
RadixTreeNodeList<T> RadixTreeNode<T>::descendants() const
{
    QList<RadixTreeNode<T> > list;
    for(RadixTreeNode<T> child: d->_children) {
        list.append(child);
        list.append(child.descendants());
    }
    return list;
}
template<typename T>
RadixTreeNodeList<T> RadixTreeNode<T>::children() const
{
    RadixTreeNodeList list;
    for(RadixTreeNode<T> child: d->_children) {
        list.append(child);
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
template<typename T>
QStringList RadixTreeNode<T>::childrenKeys(QString uri) const
{
    QStringList list;
    RadixTreeNode<T> node = find(uri);
    for(RadixTreeNode<T> child: node.children())
    {
        list << child.key();
    }
    return list;
}

typedef RadixTreeNode<QVariantList> TreeItemBase;
}
#endif // RADIXTREENODE_H
