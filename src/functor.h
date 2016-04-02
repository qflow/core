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
#include <array>
#include <experimental/any>


namespace QFlow{

template<typename R, typename ... ArgTypes>
class FunctorImpl2;

class FunctorBase
{
public:
    virtual QVariant invoke(QVariantList args) = 0;
    virtual std::type_index argType(int i) const = 0;
    virtual std::type_index returnType() const = 0;
    virtual std::size_t argCount() const = 0;
    virtual std::experimental::any invokeAny(std::vector<std::experimental::any> args);
};

template<typename R, typename ... ArgTypes>
class FunctorImpl2 : public FunctorBase
{
public:
    FunctorImpl2(std::function<R(ArgTypes...)> func) : f(func)
    {
    }
    std::type_index returnType() const
    {
        return std::type_index(typeid(R));
    }
    std::size_t argCount() const
    {
        return sizeof...(ArgTypes);
    }
    std::type_index argType(int i) const
    {
        return _argTypes[i];
    }
    R invoke2(ArgTypes... args)
    {
        return f(std::forward<ArgTypes>(args)...);
    }
    R invoke3(std::tuple<ArgTypes...> tup)
    {
        return invoke_helper(std::forward<std::tuple<ArgTypes...>>(tup),
                             std::make_index_sequence<sizeof...(ArgTypes)>{});
    }
protected:
    std::function<R(ArgTypes...)> f;
    const std::array<std::type_index, sizeof...(ArgTypes)> _argTypes = {{std::type_index(typeid(ArgTypes)) ...}};
    template<typename Tup, std::size_t... index>
    R invoke_helper(Tup&& tup, std::index_sequence<index...>)
    {
        return f(std::get<index>(std::forward<Tup>(tup))...);
    }
    template<std::size_t... index>
    R invoke_helper(QVariantList& list, std::index_sequence<index...>)
    {
        return f(qvariant_cast<ArgTypes>(list[index])...);
    }
};
template<typename T>
T advance(QVariantList::iterator* it)
{
    it->operator --();
    QVariant var = it->operator *();
    return qvariant_cast<T>(var);
}
template<typename T>
T advance(std::vector<std::experimental::any>::iterator* it)
{
    it->operator --();
    std::experimental::any var = it->operator *();
    return std::experimental::any_cast<T>(var);
}
template<typename R, typename ... ArgTypes>
class Functor : public FunctorImpl2<R, ArgTypes...>
{
public:
    Functor(std::function<R(ArgTypes...)> func) : FunctorImpl2<R, ArgTypes...>(func)
    {
    }

    QVariant invoke(QVariantList args)
    {
        QVariantList::iterator it = args.end();
        R res = this->f(std::forward<ArgTypes>(advance<ArgTypes>(&it)) ... );
        return QVariant::fromValue(res);
    }
    QVariant invokeAny(std::vector<std::experimental::any> args)
    {
        std::vector<std::experimental::any>::iterator it = args.end();
        R res = this->f(std::forward<ArgTypes>(advance<ArgTypes>(&it)) ... );
        return std::experimental::any(res);
    }
};
template<typename ... ArgTypes>
class Functor<void, ArgTypes...> : public FunctorImpl2<void, ArgTypes...>
{
public:
    Functor(std::function<void(ArgTypes...)> func) : FunctorImpl2<void, ArgTypes...>(func)
    {
    }
    QVariant invoke(QVariantList args)
    {
        QVariantList::iterator it = args.end();
        this->f(std::forward<ArgTypes>(advance<ArgTypes>(&it)) ... );
        return QVariant();
    }
};
template<typename R, typename ... ArgTypes>
Functor<R,ArgTypes...> make_functor(std::function<R(ArgTypes...)> func)
{
    return Functor<R,ArgTypes...>(func);
}
}
#endif // FUNCTOR_H

