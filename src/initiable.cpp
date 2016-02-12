#include "initiable.h"

namespace QFlow{

void QmlInitiable::classBegin()
{

}
void QmlInitiable::componentComplete()
{
    init();
}
ErrorInfo QmlInitiable::deinit()
{
    return ErrorInfo();
}
}
