#include "binarybuffer.h"
#include <QWaitCondition>
#include <atomic>
#include <QMutex>
#include <QDebug>

namespace QFlow{

enum class State {
        EMPTY,
        READY_READ,
        WRITING,
        READING
    };

class BinaryBufferPrivate
{
public:
    BinaryBufferPrivate() : _capacity(20 * 1024), _state(State::EMPTY)
    {
        _writeBuffer.reserve(_capacity);
        _readBuffer.reserve(_capacity);
    }
    ~BinaryBufferPrivate()
    {
        QMutexLocker lock(&_mutex);
        _dataAvailableCondition.wakeAll();
    }
    int _capacity;
    QByteArray _writeBuffer;
    QByteArray _readBuffer;
    QWaitCondition _dataAvailableCondition;
    QMutex _mutex;
    std::atomic<State> _state;
};

BinaryBuffer::BinaryBuffer(QObject* parent) : QIODevice(parent), d_ptr(new BinaryBufferPrivate())
{
}
BinaryBuffer::~BinaryBuffer()
{

}
void BinaryBuffer::close()
{
    Q_D(BinaryBuffer);
    QIODevice::close();
    d->_writeBuffer.clear();
    d->_readBuffer.clear();
    d->_dataAvailableCondition.wakeAll();
}

qint64 BinaryBuffer::bytesAvailable() const
{
    Q_D(const BinaryBuffer);
    if(d->_state.load() == State::READY_READ)
    {
        return d->_readBuffer.length();
    }
    return 0;
}
bool BinaryBuffer::isSequential() const
{
    return true;
}
bool BinaryBuffer::waitForReadyRead(int msecs)
{
    Q_D(BinaryBuffer);
    if(bytesAvailable() > 0) return true;
    else
    {
        QMutexLocker lock(&d->_mutex);
        if(msecs == -1)
        {
            bool res = d->_dataAvailableCondition.wait(&d->_mutex);
            return res;
        }
        else return d->_dataAvailableCondition.wait(&d->_mutex, msecs);
    }
}

qint64 BinaryBuffer::writeData(const char *data, qint64 maxSize)
{
    Q_D(BinaryBuffer);
    d->_writeBuffer.append(data, maxSize);
    Q_EMIT bytesWritten(maxSize);
    State expected = State::EMPTY;
    if(d->_state.compare_exchange_strong(expected, State::WRITING))
    {
        d->_readBuffer.append(d->_writeBuffer);
        d->_writeBuffer.clear();
        d->_state = State::READY_READ;
        d->_dataAvailableCondition.wakeAll();
        Q_EMIT readyRead();
    }
    return maxSize;
}
qint64 BinaryBuffer::readData(char *data, qint64 maxSize)
{
    Q_D(BinaryBuffer);
    State expected = State::READY_READ;
    if(d->_state.compare_exchange_strong(expected, State::READING))
    {
        int bytesCount = d->_readBuffer.length();
        if(maxSize < bytesCount) bytesCount = maxSize;
        memcpy(data, d->_readBuffer.data(), bytesCount);
        d->_readBuffer.remove(0, bytesCount);
        if(d->_readBuffer.isEmpty()) d->_state = State::EMPTY;
        else d->_state = State::READY_READ;
        return bytesCount;
    }
    return 0;
}
}
