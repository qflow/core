#ifndef NOTIMPLEMENTEDEXCEPTION_H
#define NOTIMPLEMENTEDEXCEPTION_H

#include "coreplugin_global.h"
#include <QException>

namespace QFlow{

class COREPLUGIN_EXPORT NotImplementedException : public QException
{
public:
    void raise() const;
    QException *clone() const;
};
}
#endif // NOTIMPLEMENTEDEXCEPTION_H
