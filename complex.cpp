#include "complex.h"
#include <QtMath>

template<typename T>
Complex<T>::Complex() : _real(0), _imaginary(0)
{

}
template<typename T>
Complex<T>::Complex(T real, T imaginary) : _real(real), _imaginary(imaginary)
{

}
template<typename T>
Complex<T>::Complex(const Complex<T> &other) : _real(other._real), _imaginary(other._imaginary)
{

}
template<typename T>
T Complex<T>::real() const
{
    return _real;
}
template<typename T>
void Complex<T>::setReal(T value)
{
    _real = value;
}
template<typename T>
void Complex<T>::setImaginary(T value)
{
    _imaginary = value;
}

template<typename T>
T Complex<T>::imaginary() const
{
    return _imaginary;
}
template<typename T>
Complex<T>::~Complex()
{

}
template<typename T>
T Complex<T>::modulus() const
{
    return qSqrt(_imaginary*_imaginary + _real*_real);
}
template<typename T>
bool Complex<T>::operator ==(const Complex& other) const
{
    return (other._imaginary == _imaginary) && (other._real == _real);
}


template class Complex<qreal>;
template class Complex<float>;
