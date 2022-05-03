/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vector.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tlucanti <tlucanti@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/14 23:16:33 by tlucanti          #+#    #+#             */
/*   Updated: 2022/03/28 17:00:13 by tlucanti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef VECTOR_HPP
# define VECTOR_HPP

# include "defs.h"
# include <iterator>
# include "enable_if.hpp"
# include "is_integral.hpp"

# define golden_ratio 1.618033988749895

TLU_NAMESPACE_BEGIN

# define WRAP_ITERATOR_OPERATOR_MACRO(__op) constexpr bool operator __op( \
    const self_type &cmp) const { return _ptr __op cmp._ptr; }

    template <class type_T, class allocator_T=std::allocator<type_T> >
    class vector_base;

template <class type_T>
class wrap_iterator : std::iterator<std::random_access_iterator_tag, type_T>
{
// --------------------------------- typedefs ----------------------------------
public:
    typedef std::iterator<std::random_access_iterator_tag, type_T>  iterator_traits;
//    typedef typename iterator_traits::iterator_category     iterator_category;
    typedef typename iterator_traits::value_type            value_type;
    typedef typename iterator_traits::difference_type       difference_type;
    typedef typename iterator_traits::pointer               pointer;
//    typedef typename iterator_traits::reference             reference;

private:
    typedef wrap_iterator<value_type>                    self_type;

    friend vector_base<value_type>;

// ------------------------------ private fields -------------------------------
private:
    pointer    _ptr;


// =============================================================================
// ------------------------------ initialization -------------------------------
private:
    explicit constexpr wrap_iterator(pointer ptr) : _ptr(ptr) {}
public:
    constexpr wrap_iterator() : _ptr(nullptr) {}
    constexpr wrap_iterator(const self_type &cpy) : _ptr(cpy._ptr) {}

# if CPP11
    constexpr wrap_iterator(self_type &&mv) noexcept
        : _ptr(mv._ptr)
    {
        mv._ptr = nullptr;
    }
# endif /* CPP11 */

// =============================================================================
// --------------------------------- operators ---------------------------------
    WRAP_ITERATOR_OPERATOR_MACRO(==);
    WRAP_ITERATOR_OPERATOR_MACRO(!=);
    WRAP_ITERATOR_OPERATOR_MACRO(> );
    WRAP_ITERATOR_OPERATOR_MACRO(>=);
    WRAP_ITERATOR_OPERATOR_MACRO(< );
    WRAP_ITERATOR_OPERATOR_MACRO(<=);

# if CPP20
    constexpr bool operator <=>(const self_type &cmp)
    {
        # warning "IMPLEMENT FUNCTION"
        __ABORT("FUNCTION NOT IMPLEMENTED", "");
    }
# endif /* CPP20 */

    constexpr value_type operator *() noexcept// *i
    {
        return *_ptr;
    }

    constexpr self_type &operator ++() noexcept // ++i
    {
        ++_ptr;
        return *this;
    }

    constexpr self_type operator ++(int) noexcept // i++
    {
        const self_type ret = self_type(_ptr);
        ++_ptr;
        return ret;
    }

    constexpr self_type operator +(difference_type shift)
    {
        return self_type(_ptr + shift);
    }

    constexpr self_type operator -(difference_type shift)
    {
        return self_type(_ptr - shift);
    }

    constexpr difference_type operator -(const self_type &dif) noexcept
    {
        return _ptr - dif._ptr;
    }

// ---------------------------- static members ----------------------------------
    static constexpr difference_type distance(const self_type &end,
        const self_type &begin)
    {
        return end._ptr - begin._ptr;
    }

    template <class input_it, class dest_it>
    static constexpr void copy(input_it begin, input_it end, dest_it dest) noexcept
    {
        while (begin != end)
            *dest++ = *begin++;
    }

    template <class input_it, class dest_it>
    static constexpr void copy(input_it start, difference_type count, dest_it dest)
    {
        while (count--)
            *dest++ = *start++;
    }
};

