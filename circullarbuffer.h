#ifndef CIRCULLARBUFFER_H
#define CIRCULLARBUFFER_H

#include "timestamped.h"
#include <QList>

template<typename T>
class CircullarBuffer
{
private:
    QList<T> _data;
    int _head;
    int _capacity;
    int _size;
public:
    CircullarBuffer(int capacity) : _head(0), _capacity(capacity), _size(0)
    {
    }
    CircullarBuffer() : _head(0), _capacity(0), _size(0)
    {
    }

    void setCapacity(int capacity)
    {
        _data.clear();
        _head = 0;
        _size = 0;
        _capacity = capacity;
    }

    int capacity()
    {
        return _capacity;
    }

    void set_at(int i, T value)
    {
        _data[i] = value;
    }
    T head()
    {
        return _data[_head - 1];
    }

    void push_back(T value)
    {
        if(_head == _capacity) _head = 0;
        if(_head < _data.size()) _data[_head] = value;
        else _data.push_back(value);
        _head++;
        _size++;
    }
    T & operator[](int i)
    {
        int j = i;
        if(j>=_capacity) j = j % _capacity;
        return _data[j];
    }
    T& at(int i)
    {
        return this->operator [](i);
    }

    const T & operator[](int i) const
    {
        int j = i;
        if(j>=_capacity) j = j % _capacity;
        return _data[j];
    }

    int size()
    {
        return _size;
    }
    bool isEmpty()
    {
        if(_size == 0) return true;
        return false;
    }
};

#endif // CIRCULLARBUFFER_H
