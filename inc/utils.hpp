
#ifndef UTILS_HPP
# define UTILS_HPP

# include "defs.h"

TLU_NAMESPACE_BEGIN

template <class type_T>
constexpr const type_T &min(const type_T &a, const type_T &b)
{
    return (b < a) ? b : a;
}

template <class type_T>
constexpr const type_T &max(const type_T &a, const type_T &b)
{
    return (a < b) ? b : a;
}

TLU_NAMESPACE_END

#endif /* UTILS_HPP */