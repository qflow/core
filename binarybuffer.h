#ifndef BINARYBUFFER_H
#define BINARYBUFFER_H

#include "coreplugin_global.h"
#include <QIODevice>

namespace QFlow{

class BinaryBufferPrivate;
class COREPLUGIN_EXPORT BinaryBuffer : public QIODevice
{
public:
    BinaryBuffer(QObject* parent = NULL);
    ~BinaryBuffer();
    qint64 bytesAvailable() const Q_DECL_OVERRIDE;
    bool waitForReadyRead(int msecs) Q_DECL_OVERRIDE;
    bool isSequential() const Q_DECL_OVERRIDE;
    void close() Q_DECL_OVERRIDE;
protected:
    qint64 readData(char * data, qint64 maxSize);
    qint64 writeData(const char * data, qint64 maxSize);
private:
    const QScopedPointer<BinaryBufferPrivate> d_ptr;
    Q_DECLARE_PRIVATE(BinaryBuffer)
};
}
#endif // BINARYBUFFER_H
