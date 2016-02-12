#ifndef MATRIX_P_H
#define MATRIX_P_H

#include <QSharedData>
#include <QVector>
#include <QSharedPointer>

namespace QFlow{

template <typename T>
class MatrixPrivate : public QSharedData
{
public:
    enum Order{RowMajor = 0, ColumnMajor = 1};
    MatrixPrivate();
    MatrixPrivate(int columns, int rows, int stride);
    MatrixPrivate(const MatrixPrivate<T> &other);
    ~MatrixPrivate();
    int dataElements() const;

    QSharedPointer<T> _data;
    int _columns;
    int _rows;
    int _dist;
    int _start;
    Order _order;
};
}
#endif // MATRIX_P_H
