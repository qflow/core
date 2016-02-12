#ifndef WAVEFORM_H
#define WAVEFORM_H

#include "coreplugin_global.h"
#include "circullarbuffer.h"
#include "range.h"
#include <QSharedPointer>

namespace QFlow{

typedef CircullarBuffer<Sample> SampleBuffer;
typedef QSharedPointer<SampleBuffer> SampleBufferPointer;

class WaveformPrivate;
class COREPLUGIN_EXPORT Waveform
{
public:
    Waveform(int length, int pointer, SampleBufferPointer buffer);
    Waveform();
    ~Waveform();
    TimestampRange timeScale();
    Waveform range(Timestamp start, Timestamp end);
    Waveform range(int start, int length);
    Timestamp startTimestamp();
    Timestamp endTimestamp();
    int length();
    Sample at(int i);
    Sample atTimestamp(Timestamp t);
    void setAt(int i, Sample value);
    void setValueAt(int i, qreal value);
    Waveform(const Waveform &b);
    Waveform clone();
    qreal periodSec();
    qreal periodMicrosec();
    qreal sampleRate();
    void minmax(int* minIndex, int* maxIndex);
    Waveform &operator=(const Waveform &other);
private:
    QExplicitlySharedDataPointer<WaveformPrivate> d;
};

class OutputsWaveform
{
public:
    virtual void wav(Waveform w) = 0;
};
class AcceptsWaveform
{
public:
    virtual void onWav(Waveform w) = 0;
};
}
Q_DECLARE_TYPEINFO(QFlow::Waveform, Q_MOVABLE_TYPE);
Q_DECLARE_METATYPE(QFlow::Waveform)
#endif // WAVEFORM_H
