/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rb_tree_iterator.hpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tlucanti <tlucanti@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/14 19:36:08 by tlucanti          #+#    #+#             */
/*   Updated: 2022/08/14 19:37:36 by tlucanti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef RB_TREE_ITERATOR_HPP
# define RB_TREE_ITERATOR_HPP

# include "defs.hpp"
# include "type_traits.hpp"

TLU_NAMESPACE_BEGIN


// =============================================================================
// --------------------------- rb_tree_iterator_base ---------------------------
template<class value_T, class cmp_T, class alloc_T>
class rb_tree_iterator_base : public
    std::iterator<std::bidirectional_iterator_tag, value_T>
{
// --------------------------------- typedefs ----------------------------------
private:
    typedef std::bidirectional_iterator_tag         tag;
    typedef typename std::iterator<tag, value_T>    iterator_traits;

public:
    typedef typename iterator_traits::iterator_category     iterator_category;
    typedef typename iterator_traits::value_type            value_type;
    typedef typename iterator_traits::difference_type       difference_type;
    typedef typename iterator_traits::pointer               pointer;
    typedef typename iterator_traits::reference             reference;
    typedef const reference                                 const_reference;
    typedef const pointer                                   const_pointer;

protected:
    typedef rb_tree<
            typename remove_const<value_type>::type,
            cmp_T,
            alloc_T
        >   tree_type;
    typedef typename tree_type::rb_node     rb_node;

// ------------------------------ private members ------------------------------
private:
    rb_node *_ptr;
    bool    _end;

public:
// ------------------------------ public methods -------------------------------
    virtual reference operator *() const noexcept
    {
        return _ptr->get_key();
    }

// -----------------------------------------------------------------------------
    virtual bool      operator ==(
            const rb_tree_iterator_base &cmp
        ) const noexcept
    {
        bool x = _end;
        bool y = cmp._end;
        bool z = _ptr == cmp._ptr;
        return (x and y) or ((not x) and (not y) and z);
    }

// -----------------------------------------------------------------------------
    virtual bool      operator !=(
            const rb_tree_iterator_base &cmp
        ) const noexcept
    {
        return not operator ==(cmp);
    }

// -----------------------------------------------------------------------------
    virtual rb_node   *get_node() const noexcept
    {
        return _ptr;
    }

// -----------------------------------------------------------------------------
    virtual bool      is_end() const noexcept
    {
        return _end;
    }

// ----------------------------- protected methods -----------------------------
protected:
    constexpr rb_tree_iterator_base() noexcept :
        _ptr(nullptr),
        _end(false)
    {}

// -----------------------------------------------------------------------------
    constexpr rb_tree_iterator_base(rb_node *ptr, bool end) noexcept :
        _ptr(ptr),
        _end(end)
    {}

// -----------------------------------------------------------------------------
    virtual ~rb_tree_iterator_base() noexcept DEFAULT

// -----------------------------------------------------------------------------
    constexpr void  increment() noexcept // ++i
    {
        if (_end)
            _end = false;
        else
            _ptr = tree_type::next(_ptr);
        if (_ptr == nullptr)
            _end = true;
    }

// -----------------------------------------------------------------------------
    constexpr void  decrement() noexcept // --i
    {
        if (_end)
            _end = false;
        else
            _ptr = tree_type::prev(_ptr);
    }

}; /* rb_tree_iterator_base */


