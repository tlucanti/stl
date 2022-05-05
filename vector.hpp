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

# define WRAP_ITERATOR_BOOL_OPERATOR_MACRO(__op) constexpr bool operator __op( \
    const self_type &cmp) const { return _ptr __op cmp._ptr; }

# define WRAP_ITERATOR_ARITHMETIC_OPERATOR_MACRO(__type, __op) constexpr self_type \
    operator __op(__type shift) { return self_type(_ptr __op shift); }

    template <class type_T, class allocator_T=std::allocator<type_T> >
    class vector_base;

template <class type_T>
class wrap_iterator : std::iterator<std::random_access_iterator_tag, type_T>
{
// --------------------------------- typedefs ----------------------------------
public:
    typedef std::iterator<std::random_access_iterator_tag, type_T>  iterator_traits;
// TODO:    typedef typename iterator_traits::iterator_category     iterator_category;
    typedef typename iterator_traits::value_type            value_type;
    typedef typename iterator_traits::difference_type       difference_type;
    typedef typename iterator_traits::pointer               pointer;
// TODO:    typedef typename iterator_traits::reference             reference;

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
    WRAP_ITERATOR_BOOL_OPERATOR_MACRO(==)
    WRAP_ITERATOR_BOOL_OPERATOR_MACRO(!=)
    WRAP_ITERATOR_BOOL_OPERATOR_MACRO(> )
    WRAP_ITERATOR_BOOL_OPERATOR_MACRO(>=)
    WRAP_ITERATOR_BOOL_OPERATOR_MACRO(< )
    WRAP_ITERATOR_BOOL_OPERATOR_MACRO(<=)

# if CPP20
    constexpr bool operator <=>(const self_type &cmp)
    {
        // TODO: bool operator <=>
        # warning "IMPLEMENT FUNCTION"
        __ABORT("FUNCTION NOT IMPLEMENTED", "");
    }
# endif /* CPP20 */

    constexpr value_type operator *() noexcept { return *_ptr; } // *i

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

    constexpr self_type &operator --() noexcept // --i
    {
        --_ptr;
        return *this;
    }

    constexpr self_type operator --(int) noexcept // i--
    {
        const self_type ret = self_type(_ptr);
        --_ptr;
        return ret;
    }

    WRAP_ITERATOR_ARITHMETIC_OPERATOR_MACRO(difference_type, +)
    WRAP_ITERATOR_ARITHMETIC_OPERATOR_MACRO(difference_type, -)
    WRAP_ITERATOR_ARITHMETIC_OPERATOR_MACRO(const self_type &, -)

// ---------------------------- static members ----------------------------------
    static constexpr difference_type distance(const self_type &begin,
        const self_type &end)
    {
        if (end._ptr > begin._ptr)
            return end._ptr - begin._ptr;
        return begin._ptr - end._ptr;
    }

// ---------------------------- static members ----------------------------------
    template <typename input_it>
    static constexpr difference_type distance(const input_it &begin, const input_it &end)
    {
        return std::distance(begin, end);
    }

    static constexpr void copy(const self_type &begin, const self_type &end, pointer dest)
    {
        copy(begin._ptr, end._ptr, dest);
    }

    template <class input_it>
    static constexpr void copy(const input_it &begin, const input_it &end, pointer dest) noexcept
    {
        std::copy(begin, end, dest);
    }

