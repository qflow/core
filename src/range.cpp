#include "range.h"
#include <QStringList>
#include <QVariant>
#include <QJsonObject>

namespace QFlow{

template<typename T>
Range<T>::Range() : _from(0), _to(0)
{
}
template<typename T>
Range<T>::Range(const Range &other) : _from(other._from), _to(other._to)
{

}
template<typename T>
Range<T>::Range(T low, T high) : _from(low), _to(high)
{

}
template<typename T>
Range<T> Range<T>::fromString(const QString& str)
{
    QStringList list = str.split(',');
    QVariant fromVar(list[0]);
    QVariant toVar(list[1]);
    T low = fromVar.value<T>();
    T high = toVar.value<T>();
    return Range<T>(low,high);
}
template<typename T>
QString Range<T>::toString() const
{
    QString str = QString("%1,%2").arg(_from).arg(_to);
    return str;
}
template<typename T>
T Range<T>::dist() const
{
    return _to - _from;
}
template<typename T>
bool Range<T>::operator==(const Range<T> &other) const
{
    return (_from==other._from && _to==other._to);
}
template<typename T>
bool Range<T>::operator!=(const Range<T> &other) const
{
    return !operator==(other);
}
template<typename T>
QJsonValue Range<T>::toJson() const
{
    QJsonObject res;
    res.insert("from", QString("%1").arg(_from));
    res.insert("to", QString("%1").arg(_to));
    return res;
}
template<typename T>
Range<T> Range<T>::fromJson(const QJsonValue& val)
{
    const QJsonObject obj = val.toObject();
    QVariant fromVar = obj["from"];
    QVariant toVar = obj["to"];
    return Range<T>(fromVar.value<T>(), toVar.value<T>());
}
template<typename T>
QVariant Range<T>::toVar(const QString &str)
{
    Range<T> range = Range<T>::fromString(str);
    return QVariant::fromValue<Range<T> >(range);
}

template class Range<int>;
template class Range<qreal>;
template class Range<Timestamp>;
}
