#include "treeitem.h"

namespace QFlow{

class TreeItemPrivate
{
public:
    QVariantList _data;
    QString _key;
    QMap<QString, TreeItem*> _children;
    bool _valid;
    TreeItem* _parent;
    QString _fullUri;
    TreeItemPrivate() : _valid(false), _parent(NULL)
    {

    }

    ~TreeItemPrivate()
    {
        qDeleteAll(_children.values());
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

TreeItem::TreeItem() : d_ptr(new TreeItemPrivate())
{

}
TreeItem::TreeItem(QString key, TreeItem* parent) : d_ptr(new TreeItemPrivate())
{
    Q_D(TreeItem);
    d->_key = key;
    d->_valid = true;
    d->_parent = parent;
    d->_parent->d_ptr->_children[key] = this;
    d->_data.append(key);
    if(d->_parent->d_ptr->_fullUri.isEmpty())
    {
        d->_fullUri = key;
    }
    else
    {
        d->_fullUri = QString("%1.%2").arg(d->_parent->d_ptr->_fullUri).arg(key);
    }
    d->_data.append(d->_fullUri);
}
TreeItem::TreeItem(QVariantList data, QString uri) : d_ptr(new TreeItemPrivate())
{
    Q_D(TreeItem);
    d->_key = uri;
    d->_data = data;
    d->_valid = true;
}
TreeItem::~TreeItem()
{
}
QVariantList TreeItem::data() const
{
    Q_D(const TreeItem);
    return d->_data;
}
QString TreeItem::key() const
{
    Q_D(const TreeItem);
    return d->_key;
}
TreeItem* TreeItem::parent() const
{
    Q_D(const TreeItem);
    return d->_parent;
}
TreeItem* TreeItem::find(QString uri) const
{
    Q_D(const TreeItem);
    QStringList elements = uri.split('.');
    if(d->_children.contains(elements[0]))
    {
        TreeItem* child = d->_children[elements[0]];
        if(elements.count() == 1) return child;
        elements.removeFirst();
        return child->find(TreeItemPrivate::path(elements));
    }
    else return NULL;
}
bool TreeItem::exists(QString key) const
{
    return find(key);
}
QList<TreeItem*> TreeItem::add(QString uri)
{
    Q_D(TreeItem);
    QList<TreeItem*> newList;
    QStringList elements = uri.split('.');
    if(elements.count() == 1)
    {
        TreeItem* newItem = new TreeItem(uri, this);
        newList << newItem;
        return newList;
    }
    if(!d->_children.contains(elements[0]))
    {
        QString childKey = elements[0];
        TreeItem* newChild = new TreeItem(childKey, this);
        newList << newChild;
    }
    TreeItem* child = d->_children[elements[0]];
    elements.removeFirst();
    QString newKey = TreeItemPrivate::path(elements);
    return newList << child->add(newKey);
}
bool TreeItem::isValid() const
{
    Q_D(const TreeItem);
    return d->_valid;
}
QList<TreeItem*> TreeItem::descendants() const
{
    Q_D(const TreeItem);
    QList<TreeItem*> list;
    for(TreeItem* child: d->_children) {
        list.append(child);
        list.append(child->descendants());
    }
    return list;
}
TreeItem* TreeItem::child(int index) const
{
    Q_D(const TreeItem);
    return d->_children.values()[index];
}
int TreeItem::childCount() const
{
    Q_D(const TreeItem);
    return d->_children.count();
}
int TreeItem::columnCount() const
{
    Q_D(const TreeItem);
    return d->_data.count();
}
QVariant TreeItem::dataAt(int column) const
{
    Q_D(const TreeItem);
    if(d->_data.count() <= column) return QVariant();
    return d->_data.at(column);
}
int TreeItem::index() const
{
    Q_D(const TreeItem);
    if(d->_parent)
    {
        QList<QString> siblings = d->_parent->d_ptr->_children.keys();
        int index = siblings.indexOf(d->_key);
        return index;
    }
    return 0;
}
QString TreeItem::uri() const
{
    Q_D(const TreeItem);
    return d->_fullUri;
}
}
