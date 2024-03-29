/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wrap_iterator.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tlucanti <tlucanti@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/14 16:01:52 by tlucanti          #+#    #+#             */
/*   Updated: 2022/08/14 19:39:48 by tlucanti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef WRAP_ITERATOR_HPP
# define WRAP_ITERATOR_HPP

#include <iterator>
#include "defs.hpp"

TLU_NAMESPACE_BEGIN


// =============================================================================
// ------------------------------- iterator base -------------------------------
template <class type_T>
class wrap_iterator_base : public
    std::iterator<std::random_access_iterator_tag, type_T>
{
// --------------------------------- typedefs ----------------------------------
protected:
    typedef std::random_access_iterator_tag     tag;
    typedef std::iterator<tag, type_T>          iterator_traits;

public:
    typedef typename iterator_traits::iterator_category     iterator_category;
    typedef typename iterator_traits::value_type            value_type;
    typedef typename iterator_traits::difference_type       difference_type;
    typedef typename iterator_traits::pointer               pointer;
    typedef typename iterator_traits::reference             reference;

    typedef wrap_iterator_base<const value_type>            self_type;

// ------------------------------ private members ------------------------------
protected:
    pointer    _ptr;

// ----------------------------- protected methods -----------------------------
protected:
    explicit constexpr wrap_iterator_base(pointer ptr)
        : _ptr(ptr)
    {}

// ------------------------------ public methods -------------------------------
public:
    virtual ~wrap_iterator_base() noexcept DEFAULT

// -----------------------------------------------------------------------------
    constexpr virtual value_type    &operator *() const noexcept
    {
        return *_ptr;
    }

    constexpr pointer               get_ptr() const noexcept
    {
        return _ptr;
    }

// ---------------------------- static members ----------------------------------
    static constexpr difference_type    distance(
            const self_type &begin,
            const self_type &end
        )
    {
        if (end._ptr > begin._ptr)
            return end._ptr - begin._ptr;
        return begin._ptr - end._ptr;
    }

// -----------------------------------------------------------------------------
    template <class input_it>
    static constexpr difference_type    distance(
            input_it first,
            const input_it &last
        )
    {
        return _distance(
            first,
            last,
            typename std::iterator_traits<input_it>::iterator_category()
        );
    }

// -----------------------------------------------------------------------------
    static constexpr int    compare(
            self_type lhs,
            self_type rhs,
            difference_type size
        )
    {
        while (size-- > 0)
        {
            if (*lhs != *rhs)
                return (*lhs > *rhs) * 2 - 1;
        }
        return 0;
    }

// ------------------------------ private methods ------------------------------
private:
    template <class rand_it>
    static constexpr difference_type    _distance(
            rand_it first,
            const rand_it &last,
            std::random_access_iterator_tag
        )
    {
        return last - first;
    }

}; /* wrap_iterator_base */


// =============================================================================
// --------------------------------- iterator ----------------------------------
template <class type_T>
class wrap_iterator : public
    wrap_iterator_base<type_T>
{
// --------------------------------- typedefs ----------------------------------
private:
    typedef wrap_iterator<type_T>                       self_type;
    typedef wrap_iterator_base<type_T>                  base_class;
public:
    typedef typename base_class::iterator_traits        iterator_traits;
    typedef typename base_class::iterator_category      iterator_category;
    typedef typename base_class::value_type             value_type;
    typedef typename base_class::difference_type        difference_type;
    typedef typename base_class::pointer                pointer;
    typedef typename base_class::reference              reference;

// ------------------------------ public methods -------------------------------
public:
    explicit constexpr wrap_iterator(pointer ptr) noexcept
        : base_class(ptr)
    {}

// -----------------------------------------------------------------------------
    constexpr wrap_iterator() noexcept
        : base_class(nullptr)
    {}

// -----------------------------------------------------------------------------
    ~wrap_iterator() noexcept override DEFAULT

// -----------------------------------------------------------------------------
    constexpr bool  operator ==(const self_type &cmp) const noexcept
    {
        return this->_ptr == cmp._ptr;
    }

// -----------------------------------------------------------------------------
    constexpr bool  operator !=(const self_type &cmp) const noexcept
    {
        return this->_ptr != cmp.get_ptr();
    }

// -----------------------------------------------------------------------------
    constexpr bool  operator >(const self_type &cmp) const noexcept
    {
        return this->_ptr > cmp.get_ptr();
    }

// -----------------------------------------------------------------------------
    constexpr bool  operator <(const self_type &cmp) const noexcept
    {
        return this->_ptr < cmp.get_ptr();
    }

// -----------------------------------------------------------------------------
    constexpr bool  operator <=(const self_type &cmp) const noexcept
    {
        return this->_ptr <= cmp.get_ptr();
    }

// -----------------------------------------------------------------------------
    constexpr bool  operator >=(const self_type &cmp) const noexcept
    {
        return this->_ptr >= cmp.get_ptr();
    }

// -----------------------------------------------------------------------------
    constexpr self_type     operator +(difference_type shift) const noexcept
    {
        return self_type(this->_ptr + shift);
    }

// -----------------------------------------------------------------------------
    constexpr self_type     operator -(difference_type shift) const noexcept
    {
        return self_type(this->_ptr - shift);
    }

// -----------------------------------------------------------------------------
    constexpr self_type     operator +=(difference_type shift) noexcept
    {
        return self_type((this->_ptr += shift));
    }

// -----------------------------------------------------------------------------
    constexpr self_type     operator -=(difference_type shift) noexcept
    {
        return self_type((this->_ptr -= shift));
    }

// -----------------------------------------------------------------------------
    constexpr self_type     &operator ++() noexcept // ++i
    {
        ++this->_ptr;
        return *this;
    }

// -----------------------------------------------------------------------------
    constexpr self_type     operator ++(int) noexcept // i++
    {
        ++this->_ptr;
        return self_type(this->_ptr - 1);
    }

// -----------------------------------------------------------------------------
    constexpr self_type     &operator --() noexcept // --i
    {
        --this->_ptr;
        return *this;
    }

// -----------------------------------------------------------------------------
    constexpr self_type     operator --(int) noexcept // i--
    {
        --this->_ptr;
        return self_type(this->_ptr + 1);
    }

// -----------------------------------------------------------------------------
    constexpr difference_type   operator -(const self_type &diff) const noexcept
    {
        return this->_ptr - diff._ptr;
    }

}; /* wrap_iterator */


