#ifndef FUNCTOR_H
#define FUNCTOR_H

#include "coreplugin_global.h"
#include <QVariant>
#include <functional>


namespace QFlow{


class FunctorBase
{
public:
    virtual QVariant call(QVariantList args) = 0;
protected:
    template<typename T>
    T advance(QVariantList::iterator* it)
    {
        it->operator --();
        QVariant var = it->operator *();
        return qvariant_cast<T>(var);
    }
};

template<typename R, typename ... Args>
class Functor : public FunctorBase
{
public:
    Functor(std::function<R(Args...)> func) : f(func)
    {

    }
    QVariant call(QVariantList args)
    {
        QVariantList::iterator it = args.end();
        R res = f(advance<Args>(&it) ... );
        return QVariant::fromValue(res);
    }

private:
    std::function<R(Args...)> f;
};

template<typename ... Args>
class Functor<void, Args...> : public FunctorBase
{
public:
    Functor(std::function<void(Args...)> func) : f(func)
    {

    }
    QVariant call(QVariantList args)
    {
        QVariantList::iterator it = args.end();
        f(advance<Args>(&it) ... );
        return QVariant();
    }

private:
    std::function<void(Args...)> f;
};
}
#endif // FUNCTOR_H