template <class type_T, class allocator_T>
class vector_base
{
// --------------------------------- typedefs ----------------------------------
public:
    typedef allocator_T                                 allocator_type;
    typedef std::allocator_traits<allocator_type>       allocator_traits;
    typedef typename allocator_traits::size_type        size_type;

    typedef type_T                                      value_type;
    typedef vector_base<value_type>                     self_type;
    typedef value_type &                                reference;
    typedef const value_type &                          const_reference;
    typedef value_type &&                               rvalue_type;
    typedef typename allocator_traits::difference_type  difference_type;
    typedef typename allocator_traits::pointer          pointer;
    typedef typename allocator_traits::const_pointer    const_pointer;
    typedef wrap_iterator<value_type>                   iterator;
    typedef wrap_iterator<const value_type>             const_iterator;
    typedef int                                         reverse_iterator;
    typedef int                                         const_reverse_iterator;
# if CPP11
    typedef std::initializer_list<value_type>           init_list_type;
#endif /* CPP11 */

// ----------------------------- protected fields ------------------------------
protected:
    self_type    &c;

// ------------------------------ private fields -------------------------------
    size_type       _size;
    size_type       _allocated;
    allocator_type  _allocator;
    pointer         _begin;
    pointer         _end;

public:
// =============================================================================
// ------------------------------ initialization -------------------------------
    constexpr vector_base() :
        c(*this),
        _size(0),
        _allocated(0),
        _allocator(allocator_type()),
        _begin(nullptr),
        _end(nullptr)
    {}

// -----------------------------------------------------------------------------
    constexpr explicit vector_base(const allocator_type &alloc) :
        c(*this),
        _size(0),
        _allocated(0),
        _allocator(alloc),
        _begin(nullptr),
        _end(nullptr)
    {}

// -----------------------------------------------------------------------------
    constexpr explicit vector_base(size_type size,
        const allocator_type &alloc=allocator_type()
    ) :
        c(*this),
        _size(size),
        _allocated(0),
        _allocator(alloc),
        _begin(nullptr),
        _end(nullptr)
    {
        if (size > 0)
        {
            _begin = _allocate(size, _allocated);
            _end = _begin + _size;
        }
    }

// -----------------------------------------------------------------------------
    constexpr vector_base(size_type size,
        const_reference value,
        const allocator_type &alloc=allocator_type()
    ) :
        c(*this),
        _size(size),
        _allocated(0),
        _allocator(alloc),
        _begin(nullptr),
        _end(nullptr)
    {
        if (size > 0)
        {
            _begin = _allocate(size, _allocated);
            _end = _begin + size;
        }
    }

// -----------------------------------------------------------------------------
    template <class input_it>
    explicit constexpr vector_base(input_it first,
        typename TLU_NAMESPACE::enable_if<not TLU_NAMESPACE::is_integral<
            input_it>::value, input_it>::type last,
        const allocator_type &alloc=allocator_type()
    ) :
        c(*this),
        _size(0),
        _allocated(0),
        _allocator(alloc),
        _begin(nullptr),
        _end(nullptr)
    {
        difference_type n = iterator::distance(last, first);
        if (n > 0)
        {
            _begin = _allocate(n, _allocated);
            _end = _begin + n;
            iterator::copy(first, last, _begin);
        }
    }

// -----------------------------------------------------------------------------
    constexpr vector_base(const self_type &cpy, const allocator_type &alloc=allocator_type()) :
        c(*this),
        _size(cpy._size),
        _allocated(0),
        _allocator(alloc),
        _begin(nullptr),
        _end(nullptr)
    {
        if (_size > 0)
        {
            _begin = _allocate(_size, _allocated);
            _end = _begin + _size;
            iterator::copy(cpy._begin, _size, _begin);
        }
    }

// -----------------------------------------------------------------------------
# if CPP11
    constexpr vector_base(self_type &&mv,
        const allocator_type &alloc=allocator_type()
    ) :
        c(*this),
        _size(mv._size),
        _allocated(mv._allocated),
        _allocator(alloc),
        _begin(mv._begin),
        _end(mv._end)
    {
        mv._size = 0;
        mv._allocated = 0;
        mv._begin = nullptr;
        mv._end = nullptr;
    }

// -----------------------------------------------------------------------------
    constexpr vector_base(init_list_type init,
        const allocator_type &alloc=allocator_type()
    ) :
        c(*this),
        _size(0),
        _allocated(0),
        _allocator(alloc),
        _begin(nullptr),
        _end(nullptr)
    {
        size_type n = init.size();
        if (n > 0)
        {
            _begin = _allocate(_size, _allocated);
            _end = _begin + _size;
            iterator::copy(init.begin(), _size, _begin);
        }
    }
# endif /* CPP11 */

// -------------------------------- destructors --------------------------------
    constexpr ~vector_base()
    {
        _deallocate(_begin, _allocated);
    }

// --------------------------------- assigning ---------------------------------
    constexpr vector_base &operator =(const self_type &cpy)
    {
        _size = cpy._size;
        _deallocate(_begin);
        if (_size > 0)
        {
            _begin = _allocate(_size, _allocated);
            _end = _begin + _size;
            iterator::copy(cpy._begin, _size, _begin);
        }
    }

// -----------------------------------------------------------------------------
# if CPP11
    constexpr vector_base &operator =(self_type &&mv)
    {
# warning "IMPLEMENT FUNCTION"
        __ABORT("FUNCTION NOT IMPLEMENTED", "");
    }

// -----------------------------------------------------------------------------
    constexpr vector_base &operator =(init_list_type init_list)
    {
# warning "IMPLEMENT FUNCTION"
        __ABORT("FUNCTION NOT IMPLEMENTED", "");
        /* use for (auto &&value : init_list) */
    }
#endif /* CPP11 */

// -----------------------------------------------------------------------------
    constexpr void assign(size_type count, const_reference value)
    {
# warning "IMPLEMENT FUNCTION"
        __ABORT("FUNCTION NOT IMPLEMENTED", "");
    }

// -----------------------------------------------------------------------------
    template <class input_it>
    constexpr void assign(input_it first, input_it last)
    {
# warning "IMPLEMENT FUNCTION"
        __ABORT("FUNCTION NOT IMPLEMENTED", "");
    }

// -----------------------------------------------------------------------------
# if CPP11
    constexpr void assign(init_list_type init_list)
    {
# warning "IMPLEMENT FUNCTION"
        __ABORT("FUNCTION NOT IMPLEMENTED", "");
    }
# endif /* CPP11 */

// =============================================================================
// ------------------------------ element accces -------------------------------
    __WUR constexpr allocator_type get_allocator() const noexcept { return _allocator; }

