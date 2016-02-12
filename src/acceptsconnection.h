#ifndef ACCEPTSCONNECTION_H
#define ACCEPTSCONNECTION_H

#include "connection.h"

namespace QFlow{

class AcceptsConnection
{
public:
    virtual void acceptConnection(Connection* con) = 0;
};
}
#endif // ACCEPTSCONNECTION_H
