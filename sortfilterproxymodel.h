#ifndef SORTFILTERPROXYMODEL_H
#define SORTFILTERPROXYMODEL_H

#include <QObject>
#include <QSortFilterProxyModel>

namespace QFlow{

class SortFilterProxyModel : public QSortFilterProxyModel
{
    Q_OBJECT
    Q_PROPERTY(QAbstractItemModel* sourceModel READ sourceModel WRITE setSourceModel)
    Q_PROPERTY(QString pattern READ pattern WRITE setPattern NOTIFY patternChanged)

    QString _pattern;
public:
    explicit SortFilterProxyModel(QObject *parent = 0);
Q_SIGNALS:
    void patternChanged();
    void expanded(const QModelIndex& index) const;
public Q_SLOTS:
    QString pattern() const;
    void setPattern(QString value);
protected:
    bool filterAcceptsRow(int source_row, const QModelIndex & source_parent) const;
};
}
#endif // SORTFILTERPROXYMODEL_H
