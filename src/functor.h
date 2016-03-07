#ifndef FUNCTOR_H
#define FUNCTOR_H

#include "coreplugin_global.h"
#include <QVariant>
#include <functional>
#include <tuple>
#include <utility>
#include <typeinfo>
#include <typeindex>
#include <type_traits>


namespace QFlow{


class FunctorBase
{
public:
    virtual QVariant call(QVariantList args) = 0;
    virtual std::type_index argType(int i) const = 0;
    virtual std::type_index returnType() const = 0;
    virtual std::size_t argCount() const = 0;
protected:
    template<typename T>
    T advance(QVariantList::iterator* it)
    {
        it->operator --();
        QVariant var = it->operator *();
        return qvariant_cast<T>(var);
    }
};

template<typename R, typename ... ArgTypes>
class Functor : public FunctorBase
{
public:
    Functor(std::function<R(ArgTypes...)> func) : f(func)
    {
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
    std::type_index returnType() const
    {
        return std::type_index(typeid(R));
    }
    std::size_t argCount() const
    {
        return sizeof...(ArgTypes);
    }
    const std::type_index argType(int i) const
    {
        return _argTypes[i];
    }
private:
    std::function<R(ArgTypes...)> f;
    const QList<std::type_index> _argTypes = {std::type_index(typeid(ArgTypes)) ...};
};

template<typename ... ArgTypes>
class Functor<void, ArgTypes...> : public FunctorBase
{
public:
    Functor(std::function<void(ArgTypes...)> func) : f(func)
    {
    }
    QVariant call(QVariantList args)
    {
        QVariantList::iterator it = args.end();
        f(advance<ArgTypes>(&it) ... );
        return QVariant();
    }
    void call3(std::tuple<ArgTypes...> tup)
    {
        return invoke_helper(std::forward<std::tuple<ArgTypes...>>(tup),
                             std::make_index_sequence<sizeof...(ArgTypes)>{});
    }
    void call2(ArgTypes... args)
    {
        f(std::forward<ArgTypes>(args)...);
    }
    std::type_index returnType() const
    {
        return std::type_index(typeid(void));
    }
    std::size_t argCount() const
    {
        return sizeof...(ArgTypes);
    }
    std::type_index argType(int i) const
    {
        return _argTypes[i];
    }
private:
    template<typename Tup, std::size_t... index>
    void invoke_helper(Tup&& tup, std::index_sequence<index...>)
    {
        return f(std::get<index>(std::forward<Tup>(tup))...);
    }
    std::function<void(ArgTypes...)> f;
    const QList<std::type_index> _argTypes = {std::type_index(typeid(ArgTypes)) ...};
};
}
#endif // FUNCTOR_H

