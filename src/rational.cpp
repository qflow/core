#include "rational.h"
#include <QStringList>
#include <QVariant>
 
namespace QFlow{

Rational::Rational() : _p(0), _q(0)
{
}
Rational::Rational(const Rational &other) : _p(other._p), _q(other._q)
{
 
}
Rational::Rational(int p, int q) : _p(p), _q(q)
{
 
}
Rational Rational::fromString(const QString& str)
{
    QStringList list = str.split('/');
    if(list.length() == 2)
    {
        int p = list[0].toInt();
        int q = list[1].toInt();
        return Rational(p,q);
    }
    if(list.length() == 1)
    {
        int p = list[0].toInt();
        return Rational(p,1);
    }
    return Rational();
}
QString Rational::toString() const
{
    return QString("%1/%2").arg(_p).arg(_q);
}
QJsonValue Rational::toJson() const
{
    return QJsonValue(toString());
}
Rational Rational::fromJson(const QJsonValue &val)
{
    return Rational::fromString(val.toString());
}

qreal Rational::value() const
{
    return _p/_q;
}
bool Rational::isValid() const
{
    bool res = _q != 0;
    return res;
}
QVariant Rational::toVar(const QString &str)
{
    Rational rat = Rational::fromString(str);
    return QVariant::fromValue<Rational>(rat);
}
bool Rational::operator==(const Rational& other) const
{
    return _p == other._p && _q == other._q;
}
bool Rational::operator<(const Rational& other) const
{
    return value() < other.value();
}
}
