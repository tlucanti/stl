
#ifndef UTILS_HPP
# define UTILS_HPP

# include "defs.hpp"
# include "type_traits.hpp"

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

template <class type_T>
constexpr int sign(const type_T &val) {
    return (type_T() < val) - (val < type_T());
}

TLU_NAMESPACE_END

#endif /* UTILS_HPP */
