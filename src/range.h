#ifndef RANGE_H
#define RANGE_H

#include "coreplugin_global.h"
#include "timestamped.h"
#include <QMetaType>
#include <QJsonValue>

namespace QFlow{

template<typename T>
class COREPLUGIN_EXPORT Range
{
    T _from;
    T _to;
public:
    Range();
    Range(T from, T to);
    Range(const Range<T>& other);
    static Range<T> fromString(const QString& str);
    QString toString() const;
    inline T from() const{return _from;}
    inline T to() const{return _to;}
    T dist() const;
    bool operator==(const Range<T> &other) const;
    bool operator!=(const Range<T> &other) const;
    QJsonValue toJson() const;
    static Range<T> fromJson(const QJsonValue& val);
    static QVariant toVar(const QString& str);
};
typedef Range<int> IntRange;
typedef Range<qreal> RealRange;
typedef Range<Timestamp> TimestampRange;
}

Q_DECLARE_METATYPE(QFlow::IntRange)

Q_DECLARE_METATYPE(QFlow::RealRange)

Q_DECLARE_METATYPE(QFlow::TimestampRange)

#endif // RANGE_H
