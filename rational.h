#ifndef RATIONAL_H
#define RATIONAL_H
 
#include "coreplugin_global.h"
#include <QObject>
#include <QJsonValue>
 
namespace QFlow{

class COREPLUGIN_EXPORT Rational
{
    Q_GADGET
    Q_PROPERTY(int numerator READ numerator)
    Q_PROPERTY(int denominator READ denominator)
    int _p;
    int _q;
public:
    Rational();
    Rational(int p, int q);
    Rational(const Rational& other);
    static Rational fromString(const QString& str);
    QString toString() const;
    QJsonValue toJson() const;
    bool isValid() const;
    static Rational fromJson(const QJsonValue& val);
    inline int p() const{return _p;}
    inline int q() const{return _q;}
    inline int numerator() const{return _p;}
    inline int denominator() const{return _q;}
    qreal value() const;
    static QVariant toVar(const QString &str);
    bool operator==(const Rational& other) const;
    bool operator<(const Rational& other) const;
};
}
Q_DECLARE_METATYPE(QFlow::Rational)
 
#endif // RATIONAL_H
