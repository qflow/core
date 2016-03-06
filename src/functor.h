#ifndef FUNCTOR_H
#define FUNCTOR_H

#include "coreplugin_global.h"
#include <QVariant>
#include <functional>
#include <tuple>
#include <utility>
#include <typeinfo>
#include <typeindex>


namespace QFlow{


class FunctorBase
{
public:
    virtual QVariant call(QVariantList args) = 0;
    std::type_index argType(int i) const
    {
        return _argTypes[i];
    }
protected:
    template<typename T>
    T advance(QVariantList::iterator* it)
    {
        it->operator --();
        QVariant var = it->operator *();
        return qvariant_cast<T>(var);
    }
    QList<std::type_index> _argTypes;
    std::size_t _argCount;
};

template<typename R, typename ... ArgTypes>
class Functor : public FunctorBase
{
public:
    Functor(std::function<R(ArgTypes...)> func) : f(func)
    {
        _argTypes = QList<std::type_index>(std::type_index(typeid(ArgTypes)) ... );
        _argCount = sizeof...(ArgTypes);
    }
    QVariant call(QVariantList args)
    {
        QVariantList::iterator it = args.end();
        R res = f(advance<ArgTypes>(&it) ... );
        return QVariant::fromValue(res);
    }
    R call2(ArgTypes... args)
    {
        return f(std::forward<ArgTypes>(args)...);
    }
    R call3(std::tuple<ArgTypes...> tup)
    {

    }

private:
    std::function<R(ArgTypes...)> f;
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

