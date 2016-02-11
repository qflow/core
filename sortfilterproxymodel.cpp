#include "sortfilterproxymodel.h"

namespace QFlow{

SortFilterProxyModel::SortFilterProxyModel(QObject *parent) : QSortFilterProxyModel(parent)
{

}
void SortFilterProxyModel::setPattern(QString value)
{
    _pattern = value;
    QString escaped = QRegExp::escape(value);
    QRegExp re("^" + escaped + ".*");
    setFilterRegExp(re);
    Q_EMIT patternChanged();
}
QString SortFilterProxyModel::pattern() const
{
    return _pattern;
}
bool SortFilterProxyModel::filterAcceptsRow(int source_row, const QModelIndex & source_parent) const
{
    QModelIndex source_index = sourceModel()->index(source_row, filterKeyColumn(), source_parent) ;
    QString key = sourceModel()->data(source_index, filterRole()).toString();
    bool result = filterRegExp().exactMatch(key);
    if(_pattern.contains(key))
    {
        QModelIndex dest_index = mapFromSource(source_index);
        Q_EMIT expanded(dest_index);
        return true;
    }
    return result;
}
}
