#ifndef TIMESPAN_H
#define TIMESPAN_H

#include "coreplugin_global.h"
#include <QJsonValue>
#include <QVariant>
#include <QDateTime>

namespace QFlow{

class COREPLUGIN_EXPORT Timespan
{
    Q_GADGET
    Q_PROPERTY(quint64 days READ days)
    Q_PROPERTY(quint64 hours READ hours)
    Q_PROPERTY(quint64 minutes READ minutes)
    Q_PROPERTY(quint64 seconds READ seconds)
public:
    Timespan(quint64 msecsTotal);
    Timespan(quint64 days, quint64 hours, quint64 minutes, quint64 seconds);
    Timespan();
    Timespan(const Timespan& other);
    ~Timespan();
    static Timespan fromString(const QString& text);
    static Timespan fromJson(const QJsonValue& val);
    static QVariant toVar(const QString &str);
    QString toString() const;
    QJsonValue toJson() const;
    bool operator==(const Timespan& other) const;
    bool operator<(const Timespan& other) const;
    bool isValid() const;
    quint64 msecsTotal() const;
    quint64 daysTotal() const;
    quint64 hoursTotal() const;
    quint64 days() const;
    quint64 hours() const;
    quint64 minutes() const;
    quint64 seconds() const;
    static Timespan elapsedFrom(QDateTime time);
private:
    quint64 _msecs;
    bool _isValid;
};
}
Q_DECLARE_METATYPE(QFlow::Timespan)

COREPLUGIN_EXPORT QDataStream &operator<<(QDataStream & stream, const QFlow::Timespan &timespan);
COREPLUGIN_EXPORT QDataStream &operator>>(QDataStream & stream, QFlow::Timespan &timespan);
#endif // TIMESPAN_H
