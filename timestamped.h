#ifndef TIMESTAMPED_H
#define TIMESTAMPED_H

#include <QMetaType>

namespace QFlow{

typedef quint64 Timestamp; //microseconds


template<typename T>
class Timestamped
{
    T _value;
    Timestamp _timestamp;
public:
    Timestamped(T value, Timestamp timestamp) : _value(value), _timestamp(timestamp)
    {
    }
    Timestamped() : _timestamp(0)
    {
    }
    ~Timestamped()
    {

    }

    T value()
    {
        return _value;
    }
    T value() const
    {
        return _value;
    }
    Timestamp timestamp()
    {
        return _timestamp;
    }
    Timestamp timestamp() const
    {
        return _timestamp;
    }
    void setTimestamp(Timestamp t)
    {
        _timestamp = t;
    }
    void setValue(T value)
    {
        _value = value;
    }

    Timestamped(const Timestamped<T> &b)
    {
        _value = b._value;
        _timestamp = b._timestamp;
    }
    bool operator<(const Timestamped<T> &b)
    {
        return _timestamp < b._timestamp;
    }
};
typedef Timestamped<qreal> Sample;

class OutputsTimestamp
{
public:
    virtual void tick(Timestamp t) = 0;
};
class AcceptsTimestamp
{
public:
    virtual void onTick(Timestamp t) = 0;
};
class OutputsSample
{
public:
    virtual void value(Sample val) = 0;
};
class AcceptsSample
{
public:
    virtual void onValue(Sample val) = 0;
};
class SampleFilter: public AcceptsSample, public OutputsSample
{

};
}
Q_DECLARE_METATYPE(QFlow::Sample)
#endif // TIMESTAMPED_H