// =============================================================================
// ------------------------- rb_tree forward_iterator --------------------------
template<class value_T, class cmp_T, class alloc_T>
class rb_tree_iterator : public
    rb_tree_iterator_base<value_T, cmp_T, alloc_T>
{
// --------------------------------- typedefs ----------------------------------
private:
    typedef rb_tree_iterator_base<value_T, cmp_T, alloc_T>  base_class;

public:
    typedef typename base_class::iterator_category  iterator_category;
    typedef typename base_class::value_type         value_type;
    typedef typename base_class::difference_type    difference_type;
    typedef typename base_class::pointer            pointer;
    typedef typename base_class::reference          reference;
    typedef const reference                         const_reference;
    typedef const pointer                           const_pointer;

public:
    typedef typename base_class::rb_node            rb_node;

// -----------------------------------------------------------------------------
public:
    explicit rb_tree_iterator(rb_node *ptr, bool end=false) noexcept
        : base_class(ptr, end)
    {}

// ------------------------------ public methods -------------------------------
public:
    constexpr rb_tree_iterator() noexcept DEFAULT

// -----------------------------------------------------------------------------
    constexpr rb_tree_iterator(const rb_tree_iterator &cpy) noexcept
        : base_class(cpy)
    {}

// -----------------------------------------------------------------------------
    ~rb_tree_iterator() noexcept DEFAULT

// -----------------------------------------------------------------------------
    constexpr rb_tree_iterator  &operator ++() noexcept // ++i
    {
        this->increment();
        return *this;
    }

// -----------------------------------------------------------------------------
    constexpr rb_tree_iterator  operator ++(int) noexcept // i++
    {
        rb_tree_iterator ret(*this);
        this->increment();
        return ret;
    }

// -----------------------------------------------------------------------------
    constexpr rb_tree_iterator  &operator --() noexcept // --i
    {
        this->decrement();
        return *this;
    }

// -----------------------------------------------------------------------------
    constexpr rb_tree_iterator  operator --(int) noexcept // i--
    {
       rb_tree_iterator ret(*this);
       this->decrement();
       return ret;
    }

}; /* rb_tree_iterator */


// =============================================================================
// ------------------------- rb_tree reverse_iterator --------------------------
template<class value_T, class key_T, class cmp_T>
class rb_tree_reverse_iterator : public
    rb_tree_iterator_base<value_T, key_T, cmp_T>
{
// --------------------------------- typedefs ----------------------------------
private:
    typedef rb_tree_iterator_base<value_T, key_T, cmp_T>    base_class;

public:
    typedef typename base_class::iterator_category  iterator_category;
    typedef typename base_class::value_type         value_type;
    typedef typename base_class::difference_type    difference_type;
    typedef typename base_class::pointer            pointer;
    typedef typename base_class::reference          reference;
    typedef const reference                         const_reference;
    typedef const pointer                           const_pointer;

protected:
    typedef typename base_class::rb_node rb_node;

// -----------------------------------------------------------------------------
public: // TODO: remove this
    constexpr rb_tree_reverse_iterator(rb_node *ptr, bool end) noexcept
        : base_class(ptr, end)
    {}

// ------------------------------ public methods -------------------------------
public:
    constexpr rb_tree_reverse_iterator() noexcept DEFAULT

// -----------------------------------------------------------------------------
    constexpr rb_tree_reverse_iterator(const rb_tree_reverse_iterator &cpy) noexcept
        : base_class(cpy)
    {}

// -----------------------------------------------------------------------------
    ~rb_tree_reverse_iterator() noexcept DEFAULT

// -----------------------------------------------------------------------------
    constexpr rb_tree_reverse_iterator  &operator ++() noexcept // ++i
    {
        this->decrement();
        return *this;
    }

// -----------------------------------------------------------------------------
    constexpr rb_tree_reverse_iterator  operator ++(int) noexcept // i++
    {
        rb_tree_reverse_iterator ret(*this);
        this->decrement();
        return ret;
    }

// -----------------------------------------------------------------------------
    constexpr rb_tree_reverse_iterator  &operator --() noexcept // --i
    {
        this->increment();
        return *this;
    }

// -----------------------------------------------------------------------------
    constexpr rb_tree_reverse_iterator  operator --(int) noexcept // i--
    {
        rb_tree_reverse_iterator ret(*this);
        this->increment();
        return ret;
    }

}; /* rb_tree_reverse_iterator */

TLU_NAMESPACE_END

#endif /* RB_TREE_ITERATOR_HPP */