    __WUR constexpr reference at(size_type pos) { return _at(pos); }
    __WUR constexpr const_reference at(size_type pos) const { return _at(pos); }

    __WUR constexpr reference operator[](size_type pos) { return _at(pos); }
    __WUR constexpr const_reference operator[](size_type pos) const {return _at(pos);}

    __WUR constexpr reference front() { return _at((size_type)0); }
    __WUR constexpr const_reference front() const { return _at((size_type)0); }

    __WUR constexpr reference back() { return _at(_size - 1); }
    __WUR constexpr const_reference back() const { return _at(_size - 1); }

    __WUR constexpr pointer data() { return _begin; }
    __WUR constexpr const_pointer data() const { return _begin; }

// =============================================================================
// --------------------------------- iterators ---------------------------------
    __WUR constexpr iterator begin() noexcept { return _iterator(_begin); }
    __WUR constexpr const_iterator begin() const noexcept { return _iterator(_begin); }

    __WUR constexpr iterator end() noexcept { return _iterator(_end); }
    __WUR constexpr const_iterator end() const noexcept { return _iterator(_end); }

    __WUR constexpr reverse_iterator rbegin() noexcept { return _riterator(_end - 1); }
    __WUR constexpr const_reverse_iterator rbegin() const noexcept
    { return _riterator(_end - 1); }

