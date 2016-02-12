#include "random.h"
#include <random>
#include <QDateTime>

namespace QFlow{

class RandomPrivate
{
public:
    RandomPrivate()
    {

    }
    ~RandomPrivate()
    {

    }
    static std::default_random_engine e;
    static std::uniform_int_distribution<qulonglong> d;
    //const static bool initialized{false};
};
std::default_random_engine RandomPrivate::e{(unsigned int)QDateTime::currentMSecsSinceEpoch()};
std::uniform_int_distribution<qulonglong> RandomPrivate::d{(qulonglong)1, (qulonglong)1E8};
Random::Random() : d_ptr(new RandomPrivate())
{

}

qulonglong Random::generate()
{
    return RandomPrivate::d(RandomPrivate::e);
}
}
