#ifndef TREEITEM_H
#define TREEITEM_H

#include "coreplugin_global.h"
#include <QMap>
#include <QVariant>

namespace QFlow{

class TreeItemPrivate;
class COREPLUGIN_EXPORT TreeItem
{
public:
    TreeItem();
    TreeItem(QVariantList data, QString uri = QString());
    TreeItem(QString key, TreeItem* parent);
    ~TreeItem();
    QVariantList data() const;
    QString key() const;
    TreeItem* find(QString uri) const;
    TreeItem* parent() const;
    TreeItem* child(int index) const;
    int childCount() const;
    bool exists(QString key) const;
    QList<TreeItem*> add(QString uri);
    bool isValid() const;
    QList<TreeItem*> descendants() const;
    QVariant dataAt(int column) const;
    int columnCount() const;
    int index() const;
    QString uri() const;
private:
    const QScopedPointer<TreeItemPrivate> d_ptr;
    Q_DECLARE_PRIVATE(TreeItem)
};
}
#endif // TREEITEM_H
