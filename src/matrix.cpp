#include "matrix.h"
#include "matrix_p.h"

/*template<typename T>
Data<T>::Data() : _array(NULL)
{

}

template<typename T>
Data<T>::Data(int size)
{
    _array = new T[size];
}
template<typename T>
Data<T>::~Data()
{
    delete _array;
}
template<typename T>
bool Data<T>::isNull() const
{
    return !_array;
}
template<typename T>
T* Data<T>::array()
{
    return _array;
}*/

namespace QFlow{

template<typename T>
MatrixPrivate<T>::MatrixPrivate() : QSharedData(), _columns(0), _rows(0), _start(0), _order(RowMajor)
{

}
template<typename T>
int MatrixPrivate<T>::dataElements() const
{
    int ret = (_rows*_columns) - (_rows-1)*(_columns-_dist);
    return ret;
}

template<typename T>
MatrixPrivate<T>::MatrixPrivate(int columns, int rows, int dist) : _columns(columns), _rows(rows), _dist(dist), _start(0), _order(RowMajor)
{
    if(_dist == AUTO_DIST) _dist = columns;
    int length = dataElements();
    _data.reset(new T[length]);
}
template<typename T>
MatrixPrivate<T>::MatrixPrivate(const MatrixPrivate<T> &other) : QSharedData(other),
    _columns(other._columns), _rows(other._rows), _dist(other._dist), _start(0), _order(other._order)
{
    int el = dataElements();
    _data.reset(new T[el]);
    memcpy(_data.data(), other._data.data() + sizeof(T)*other._start, sizeof(T)*el);
}
template<typename T>
MatrixPrivate<T>::~MatrixPrivate()
{
}

template<typename T>
Matrix<T>::Matrix() : d(new MatrixPrivate<T>())
{

}
template<typename T>
Matrix<T>::Matrix( int columns, int rows, int dist) : d(new MatrixPrivate<T>(columns, rows, dist))
{

}
template<typename T>
Matrix<T>::Matrix(QSize size, int dist) : d(new MatrixPrivate<T>(size.width(), size.height(), dist))
{

}

template<typename T>
void Matrix<T>::init(int columns, int rows, int dist)
{
    d->_columns = columns;
    d->_rows = rows;
    d->_dist = dist;
    if(d->_dist == AUTO_DIST) d->_dist = columns;
    d->_order = MatrixPrivate<T>::RowMajor;
    d->_data.reset(new T[d->dataElements()]);
}
template<typename T>
void Matrix<T>::init(QSize size, int dist)
{
    init(size.width(), size.height(), dist);
}
template<typename T>
QSize Matrix<T>::size() const
{
    return QSize(d->_columns, d->_rows);
}
template<typename T>
Matrix<T>::Matrix(const Matrix<T> &other) : d(other.d)
{

}
template<typename T>
Matrix<T>::~Matrix()
{

}

template<typename T>
bool Matrix<T>::operator!=(const Matrix<T> &other) const
{
    return !(other == *this);
}
template<typename T>
Matrix<T> &Matrix<T>::operator=(const Matrix<T> & other)
{
    if (this == &other)
    return *this;
    d = other.d;
    return *this;
}
template<typename T>
bool Matrix<T>::operator ==(const Matrix<T> &other) const
{
    return d->_data == other.d->_data;
}
template<typename T>
bool Matrix<T>::operator !() const
{
    return !d->_data;
}
template<typename T>
Matrix<T>::operator bool() const
{
    return d->_data;
}
template<typename T>
T* Matrix<T>::data()
{
    return d->_data.data() + d->_start;
}

template<typename T>
const T* Matrix<T>::data() const
{
    return d->_data.data() + d->_start;
}
template<typename T>
int Matrix<T>::columns() const
{
    return d->_columns;
}
template<typename T>
int Matrix<T>::rows() const
{
    return d->_rows;
}
template<typename T>
int Matrix<T>::dist() const
{
    return d->_dist;
}
template<typename T>
Matrix<T>::const_iterator::const_iterator(int index, MatrixPrivate<T>* matrix, IteratorDirection dir) :
    _index(index), _originalIndex(index), _matrix(matrix)
{
    if(dir == Matrix::Column) _increment = 1;
    else _increment = _matrix->_dist;
    _data = _matrix->_data.data();
}
template<typename T>
Matrix<T>::const_iterator::const_iterator(const const_iterator &other) :
    _increment(other._increment),
    _index(other._index), _originalIndex(other._index), _matrix(other._matrix),
    _data(other._data)
{

}
template<typename T>
void Matrix<T>::fill(T value)
{
    for(int i=0; i<d->dataElements(); i++)
    {
        d->_data.data()[i] = value;
    }
}
template<typename T>
void Matrix<T>::fill(const Matrix<T> &other)
{
    memcpy(d->_data.data(), other.d->_data.data(), sizeof(T)*d->dataElements());
}

template<typename T>
typename Matrix<T>::const_iterator Matrix<T>::const_iterator::row() const
{
    return const_iterator(_index, _matrix, Matrix::Row);
}
template<typename T>
typename Matrix<T>::const_iterator Matrix<T>::const_iterator::column() const
{
    return const_iterator(_index, _matrix, Matrix::Column);
}
template<typename T>
typename Matrix<T>::const_iterator Matrix<T>::const_iterator::operator -(int dec)
{
    Matrix<T>::const_iterator i(_index - dec*_increment, _matrix);
    i._increment = _increment;
    return i;
}
template<typename T>
bool Matrix<T>::const_iterator::operator!=( const const_iterator& other) const
{
    return _index != other._index;
}
template<typename T>
typename Matrix<T>::const_iterator& Matrix<T>::const_iterator::toNextRow()
{
    _index += _matrix->_dist;
    return *this;
}
template<typename T>
typename Matrix<T>::const_iterator& Matrix<T>::const_iterator::reset()
{
    _index = _originalIndex;
    return *this;
}





template<typename T>
Matrix<T>::iterator::iterator(int index, MatrixPrivate<T>* matrix, IteratorDirection dir) :
    _index(index), _matrix(matrix)
{
    if(dir == Matrix::Column) _increment = 1;
    else _increment = _matrix->_dist;
    _data = _matrix->_data.data();
}
template<typename T>
bool Matrix<T>::iterator::operator!=( const iterator& other) const
{
    return _index != other._index;
}
template<typename T>
Matrix<T>::iterator::iterator(const iterator &other) : _increment(other._increment), _index(other._index),
    _matrix(other._matrix), _data(other._data)
{

}
template<typename T>
typename Matrix<T>::iterator& Matrix<T>::iterator::toNextRow()
{
    _index += _matrix->_dist;
    return *this;
}
template<typename T>
typename Matrix<T>::iterator& Matrix<T>::iterator::moveBy(int increment)
{
    _index += increment;
    return *this;
}
template<typename T>
typename Matrix<T>::const_iterator& Matrix<T>::const_iterator::moveBy(int increment)
{
    _index += increment;
    return *this;
}
template<typename T>
typename Matrix<T>::iterator& Matrix<T>::iterator::moveTo(int index)
{
    _index = index;
    return *this;
}

template<typename T>
typename Matrix<T>::iterator Matrix<T>::iterator::row() const
{
    return iterator(_index, _matrix, Matrix::Row);
}
template<typename T>
typename Matrix<T>::iterator Matrix<T>::iterator::column() const
{
    return iterator(_index, _matrix, Matrix::Column);
}


template<typename T>
typename Matrix<T>::const_iterator Matrix<T>::begin(IteratorDirection dir) const
{
    return const_iterator (0, d.data(), dir);
}
template<typename T>
typename Matrix<T>::const_iterator Matrix<T>::end(IteratorDirection dir) const
{
    return const_iterator(d->dataElements(), d.data(), dir);
}
template<typename T>
typename Matrix<T>::iterator Matrix<T>::begin(IteratorDirection dir)
{
    return iterator(d->_start, d.data(), dir);
}
template<typename T>
typename Matrix<T>::iterator Matrix<T>::end(IteratorDirection dir)
{
    return iterator(d->dataElements(), d.data(), dir);
}
template<typename T>
typename Matrix<T>::const_iterator Matrix<T>::at(int column, int row, IteratorDirection dir) const
{
    int index;
    if(d->_order == MatrixPrivate<T>::RowMajor)
    {
        index = d->_start + (column) + (row)*d->_dist;
    }
    else
    {
        index = (row) + (column)*d->_rows;
    }
    return const_iterator(index, d.data(), dir);
}
template<typename T>
typename Matrix<T>::iterator Matrix<T>::at(int column, int row, IteratorDirection dir)
{
    int index = d->_start + (row)*d->_dist + (column);
    return iterator(index, d.data(), dir);
}
template<typename T>
Matrix<T> Matrix<T>::transposed() const
{
    Matrix newMatrix = copy();
    newMatrix.d->_order = MatrixPrivate<T>::ColumnMajor;
    return newMatrix;
}

template<typename T>
Matrix<T> Matrix<T>::copy() const
{
    Matrix newMatrix = *this;
    newMatrix.d.detach();
    return newMatrix;
}
template<typename T>
int Matrix<T>::elements() const
{
    return d->_columns * d->_rows;
}

template<typename T>
typename Matrix<T>::iterator Matrix<T>::iterator::left() const
{
    return iterator(_index - 1, _matrix);
}
template<typename T>
typename Matrix<T>::iterator Matrix<T>::iterator::topLeft() const
{
    return iterator(_index - _matrix->_dist - 1, _matrix);
}
template<typename T>
typename Matrix<T>::iterator Matrix<T>::iterator::topRight() const
{
    return iterator(_index + _matrix->_dist + 1, _matrix);
}
template<typename T>
typename Matrix<T>::iterator Matrix<T>::iterator::right() const
{
    return iterator(_index + 1, _matrix);
}
template<typename T>
typename Matrix<T>::iterator Matrix<T>::iterator::top() const
{
    return iterator(_index - _matrix->_dist, _matrix);
}
template<typename T>
typename Matrix<T>::iterator Matrix<T>::iterator::bottom() const
{
    return iterator(_index + _matrix->_dist, _matrix);
}
template<typename T>
typename Matrix<T>::iterator Matrix<T>::iterator::bottomLeft() const
{
    return iterator(_index + _matrix->_dist - 1, _matrix);
}
template<typename T>
typename Matrix<T>::iterator Matrix<T>::iterator::bottomRight() const
{
    return iterator(_index + _matrix->_dist + 1, _matrix);
}


template<typename T>
typename Matrix<T>::const_iterator Matrix<T>::const_iterator::left() const
{
    return const_iterator(_index - 1, _matrix);
}
template<typename T>
typename Matrix<T>::const_iterator Matrix<T>::const_iterator::topLeft() const
{
    return const_iterator(_index - _matrix->_dist - 1, _matrix);
}
template<typename T>
typename Matrix<T>::const_iterator Matrix<T>::const_iterator::topRight() const
{
    return const_iterator(_index + _matrix->_dist + 1, _matrix);
}
template<typename T>
typename Matrix<T>::const_iterator Matrix<T>::const_iterator::right() const
{
    return const_iterator(_index + 1, _matrix);
}
template<typename T>
typename Matrix<T>::const_iterator Matrix<T>::const_iterator::top() const
{
    return const_iterator(_index - _matrix->_dist, _matrix);
}
template<typename T>
typename Matrix<T>::const_iterator Matrix<T>::const_iterator::bottom() const
{
    return const_iterator(_index + _matrix->_dist, _matrix);
}
template<typename T>
typename Matrix<T>::const_iterator Matrix<T>::const_iterator::bottomLeft() const
{
    return const_iterator(_index + _matrix->_dist - 1, _matrix);
}
template<typename T>
typename Matrix<T>::const_iterator Matrix<T>::const_iterator::bottomRight() const
{
    return const_iterator(_index + _matrix->_dist + 1, _matrix);
}
template<typename T>
const Matrix<T> Matrix<T>::rows(int start, int howmany) const
{
    Matrix<T> mat;
    mat.d->_data = d->_data;
    mat.d->_columns = d->_columns;
    mat.d->_dist = d->_dist;
    mat.d->_start = start*d->_dist;
    mat.d->_rows = howmany;
    return mat;
}
template<typename T>
int Matrix<T>::dataElements() const
{
    return d->dataElements();
}

template class Matrix<short>;
template class Matrix<bool>;
template class Matrix<unsigned short>;
template class Matrix<uchar>;
template class Matrix<qreal>;
template class Matrix<RealComplex>;
template class Matrix<float>;
template class Matrix<FloatComplex>;
}
