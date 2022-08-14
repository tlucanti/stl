/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   stack_base.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tlucanti <tlucanti@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/14 11:26:42 by tlucanti          #+#    #+#             */
/*   Updated: 2022/08/14 20:11:25 by tlucanti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef STACK_BASE_HPP
# define STACK_BASE_HPP

# include <deque>
# include "defs.hpp"

TLU_NAMESPACE_BEGIN


// =============================================================================
// -------------------------------- stack_base ---------------------------------
template <class type_T, class container_T=std::deque<type_T> >
class stack_base
{
// --------------------------------- typedefs ----------------------------------
public:
    typedef container_T                                 container_type;
    typedef typename container_type::value_type         value_type;
    typedef typename container_type::size_type          size_type;
    typedef typename container_type::reference          reference;
    typedef typename container_type::const_reference    const_reference;

// ----------------------------- protected fields ------------------------------
protected:
    container_T c;

// =============================================================================
// ------------------------------ initialization -------------------------------
public:
    constexpr stack_base() DEFAULT

// -----------------------------------------------------------------------------
    explicit constexpr stack_base(const container_type &cont) :
        c(cont)
    {}

// -----------------------------------------------------------------------------
    constexpr stack_base(const stack_base &cpy) :
        c(cpy.c)
    {}

// -----------------------------------------------------------------------------
#if CPP11
    explicit constexpr stack_base(container_type &&cont) noexcept :
        c(std::move(cont))
    {}

// -----------------------------------------------------------------------------
    constexpr stack_base(stack_base &&mv) noexcept :
        c(std::move(mv.c))
    {}
#endif /* CPP11 */

// -------------------------------- destructors --------------------------------
    ~stack_base() DEFAULT

// --------------------------------- assigning ---------------------------------
    constexpr stack_base    &operator =(const stack_base &cpy)
    {
        c = cpy.c;
        return *this;
    }

// =============================================================================
// ------------------------------ element access -------------------------------
    constexpr reference         top()
    {
        return c.back();
    }

// -----------------------------------------------------------------------------
    constexpr const_reference   top() const
    {
        return c.back();
    }

// =============================================================================
// --------------------------------- capacity ----------------------------------
    WUR constexpr bool          empty() const
    {
        return c.empty();
    }

// -----------------------------------------------------------------------------
    WUR constexpr size_type     size() const
    {
        return c.size();
    }

// =============================================================================
// --------------------------------- modifiers ---------------------------------
    constexpr void              push(const value_type &value)
    {
        c.push_back(value);
    }

// -----------------------------------------------------------------------------
    constexpr void              pop()
    {
        c.pop_back();
    }

// -----------------------------------------------------------------------------
    constexpr void              swap(stack_base &other)
    {
        c.swap(other.c);
    }

// =============================================================================
// -------------------------- lexicographical compare --------------------------
    friend constexpr bool operator ==(
            const stack_base<type_T, container_T> &lhs,
            const stack_base<type_T, container_T> &rhs
        )
    {
        return lhs.c == rhs.c;
    }

// -----------------------------------------------------------------------------
    friend constexpr bool operator !=(
            const stack_base<type_T, container_T> &lhs,
            const stack_base<type_T, container_T> &rhs
        )
    {
        return lhs.c != rhs.c;
    }

// -----------------------------------------------------------------------------
    friend constexpr bool operator >(
            const stack_base<type_T, container_T> &lhs,
            const stack_base<type_T, container_T> &rhs
        )
    {
        return lhs.c > rhs.c;
    }

// -----------------------------------------------------------------------------
    friend constexpr bool operator >=(
            const stack_base<type_T, container_T> &lhs,
            const stack_base<type_T, container_T> &rhs
        )
    {
        return lhs.c >= rhs.c;
    }

// -----------------------------------------------------------------------------
    friend constexpr bool operator <(
            const stack_base<type_T, container_T> &lhs,
            const stack_base<type_T, container_T> &rhs
        )
    {
        return lhs.c < rhs.c;
    }

// -----------------------------------------------------------------------------
    friend constexpr bool operator <=(
            const stack_base<type_T, container_T> &lhs,
            const stack_base<type_T, container_T> &rhs
        )
    {
        return lhs.c <= rhs.c;
    }

}; /* stack_base */

TLU_NAMESPACE_END

#endif /* STACK_BASE_HPP */