    static constexpr void copy(pointer start, pointer end, pointer dest)
    {
        if (start > dest)
        {
            while (start != end)
                *dest++ = *start++;
        }
        else
        {
            difference_type dist = distance(start, end);
            while (dist)
                dest[dist--] = *end--;
        }
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
            _allocate(size);
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
            _allocate(size);
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
            _allocate(n);
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
            _allocate(_size);
            iterator::copy(cpy._begin, cpy._end, _begin);
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
#endif /* CPP11 */

// -----------------------------------------------------------------------------
# if CPP11
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
        _size = init.size();
        if (_size > 0)
        {
            _allocate(_size);
            iterator::copy(init.begin(), init.end(), _begin);
        }
    }
# endif /* CPP11 */

// -------------------------------- destructors --------------------------------
    constexpr ~vector_base()
    {
        _deallocate();
        _size = 0;
    }

// --------------------------------- assigning ---------------------------------
    constexpr vector_base &operator =(const self_type &cpy)
    {
        _size = cpy._size;
        _deallocate();
        if (_size > 0)
        {
            _allocate(_size);
            iterator::copy(cpy._begin, cpy._end, _begin);
        }
    }

// -----------------------------------------------------------------------------
# if CPP11
    constexpr vector_base &operator =(self_type &&mv)
    {
    // TODO: move assignment
    // TODO: do not forget to move allocator
# warning "IMPLEMENT FUNCTION"
        __ABORT("FUNCTION NOT IMPLEMENTED", "");
    }
#endif /* CPP11 */

// -----------------------------------------------------------------------------
# if CPP11
    constexpr vector_base &operator =(init_list_type init_list)
    {
    // TODO: copy assignment
    // TODO: copy allocator?
# warning "IMPLEMENT FUNCTION"
        __ABORT("FUNCTION NOT IMPLEMENTED", "");
        /* TODO: use for (auto &&value : init_list) */
    }
#endif /* CPP11 */

// -----------------------------------------------------------------------------
    constexpr void assign(size_type count, const_reference value)
    {
    // TODO: assign
# warning "IMPLEMENT FUNCTION"
        __ABORT("FUNCTION NOT IMPLEMENTED", "");
    }

// -----------------------------------------------------------------------------
    template <class input_it>
    constexpr void assign(input_it first, input_it last)
    {
    // TODO: assign
# warning "IMPLEMENT FUNCTION"
        __ABORT("FUNCTION NOT IMPLEMENTED", "");
    }

// -----------------------------------------------------------------------------
# if CPP11
    constexpr void assign(init_list_type init_list)
    {
    // TODO: assign
# warning "IMPLEMENT FUNCTION"
        __ABORT("FUNCTION NOT IMPLEMENTED", "");
    }
# endif /* CPP11 */

// =============================================================================
// ------------------------------ element access -------------------------------
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
        /* TODO: not this */ return allocator_traits::max_size();
        /* TODO: last element in golden ratio array */
    }
    constexpr void reserve(size_type capacity)
    {
    // TODO: reserve
# warning "IMPLEMENT FUNCTION"
        __ABORT("FUNCTION NOT IMPLEMENTED", "");
    }

# if CPP11
    constexpr void shriknk_to_fit() {} // allocated size is always optimal
# endif /* CPP11 */

// =============================================================================
// --------------------------------- modifiers ---------------------------------
    constexpr void clear() noexcept
    {
    // TODO: clear
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
            start[i] = value; // TODO: maybe use _allocator.construct() here
    }

