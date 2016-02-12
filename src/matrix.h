#ifndef MATRIX_H
#define MATRIX_H

#include "coreplugin_global.h"
#include <QMetaType>
#include <QExplicitlySharedDataPointer>
#include <QSize>
#include "complex.h"

namespace QFlow{

template<typename T> class MatrixPrivate;

#define AUTO_DIST -1

template <typename T>
class COREPLUGIN_EXPORT Matrix
{
    friend class FFTMany1DBackward;
    friend class FFTMany1DForward;
public:
    enum IteratorDirection{Column = 0, Row = 1};
    class COREPLUGIN_EXPORT const_iterator
    {
        friend class Matrix;
    public:
        inline const T& operator*() const{return _data[_index];}
        bool operator!=( const const_iterator& ) const;
        inline const_iterator& operator++(){_index += _increment; return *this;}
        const_iterator operator-(int dec);
        const_iterator& toNextRow();
        const_iterator& reset();
        const_iterator(const const_iterator &other);
        const_iterator row() const;
        const_iterator column() const;
        const_iterator left() const;
        const_iterator topLeft() const;
        const_iterator topRight() const;
        const_iterator right() const;
        const_iterator top() const;
        const_iterator bottom() const;
        const_iterator bottomLeft() const;
        const_iterator bottomRight() const;
        const_iterator& moveBy(int increment);
    private:
        const_iterator(int index, MatrixPrivate<T>* matrix, IteratorDirection dir = Column);
        int _increment;
        int _index;
        int _originalIndex;
        MatrixPrivate<T>* _matrix;
        T* _data;
    };
    class COREPLUGIN_EXPORT iterator
    {
        friend class Matrix;
    public:
        inline T& operator*() const{return _data[_index];}
        bool operator!=( const iterator& ) const;
        inline iterator& operator++(){_index += _increment; return *this;}
        iterator& toNextRow();
        iterator(const iterator &other);
        iterator row() const;
        iterator column() const;
        iterator left() const;
        iterator topLeft() const;
        iterator topRight() const;
        iterator right() const;
        iterator top() const;
        iterator bottom() const;
        iterator bottomLeft() const;
        iterator bottomRight() const;
        iterator& moveBy(int increment);
        iterator& moveTo(int index);
    private:
        iterator(int index, MatrixPrivate<T>* matrix, IteratorDirection dir = Column);
        int _increment;
        int _index;
        MatrixPrivate<T>* _matrix;
        T* _data;
    };
    Matrix();
    Matrix(int columns, int rows, int dist = AUTO_DIST);
    Matrix(QSize size, int sride = AUTO_DIST);
    Matrix(const Matrix<T> &other);
    ~Matrix();
    void init(int columns, int rows, int dist = AUTO_DIST);
    void init(QSize size, int dist = AUTO_DIST);
    QSize size() const;
    Matrix<T> &operator=(const Matrix<T> &other);
    bool operator==(const Matrix<T> &other) const;
    bool operator!=(const Matrix<T> &other) const;
    bool operator!() const;
    operator bool() const;
    T* data();
    const T* data() const;
    int columns() const;
    int rows() const;
    int dist() const;
    void fill(T value);
    void fill(const Matrix<T>& other);
    Matrix<T> copy() const;
    int elements() const;
    Matrix<T> transposed() const;
    const Matrix<T> rows(int start, int howmany) const;
    int dataElements() const;

    const_iterator begin(IteratorDirection dir = Column) const;
    const_iterator end(IteratorDirection dir = Column) const;
    const_iterator at(int column, int row, IteratorDirection dir = Column) const;
    iterator begin(IteratorDirection dir = Column);
    iterator end(IteratorDirection dir = Column);
    iterator at(int column, int row, IteratorDirection dir = Column);
private:
    QExplicitlySharedDataPointer<MatrixPrivate<T> > d;
};
typedef Matrix<bool> BoolMatrix;
typedef BoolMatrix Mask;
class AcceptsMask
{
public:
    virtual void from(const Mask& mask) = 0;
};

class OutputsMask
{
public:
    virtual void to(const Mask& mask) = 0;
};
class MaskFilter : public AcceptsMask, public OutputsMask
{

};

typedef Matrix<short> ShortMatrix;
typedef Matrix<unsigned short> UnsignedShortMatrix;
typedef Matrix<uchar> UCharMatrix;
typedef Matrix<qreal> RealMatrix;
typedef Matrix<RealComplex> RealComplexMatrix;
typedef Matrix<float> FloatMatrix;
}

Q_DECLARE_TYPEINFO(QFlow::BoolMatrix, Q_MOVABLE_TYPE);
Q_DECLARE_METATYPE(QFlow::BoolMatrix)

Q_DECLARE_TYPEINFO(QFlow::ShortMatrix, Q_MOVABLE_TYPE);
Q_DECLARE_METATYPE(QFlow::ShortMatrix)


Q_DECLARE_TYPEINFO(QFlow::UnsignedShortMatrix, Q_MOVABLE_TYPE);
Q_DECLARE_METATYPE(QFlow::UnsignedShortMatrix)


Q_DECLARE_TYPEINFO(QFlow::UCharMatrix, Q_MOVABLE_TYPE);
Q_DECLARE_METATYPE(QFlow::UCharMatrix)


Q_DECLARE_TYPEINFO(QFlow::RealMatrix, Q_MOVABLE_TYPE);
Q_DECLARE_METATYPE(QFlow::RealMatrix)


Q_DECLARE_TYPEINFO(QFlow::RealComplexMatrix, Q_MOVABLE_TYPE);
Q_DECLARE_METATYPE(QFlow::RealComplexMatrix)


Q_DECLARE_TYPEINFO(QFlow::FloatMatrix, Q_MOVABLE_TYPE);
Q_DECLARE_METATYPE(QFlow::FloatMatrix)

#endif // MATRIX_H
