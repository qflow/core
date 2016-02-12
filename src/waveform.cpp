#include "waveform.h"
#include <QSharedData>

namespace QFlow{

class WaveformPrivate : public QSharedData
{
public:
    SampleBufferPointer _buffer;
    int _length;
    int _pointer;
    WaveformPrivate() : QSharedData(), _length(0), _pointer(0)
    {

    }
    WaveformPrivate(int length, int pointer, SampleBufferPointer buffer) : _buffer(buffer), _length(length), _pointer(pointer)
    {

    }

    WaveformPrivate(const WaveformPrivate &other) : QSharedData(other)
    {

    }

    ~WaveformPrivate()
    {

    }
};

Waveform::Waveform(int length, int pointer, SampleBufferPointer buffer) : d(new WaveformPrivate(length, pointer, buffer))
{
}
Waveform::Waveform() : d(new WaveformPrivate())
{

}
Waveform::~Waveform()
{

}
TimestampRange Waveform::timeScale()
{
    return TimestampRange(startTimestamp(), endTimestamp());
}
Waveform Waveform::range(Timestamp start, Timestamp end)
{
    qreal per = periodMicrosec();
    qreal startIndex = (qreal)start / per;
    qreal endIndex = (qreal)end / per;
    qreal length = endIndex - startIndex;
    int lenInt = length + 0.5;
    int startInt = startIndex + 0.5;
    return Waveform(lenInt, startInt, d->_buffer);
}

Waveform::Waveform(const Waveform &b) : d(b.d)
{
}
Waveform Waveform::clone()
{
    SampleBufferPointer copy(new SampleBuffer(d->_length));
    for(int i=0;i<d->_length;i++)
    {
        copy->push_back(at(i));
    }
    return Waveform(d->_length,0,copy);
}
qreal Waveform::periodMicrosec()
{
    qreal period = periodSec() * 1E6;
    return period;
}
qreal Waveform::periodSec()
{
    qreal period = (qreal)(1/sampleRate());
    return period;
}
qreal Waveform::sampleRate()
{
    qreal result = (qreal)(d->_length*1E6)/(qreal)(endTimestamp() - startTimestamp());
    return result;
}

void Waveform::minmax(int* minIndex, int* maxIndex)
{
    qreal min = at(0).value();
    *minIndex = 0;
    qreal max = at(0).value();
    *maxIndex = 0;
    for(int i=0;i<d->_length;i++)
    {
        if(at(i).value() < min)
        {
            min = at(i).value();
            *minIndex = i;
        }
        if(at(i).value() > max)
        {
            max = at(i).value();
            *maxIndex = i;
        }
    }
}
Waveform Waveform::range(int start, int length)
{
    return Waveform(length, start, d->_buffer);
}

Timestamp Waveform::startTimestamp()
{
    return at(0).timestamp();
}
Timestamp Waveform::endTimestamp()
{
    return at(d->_length - 1).timestamp();
}
int Waveform::length()
{
    return d->_length;
}
Sample Waveform::at(int i)
{
    return d->_buffer->at(d->_pointer + i);
}
Sample Waveform::atTimestamp(Timestamp t)
{
    qreal per = periodMicrosec();
    qreal index = t / per;
    int indexInt = index + 0.5;
    return at(indexInt);
}

void Waveform::setAt(int i, Sample value)
{
    d->_buffer->set_at(i, value);
}
void Waveform::setValueAt(int i, qreal value)
{
    setAt(i, Sample(value, at(i).timestamp()));
}
Waveform &Waveform::operator=(const Waveform & other)
{
    if (this == &other)
    return *this;
    d = other.d;
    return *this;
}
}