// =============================================================================
// ----------------------------- reverse iterator ------------------------------
template <class type_T>
class wrap_reverse_iterator : public wrap_iterator_base<type_T>
{
// --------------------------------- typedefs ----------------------------------
private:
    typedef wrap_reverse_iterator<type_T>               self_type;
    typedef wrap_iterator_base<type_T>                  base_class;
public:
    typedef typename base_class::iterator_traits        iterator_traits;
    typedef typename base_class::iterator_category      iterator_category;
    typedef typename base_class::value_type             value_type;
    typedef typename base_class::difference_type        difference_type;
    typedef typename base_class::pointer                pointer;
    typedef typename base_class::reference              reference;

// ------------------------------ public methods -------------------------------
public:
    explicit constexpr wrap_reverse_iterator(pointer ptr) noexcept
        : base_class(ptr)
    {}

// -----------------------------------------------------------------------------
    constexpr wrap_reverse_iterator() noexcept
        : base_class(nullptr)
    {}
    
// -----------------------------------------------------------------------------
    ~wrap_reverse_iterator() noexcept override DEFAULT


// -----------------------------------------------------------------------------
    constexpr bool  operator ==(const self_type &cmp) const noexcept
    {
        return this->_ptr == cmp._ptr;
    }

// -----------------------------------------------------------------------------
    constexpr bool  operator !=(const self_type &cmp) const noexcept
    {
        return this->_ptr != cmp._ptr;
    }

// -----------------------------------------------------------------------------
    virtual bool    operator >(const base_class &cmp) const noexcept
    {
        return this->_ptr < cmp.get_ptr();
    }

// -----------------------------------------------------------------------------
    constexpr bool  operator >=(const self_type &cmp) const noexcept
    {
        return this->_ptr <= cmp._ptr;
    }

// -----------------------------------------------------------------------------
    constexpr bool  operator <(const self_type &cmp) const noexcept
    {
        return this->   _ptr > cmp._ptr;
    }

// -----------------------------------------------------------------------------
    constexpr bool  operator <=(const self_type &cmp) const noexcept
    {
        return this->_ptr >= cmp._ptr;
    }

// -----------------------------------------------------------------------------
    constexpr self_type     operator +(difference_type shift) const noexcept
    {
        return self_type(this->_ptr + -shift);
    }

// -----------------------------------------------------------------------------
    constexpr self_type     operator -(difference_type shift) const noexcept
    {
        return self_type(this->_ptr - -shift);
    }

// -----------------------------------------------------------------------------
    constexpr self_type     operator +=(difference_type shift) noexcept
    {
        return self_type((this->_ptr += -shift));
    }

// -----------------------------------------------------------------------------
    constexpr self_type     operator -=(difference_type shift) noexcept
    {
        return self_type((this->_ptr -= -shift));
    }

// -----------------------------------------------------------------------------
    constexpr self_type     &operator ++() noexcept // ++i
    {
        --this->_ptr;
        return *this;
    }

// -----------------------------------------------------------------------------
    constexpr self_type     operator ++(int) noexcept // i++
    {
        --this->_ptr;
        return self_type(this->_ptr + 1);
    }

// -----------------------------------------------------------------------------
    constexpr self_type     &operator --() noexcept // --i
    {
        ++this->_ptr;
        return *this;
    }

// -----------------------------------------------------------------------------
    constexpr self_type     operator --(int) noexcept // i--
    {
        ++this->_ptr;
        return self_type(this->_ptr - 1);
    }

// -----------------------------------------------------------------------------
    constexpr difference_type   operator -(const self_type &diff) const noexcept
    {
        return diff._ptr - this->_ptr;
    }

}; /* wrap_reverse_iterator */

TLU_NAMESPACE_END

#endif /* WRAP_ITERATOR_HPP */