    __WUR constexpr reverse_iterator rend() noexcept { return _riterator(_begin - 1); }
    __WUR constexpr const_reverse_iterator rend() const noexcept
    { return _riterator(_begin - 1); }

# if CPP11
    __WUR constexpr const_iterator cbegin() const noexcept {return _iterator(_begin); }
    __WUR constexpr const_iterator cend() const noexcept { return _iterator(_end); }

    __WUR constexpr const_reverse_iterator crbegin() const noexcept
    { return _riterator(_end - 1); }
    __WUR constexpr const_reverse_iterator crend() const noexcept
    { return _riterator(_begin - 1); }
# endif /* CPP11 */

// =============================================================================
// --------------------------------- capacity ----------------------------------
    __WUR constexpr bool empy() const noexcept { return !_size; }
    __WUR constexpr size_type size() const noexcept { return _size; }
    __WUR constexpr size_type max_size() const noexcept
    {
# warning "IMPLEMENT FUNCTION"
        __ABORT("FUNCTION NOT IMPLEMENTED", "");
        /* not this */ return allocator_traits::max_size();
        /* last element in golden ratio array */
    }
    constexpr void reserve(size_type capacity)
    {
# warning "IMPLEMENT FUNCTION"
        __ABORT("FUNCTION NOT IMPLEMENTED", "");
    }

# if CPP11
    constexpr void shriknk_to_fit()
    {
# warning "IMPLEMENT FUNCTION"
        __ABORT("FUNCTION NOT IMPLEMENTED", "");
    }
# endif /* CPP11 */

// =============================================================================
// --------------------------------- modifiers ---------------------------------
    constexpr void clear() noexcept
    {
# warning "IMPLEMENT FUNCTION"
        __ABORT("FUNCTION NOT IMPLEMENTED", "");
    }

// -----------------------------------------------------------------------------
    constexpr iterator insert(const_iterator pos, const_reference value)
    {
        pointer start = _insert(pos);
        *start = value;
        return _iterator(start);
    }

// -----------------------------------------------------------------------------
# if CPP11
    constexpr iterator insert(const_iterator pos, rvalue_type value)
    {
        pointer start = _insert(pos);
        *start = std::move(value);
    }
# endif /* CPP11 */

// -----------------------------------------------------------------------------
    constexpr void insert(const_iterator pos, size_type count, const_reference value)
    {
        pointer start = _insert(pos, count);
        for (size_type i=0; i < count; ++i)
            start[i] = value;
    }

// -----------------------------------------------------------------------------
    template <class input_it>
    constexpr void insert(const_iterator pos, input_it first, input_it last)
    {
        size_type count = iterator::distance(first, last);
        pointer start = _insert(pos, count);
        for (size_type i=0; i < count; ++i)
            start[i] = first++;
    }

// -----------------------------------------------------------------------------
# if CPP11
    constexpr iterator insert(const_iterator pos, init_list_type init_list)
    {
        iterator ret = _iterator(pos);
        size_type count = init_list.size();
        pointer start = _insert(pos, count);
        size_type i = 0;
        for (auto &&value : init_list)
            start[i++] = std::move(value);
        return ++ret;
    }
// -----------------------------------------------------------------------------
    template <class ... arg_type>
    constexpr iterator emplace(const_iterator pos, arg_type && ... args)
    {
# warning "IMPLEMENT FUNCTION"
        __ABORT("FUNCTION NOT IMPLEMENTED", "");
    }
# endif /* CPP11 */

// -----------------------------------------------------------------------------
    constexpr iterator erase(const_iterator pos)
    {
        if (pos == end())
            return end();
        pointer fin = _erase(pos);
        return _iterator(fin);
    }

// -----------------------------------------------------------------------------
    constexpr iterator erase(const_iterator first, const_iterator last)
    {
        size_type count = iterator::distance(first, last);
        if (count == 0)
            return last;
        pointer fin = _erase(first, count);
        return _iterator(fin);
    }

// -----------------------------------------------------------------------------
    constexpr void push_back(const_reference value)
    {
        _append(value);
    }

// -----------------------------------------------------------------------------
# if CPP11
    constexpr void push_back(rvalue_type value)
    {
        _append(std::move(value));
    }

// -----------------------------------------------------------------------------
    template <class ... arg_type>
    constexpr reference emplace_back(arg_type && ... args)
    {
# warning "IMPLEMENT FUNCTION"
        __ABORT("FUNCTION NOT IMPLEMENTED", "");
    }
# endif /* CPP11 */

// -----------------------------------------------------------------------------
    constexpr void pop_back()
    {
        _pop();
    }

// -----------------------------------------------------------------------------
    constexpr void resize(size_type count, const_reference value=value_type())
    {
        if (_size > count)
            _pop(_size - count);
        else
            _append(value, count - _size);
    }

// -----------------------------------------------------------------------------
    constexpr void swap(self_type &swp)
    {
        std::swap(_size, swp._size);
        std::swap(_allocated, swp._allocated);
        std::swap(_allocator, swp._allocator);
        std::swap(_begin, swp._begin);
        std::swap(_end, swp._end);
# warning "IMPLEMENT FUNCTION"
        __ABORT("FUNCTION NOT IMPLEMENTED", "");
        /* add swaps for other private/protected fields */
    }

// =============================================================================
// ----------------------------- internal methods ------------------------------
private:
// ----------------------------- memory allocation -----------------------------
    constexpr pointer _allocate(size_type req_size, size_type &alloc_size)
    {
        alloc_size = req_size * golden_ratio;
        return _allocator.allocate(alloc_size);
    }
// -----------------------------------------------------------------------------
    constexpr void _construct(pointer start, size_type construct_size)
    {
# warning "IMPLEMENT FUNCTION"
        __ABORT("FUNCTION NOT IMPLEMENTED", "");
    }
// -----------------------------------------------------------------------------
    constexpr void _deallocate(pointer &start, size_type size)
    {
        _allocator.deallocate(start, size);
        start = nullptr;
    }
// -----------------------------------------------------------------------------
    constexpr void _destruct(pointer start, size_type destruct_size)
    {
# warning "IMPLEMENT FUNCTION"
        __ABORT("FUNCTION NOT IMPLEMENTED", "");
    }

// ------------------------------ element accces -------------------------------
    constexpr reference _at(size_type pos) const
    {
        if (pos >= _size or pos < -_size)
            throw std::out_of_range("");
        return _begin[(pos + _size) % _size];
    }

// -----------------------------------------------------------------------------
    constexpr iterator _iterator(pointer ptr)
    {
        return iterator(ptr);
    }

// -----------------------------------------------------------------------------
    constexpr reverse_iterator _riterator(pointer ptr)
    {
# warning "IMPLEMENT FUNCTION"
        __ABORT("FUNCTION NOT IMPLEMENTED", "");
    }

// -----------------------------------------------------------------------------
    constexpr pointer _insert(pointer ptr, size_type count=1)
    {
# warning "IMPLEMENT FUNCTION"
        __ABORT("FUNCTION NOT IMPLEMENTED", "");
        // if ptr == _end -> call _append
    }

// -----------------------------------------------------------------------------
    constexpr pointer _erase(pointer ptr, size_type count=1)
    {
# warning "IMPLEMENT FUNCTION"
        __ABORT("FUNCTION NOT IMPLEMENTED", "");
        // if ptr == _end -> call _pop
    }

// -----------------------------------------------------------------------------
    constexpr void _append(const_reference value, size_type count=1)
    {
# warning "IMPLEMENT FUNCTION"
        __ABORT("FUNCTION NOT IMPLEMENTED", "");
    }

// -----------------------------------------------------------------------------
    constexpr void _pop(size_type count=1)
    {
# warning "IMPLEMENT FUNCTION"
        __ABORT("FUNCTION NOT IMPLEMENTED", "");
    }
};

TLU_NAMESPACE_END

#endif /* VECTOR_HPP */
