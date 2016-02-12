#ifndef COMPLEX_H
#define COMPLEX_H

#include "coreplugin_global.h"
#include <QMetaType>

template<typename T>
class COREPLUGIN_EXPORT Complex
{
    T _real;
    T _imaginary;
public:
    Complex();
    Complex(T real, T imaginary);
    Complex(const Complex<T>& other);
    T imaginary() const;
    T real() const;
    T modulus() const;
    void setImaginary(T value);
    void setReal(T value);
    bool operator ==(const Complex& other) const;
    ~Complex();
};
typedef Complex<qreal> RealComplex;
Q_DECLARE_METATYPE(RealComplex)

typedef Complex<float> FloatComplex;
Q_DECLARE_METATYPE(FloatComplex)
#endif // COMPLEX_H