// -----------------------------------------------------------------------------
    template <class input_it>
    constexpr void insert(const_iterator pos, input_it first, input_it last)
    {
        size_type count = iterator::distance(first, last);
        pointer start = _insert(pos, count);
        for (size_type i=0; i < count; ++i)
            start[i] = first++; // TODO: maybe use _allocator.construct() here
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
# endif /* CPP11 */

// -----------------------------------------------------------------------------
# if CPP11
    template <class ... arg_type>
    constexpr iterator emplace(const_iterator pos, arg_type && ... args)
    {
    // TODO: emplace
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
        ++_size;
        *_end++ = value;
    }
// -----------------------------------------------------------------------------
# if CPP11
    constexpr void push_back(rvalue_type value)
    {
        _append(std::move(value));
        ++_size;
        *_end++ = value;
    }
# endif /* CPP11 */

// -----------------------------------------------------------------------------
# if CPP11
    template <class ... arg_type>
    constexpr reference emplace_back(arg_type && ... args)
    {
    // TODO: emplace_back
# warning "IMPLEMENT FUNCTION"
        __ABORT("FUNCTION NOT IMPLEMENTED", "");
    }
# endif /* CPP11 */

// -----------------------------------------------------------------------------
    constexpr void pop_back()
    {
        _pop();
        --_size;
        --_end;
    }

// -----------------------------------------------------------------------------
    constexpr void resize(size_type count)
    {
        _size = count;
        if (count > _allocated)
            _append(count - _allocated);
        else if (count < _allocated)
            _pop(_allocated - count);
    }

// -----------------------------------------------------------------------------
    constexpr void resize(size_type count, const_reference value)
    {
        difference_type index = _size;
        resize(count);
        for (; index < _size; ++index)
            _begin[index] = value; // TODO: maybe use _allocator.construct() here
    }

// -----------------------------------------------------------------------------
    constexpr void swap(self_type &swp)
    {
        std::swap(_size, swp._size);
        std::swap(_allocated, swp._allocated);
        std::swap(_allocator, swp._allocator);
        std::swap(_begin, swp._begin);
        std::swap(_end, swp._end);
        std::swap(c, swp.c);
        /* TODO: add swaps for other private/protected fields */
    }

// =============================================================================
// ----------------------------- internal methods ------------------------------
private:
// ----------------------------- memory allocation -----------------------------
    __WUR constexpr size_type _upper_bound_grid(size_type req)
    {
        static const size_type grid[] = {
            7u, 11u, 18u, 29u, 47u, 76u, 123u, 199u, 322u, 521u, 843u, 1364u,
            2207u, 3571u, 5778u, 9349u, 15127u, 24476u, 39603u, 64079u, 103682u,
            167761u, 271443u, 439204u, 710647u, 1149851u, 1860498u, 3010349u,
            4870847u, 7881196u, 12752043u, 20633239u, 33385282u, 54018521u,
            87403803u, 141422324u, 228826127u, 370248451u, 599074578u,
            969323029u, 1568397607u, 2537720636u, 4106118243u, 6643838879u,
            10749957122u, 17393796001u, 28143753123u, 45537549124u,
            73681302247u, 119218851371u, 192900153618u, 312119004989u,
            505019158607u, 817138163596u, 1322157322203u, 2139295485799u,
            3461452808002u, 5600748293801u, 9062201101803u, 14662949395604u,
            23725150497407u, 38388099893011u, 62113250390418u, 100501350283429u,
            162614600673847u, 263115950957277u, 425730551631124u,
            688846502588401u, 1114577054219525u, 1803423556807926u,
            2918000611027450u, 4721424167835376u, 7639424778862826u,
            12360848946698202u, 20000273725561032u, 32361122672259236u,
            52361396397820264u, 84722519070079504u, 137083915467899776u,
            221806434537979296u, 358890350005879104u, 580696784543858432u,
            939587134549737600u, 1520283919093596160u, 2459871053643333632u,
            3980154972736929792u, 6440026026380264448u, 10420180999117195264u,
            16860207025497458688u
        };
        int l, r, mid;

        if (req <= 103682)
        {
            l = 0;
            r = 20;
        } else {
            l = 21;
            r = 88;
        }
        while (r - l > 1)
        {
            mid = (l + r) / 2;
            if (req >= grid[mid])
                l = (l + r) / 2;
            else
                r = (l + r) / 2;
        }
        return grid[r];
    }

// -----------------------------------------------------------------------------
    constexpr void _allocate(size_type req_size)
    {
        _allocated = _upper_bound_grid(req_size);
        try {
            _begin = _allocator.allocate(_allocated);
        } catch (std::bad_alloc &)
        {
            _deallocate();
            _size = 0;
            throw ;
        }
        _size = req_size;
        _end = _begin + _size;
    }

// -----------------------------------------------------------------------------
    constexpr void _deallocate()
    {
        _allocator.deallocate(_begin, _allocated);
        _allocated = 0;
        _begin = nullptr;
        _end = nullptr;
        // TODO: maybe call _allocator.destroy() for all elements
    }

// -----------------------------------------------------------------------------
    constexpr void _alloc_more(size_type least)
    {
        _allocated = _upper_bound_grid(least);
        _reallocate(_allocated);
    }

// -----------------------------------------------------------------------------
    constexpr void _shrink()
    {
        if (_size < _allocated / (golden_ratio * golden_ratio) and _allocated > 7)
            _alloc_more(_size);
    }

// -----------------------------------------------------------------------------
    constexpr void _reallocate(size_type new_size)
    {
        try {
            pointer new_begin = _allocator.allocate(new_size);
            iterator::copy(_begin, _size, new_begin);
            _deallocate(_begin, _allocated);
            _allocated = new_size;
            _begin = new_begin;
            _end = _begin + _size;
        } catch (std::bad_alloc &) {
            _deallocate();
            _size = 0;
            throw ;
        }
    }
// ------------------------------ element access -------------------------------
    __WUR constexpr reference _at(difference_type pos) const
    {
        if (pos >= static_cast<difference_type>(_size) or pos < -static_cast<difference_type>(_size))
            throw std::out_of_range("out of range");
        return _begin[(pos + _size) % _size];
    }

// -----------------------------------------------------------------------------
    __WUR constexpr iterator _iterator(pointer ptr)
    {
        return iterator(ptr);
    }

// -----------------------------------------------------------------------------
    __WUR constexpr reverse_iterator _riterator(pointer ptr)
    {
# warning "IMPLEMENT FUNCTION"
        __ABORT("FUNCTION NOT IMPLEMENTED", "");
    }

// -----------------------------------------------------------------------------
    __WUR constexpr pointer _insert(pointer ptr, size_type count=1)
    {
        difference_type index = ptr - _begin;
        if (_size + count > _allocated)
            _alloc_more(_size + count);
        iterator::copy(_begin + index + 1, count, _end - _begin - index);
        return _begin + index;
    }

// -----------------------------------------------------------------------------
    __WUR constexpr pointer _erase(pointer ptr, size_type count=1)
    {
        iterator::copy(ptr + count, ptr, count);
        _size -= count;
        _shrink();
        _end -= count; // TODO: maybe uwe _allocator.destroy(_end) here
    }

// -----------------------------------------------------------------------------
    constexpr void _append(size_type count=1)
    {
        if (_size + count > _allocated)
            _alloc_more(_size + count);
    }

// -----------------------------------------------------------------------------
    constexpr void _pop(size_type count=1)
    {
        _size -= count;
        _shrink();
        _end -= count; // TODO: maybe uwe _allocator.destroy(_end) here
    }

// -----------------------------------------------------------------------------
};

TLU_NAMESPACE_END

#endif /* VECTOR_HPP */
