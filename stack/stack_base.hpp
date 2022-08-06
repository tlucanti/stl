
#ifndef STACK_BASE_HPP
# define STACK_BASE_HPP

# include "defs.h"
# include <deque>

TLU_NAMESPACE_BEGIN

template <class type_T, class container_T=std::deque<type_T> >
class stack_base
{
public:
    typedef container_T                                 container_type;
    typedef typename container_type::value_type         value_type;
    typedef typename container_type::size_type          size_type;
    typedef typename container_type::reference          reference;
    typedef typename container_type::const_reference    const_reference;

protected:
    container_T c;

public:
// ------------------------------ constructors --------------------------------
    stack_base() DEFAULT

    explicit stack_base(const container_type &cont) : c(cont) {}

    stack_base(const stack_base &cpy) : c(cpy.c) {}

#if CPP11
    explicit stack_base(container_type &&cont) : c(std::move(cont)) {}

    stack_base(stack_base &&mv) noexcept : c(std::move(mv.c)) {}
#endif /* CPP11 */

// ------------------------------------- destructors -------------------------
    ~stack_base() DEFAULT

// --------------------------------- opearator = -----------------------------
    stack_base &operator =(const stack_base &cpy)
    {
        c = cpy.c;
        return *this;
    }

// --------------------------- element access -----------------------------
    reference top()
    {
        return c.back();
    }

    const_reference top() const
    {
        return c.back();
    }

// ------------------------------- capacity --------------------------------
    WUR bool empty() const
    {
        return c.empty();
    }

    WUR size_type size() const
    {
        return c.size();
    }

// ------------------------------- modifiers ----------------------------------
    void push(const value_type &value)
    {
        c.push_back(value);
    }

    void pop()
    {
        c.pop_back();
    }

    void swap(stack_base &other)
    {
        c.swap(other.c);
    }

// ----------------------------------- non-member functions ----------------------
    friend bool operator ==(
            const stack_base<type_T, container_T> &lhs,
            const stack_base<type_T, container_T> &rhs
        )
    {
        return lhs.c == rhs.c;
    }

    friend bool operator !=(
            const stack_base<type_T, container_T> &lhs,
            const stack_base<type_T, container_T> &rhs
    )
    {
        return lhs.c != rhs.c;
    }

    friend bool operator >(
            const stack_base<type_T, container_T> &lhs,
            const stack_base<type_T, container_T> &rhs
    )
    {
        return lhs.c > rhs.c;
    }

    friend bool operator >=(
            const stack_base<type_T, container_T> &lhs,
            const stack_base<type_T, container_T> &rhs
    )
    {
        return lhs.c >= rhs.c;
    }

    friend bool operator <(
            const stack_base<type_T, container_T> &lhs,
            const stack_base<type_T, container_T> &rhs
    )
    {
        return lhs.c < rhs.c;
    }

    friend bool operator <=(
            const stack_base<type_T, container_T> &lhs,
            const stack_base<type_T, container_T> &rhs
    )
    {
        return lhs.c <= rhs.c;
    }
}; /* stack_base */

TLU_NAMESPACE_END

namespace std
{
    template <class type_T, class container_T>
    void swap(
            TLU_NAMESPACE::stack_base<type_T, container_T> &lhs,
            TLU_NAMESPACE::stack_base<type_T, container_T> &rhs
        )
    {
        lhs.swao(rhs);
    }
} /* std */

#endif /* STACK_BASE_HPP */