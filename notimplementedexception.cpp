#include "notimplementedexception.h"

namespace QFlow{

void NotImplementedException::raise() const
{
    throw *this;
}

QException *NotImplementedException::clone() const
{
    return new NotImplementedException(*this);
}
}
