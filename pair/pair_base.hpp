
#ifndef PAIR_BASE_HPP
# define PAIR_BASE_HPP

# include <algorithm>
# include "defs.hpp"
# include "type_traits.hpp"

TLU_NAMESPACE_BEGIN

// ====================================================================
template <class type_T, class type_Y>
struct pair_base
{
// --------------------------------------- typedefs ---------------------------
    typedef type_T  first_type;
    typedef type_Y  second_type;

// ------------------------------------- public members -------------------------
    first_type  first;
    second_type second;

// ------------------------------------- constructors -------------------------
    constexpr pair_base() : first(), second() {}

    constexpr pair_base(const first_type &_first, const second_type &_second) : first(_first), second(_second) {}

    template <class type_U1, class type_U2>
    constexpr explicit pair_base(const pair_base<type_U1, type_U2> &cpy) : first(cpy.first), second(cpy.second) {}

    template <class type_U1, class type_U2>
    constexpr pair_base(const std::pair<type_U1, type_U2> &cpy) : first(cpy.first), second(cpy.second) {}

    constexpr pair_base(const pair_base &cpy) : first(cpy.first), second(cpy.second) {}

#if CPP11
    template <class type_U1, class type_U2>
    constexpr pair_base(type_U1 &&_first, type_U2 &&_second) : first(std::forward<type_U1>(_first)), second(std::forward<type_U2>(_second)) {}

    template<class U1, class U2>
    constexpr explicit pair_base(pair_base<U1, U2> &&mv) : first(std::move(mv.first)), second(std::move(mv.second)) {}

    constexpr pair_base(const pair_base &&mv) : first(std::move(mv.first)), second(std::move(mv.second)) {}
#endif /* CPP11 */

// ------------------------------------- destructors -------------------------
    ~pair_base() DEFAULT

// ------------------------------------- opearator = -------------------------
    constexpr pair_base &operator =(const pair_base &other)
    {
        first = other.first;
        second = other.second;
        return *this;
    }

    template <class U1, class U2>
    constexpr pair_base &operator =(const pair_base<U1, U2> &other)
    {
        first = other.first;
        second = other.second;
        return *this;
    }

// ------------------------------------- swap -------------------------
    constexpr void swap(pair_base &other) noexcept
    {
        std::swap(first, other.first);
        std::swap(second, other, second);
    }
}; /* pair_base */

// ====================================================================
// ------------------------- non-member functions ---------------------
template <class type_T, class type_Y>
constexpr pair_base<type_T, type_Y> make_pair(type_T first, type_Y second)
{
    return pair_base<type_T, type_Y>(first, second);
}

// -------------------------------- operators ---------------------------------
TLU_NAMESPACE_HIDDEN_BEGIN

template <class type_T, class type_Y>
int pair_base_compare(
        const TLU_NAMESPACE::pair_base<type_T, type_Y> &lhs,
        const TLU_NAMESPACE::pair_base<type_T, type_Y> &rhs
    )
{
    if (rhs.first < lhs.first)
        return 1;
    else if (lhs.first < rhs.first)
        return -1;
    else if (rhs.second < lhs.second)
        return 1;
    else if (lhs.second < rhs.second)
        return -1;
    return 0;
}

TLU_NAMESPACE_HIDDEN_END

template <class T, class Y, class type_T, class type_Y> struct change_pair_type<pair_base<T, Y>, type_T, type_Y> {
    typedef pair_base<type_T, type_Y> type;
};

TLU_NAMESPACE_END

template <class type_T, class type_Y>
bool operator ==(
        const TLU_NAMESPACE::pair_base<type_T, type_Y> &lhs,
        const TLU_NAMESPACE::pair_base<type_T, type_Y> &rhs
    )
{
    return lhs.first == rhs.first and lhs.second == rhs.second;
}

template <class type_T, class type_Y>
bool operator !=(
        const TLU_NAMESPACE::pair_base<type_T, type_Y> &lhs,
        const TLU_NAMESPACE::pair_base<type_T, type_Y> &rhs
    )
{
    return TLU_NAMESPACE::TLU_NAMESPACE_HIDDEN::pair_base_compare(lhs, rhs) != 0;
}

template <class type_T, class type_Y>
bool operator >(
        const TLU_NAMESPACE::pair_base<type_T, type_Y> &lhs,
        const TLU_NAMESPACE::pair_base<type_T, type_Y> &rhs
    )
{
    return TLU_NAMESPACE::TLU_NAMESPACE_HIDDEN::pair_base_compare(lhs, rhs) > 0;
}

template <class type_T, class type_Y>
bool operator >=(
        const TLU_NAMESPACE::pair_base<type_T, type_Y> &lhs,
        const TLU_NAMESPACE::pair_base<type_T, type_Y> &rhs
    )
{
    return TLU_NAMESPACE::TLU_NAMESPACE_HIDDEN::pair_base_compare(lhs, rhs) >= 0;
}

template <class type_T, class type_Y>
bool operator <(
        const TLU_NAMESPACE::pair_base<type_T, type_Y> &lhs,
        const TLU_NAMESPACE::pair_base<type_T, type_Y> &rhs
    )
{
    return TLU_NAMESPACE::TLU_NAMESPACE_HIDDEN::pair_base_compare(lhs, rhs) < 0;
}

template <class type_T, class type_Y>
bool operator <=(
        const TLU_NAMESPACE::pair_base<type_T, type_Y> &lhs,
        const TLU_NAMESPACE::pair_base<type_T, type_Y> &rhs
    )
{
    return TLU_NAMESPACE::TLU_NAMESPACE_HIDDEN::pair_base_compare(lhs, rhs) <= 0;
}

#endif /* PAIR_BASE_HPP */
