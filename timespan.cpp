#include "timespan.h"
#include <QStringList>
#include <QString>
#include <QRegExp>
#include <QDataStream>

namespace QFlow{

#define DAY (quint64)(24 * 60 * 60 * 1E3)
#define HOUR (quint64)(60 * 60 * 1E3)
#define MINUTE (quint64)(60 * 1E3)
#define SECOND (quint64)1E3


Timespan::Timespan(quint64 msecs) : _msecs(msecs), _isValid(true)
{
}
Timespan::Timespan(quint64 days, quint64 hours, quint64 minutes, quint64 seconds)
{
    _msecs = 0;
    _msecs += seconds * SECOND;
    _msecs += minutes * MINUTE;
    _msecs += hours * HOUR;
    _msecs += days * DAY;
}

Timespan::Timespan() : _msecs(0), _isValid(false)
{

}
Timespan::Timespan(const Timespan &other)
{
    _isValid = other._isValid;
    _msecs = other._msecs;
}
Timespan::~Timespan()
{

}

Timespan Timespan::fromString(const QString& text)
{
    QRegExp rx("P((\\d+)Y){0,1}((\\d+)M){0,1}((\\d+)D){0,1}T((\\d+)H){0,1}((\\d+)M){0,1}((\\d+)S){0,1}");
    int pos = rx.indexIn(text);
    if(pos == -1) return Timespan();

    QStringList l = rx.capturedTexts();

    quint64 days = 0;
    if(!l[6].isEmpty()) days = l[6].toULongLong();
    quint64 hours = 0;
    if(!l[8].isEmpty()) hours = l[8].toULongLong();
    quint64 minutes = 0;
    if(!l[10].isEmpty()) minutes = l[10].toULongLong();
    quint64 seconds = 0;
    if(!l[12].isEmpty()) seconds = l[12].toULongLong();

    Timespan ts(days, hours, minutes, seconds);
    return ts;

}
bool Timespan::isValid() const
{
    return _isValid;
}
quint64 Timespan::msecsTotal() const
{
    return _msecs;
}
quint64 Timespan::daysTotal() const
{
    quint64 days = _msecs / DAY;
    return days;
}
quint64 Timespan::days() const
{
    return daysTotal();
}

quint64 Timespan::hoursTotal() const
{
    quint64 hours = _msecs / HOUR;
    return hours;
}
quint64 Timespan::hours() const
{
    quint64 ms = _msecs;
    ms -= days() * DAY;
    quint64 res = ms / HOUR;
    return res;
}
quint64 Timespan::minutes() const
{
    quint64 ms = _msecs;
    ms -= days() * DAY;
    ms -= hours() * HOUR;
    quint64 res = ms / MINUTE;
    return res;
}
quint64 Timespan::seconds() const
{
    quint64 ms = _msecs;
    ms -= days() * DAY;
    ms -= hours() * HOUR;
    ms -= minutes() * MINUTE;
    quint64 res = ms / SECOND;
    return res;
}
QString Timespan::toString() const
{
    quint64 d = days();
    quint64 h = hours();
    quint64 m = minutes();
    quint64 s = seconds();
    QString res = QString("P%1DT%2H%3M%4S").arg(d).arg(h).arg(m).arg(s);
    return res;
}
Timespan Timespan::fromJson(const QJsonValue& val)
{
    QString str = val.toString();
    return Timespan::fromString(str);
}
QVariant Timespan::toVar(const QString &str)
{
    Timespan t = Timespan::fromString(str);
    return QVariant::fromValue<Timespan>(t);
}

QJsonValue Timespan::toJson() const
{
    return QJsonValue(toString());
}
bool Timespan::operator==(const Timespan& other) const
{
    return _msecs == other._msecs;
}
bool Timespan::operator<(const Timespan& other) const
{
    return _msecs < other._msecs;
}
Timespan Timespan::elapsedFrom(QDateTime time)
{
    QDateTime now = QDateTime::currentDateTime();
    qint64 msecs = time.msecsTo(now);
    Timespan span(msecs);
    return span;
}
}
QDataStream &operator<<(QDataStream & stream, const QFlow::Timespan &timespan)
{
    stream << timespan.msecsTotal();
    return stream;
}

QDataStream &operator>>(QDataStream & stream, QFlow::Timespan &timespan)
{
    quint64 msecsTotal;
    stream >> msecsTotal;
    timespan = QFlow::Timespan(msecsTotal);
    return stream;
}
