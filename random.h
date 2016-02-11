#ifndef RANDOM_H
#define RANDOM_H

#include "coreplugin_global.h"
#include <QObject>

namespace QFlow{

class RandomPrivate;
class COREPLUGIN_EXPORT Random
{
public:
    Random();
    static qulonglong generate();
private:
    const QScopedPointer<RandomPrivate> d_ptr;
    Q_DECLARE_PRIVATE(Random)
};
}
#endif // RANDOM_H
