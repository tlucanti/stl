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
# include "type_traits.hpp"
# include "iterator.hpp"

# define golden_ratio 1.618033988749895

TLU_NAMESPACE_BEGIN

template <class type_T, class allocator_T=std::allocator<type_T>>
class vector_base
{
// --------------------------------- typedefs ----------------------------------
public:
    typedef allocator_T                                 allocator_type;
    typedef std::allocator_traits<allocator_type>       allocator_traits;
    typedef typename allocator_traits::size_type        size_type;

    typedef type_T                                      value_type;
    typedef vector_base<value_type, allocator_type>     self_type;
    typedef value_type &                                reference;
    typedef const value_type &                          const_reference;
#if CPP11
    typedef value_type &&                               rvalue_type;
#endif /* CPP11 */
    typedef typename allocator_traits::difference_type  difference_type;
    typedef typename allocator_traits::pointer          pointer;
    typedef typename allocator_traits::const_pointer    const_pointer;
    typedef wrap_iterator<value_type>                   iterator;
    typedef wrap_iterator<const value_type>             const_iterator;
    typedef wrap_reverse_iterator<value_type>           reverse_iterator;
    typedef wrap_reverse_iterator<const value_type>     const_reverse_iterator;
# if CPP11
    typedef std::initializer_list<value_type>           init_list_type;
#endif /* CPP11 */

// ----------------------------- protected fields ------------------------------
PROTECTED:
    self_type    &c;

// ------------------------------ private fields -------------------------------
    size_type       _allocated;
    allocator_type  _allocator;
    pointer         _begin;
    pointer         _end;

public:
// =============================================================================
// ------------------------------ initialization -------------------------------
    constexpr vector_base() :
        c(*this),
        _allocated(0),
        _allocator(allocator_type()),
        _begin(nullptr),
        _end(nullptr)
    {}

// -----------------------------------------------------------------------------
    constexpr explicit vector_base(const allocator_type &alloc) :
        c(*this),
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
        _allocated(0),
        _allocator(alloc),
        _begin(nullptr),
        _end(nullptr)
    {
        if (size > 0)
            _init(size);
    }

// -----------------------------------------------------------------------------
    constexpr vector_base(size_type size,
        const_reference value,
        const allocator_type &alloc=allocator_type()
    ) :
        c(*this),
        _allocated(0),
        _allocator(alloc),
        _begin(nullptr),
        _end(nullptr)
    {
        if (size > 0)
            _init(size, false);
        _construct(_begin, size, value);
    }

// -----------------------------------------------------------------------------
    template <class input_it>
    explicit constexpr vector_base(typename TLU_NAMESPACE::enable_if<
            not TLU_NAMESPACE::is_integral<input_it>::value, input_it>::type first,
            input_it last, const allocator_type &alloc=allocator_type()
    ) :
        c(*this),
        _allocated(0),
        _allocator(alloc),
        _begin(nullptr),
        _end(nullptr)
    {
        difference_type n = iterator::distance(first, last);
        if (n > 0)
        {
            _init(n, false);
            _copy(first, last, _begin);
        }
    }

// -----------------------------------------------------------------------------
    constexpr vector_base(const self_type &cpy, const allocator_type &alloc=allocator_type()) :
        c(*this),
        _allocated(0),
        _allocator(alloc),
        _begin(nullptr),
        _end(nullptr)
    {
        if (cpy.size() > 0)
        {
            _init(cpy.size(), false);
            _copy(cpy._begin, cpy._end, _begin);
        }
    }

// -----------------------------------------------------------------------------
# if CPP11
    constexpr vector_base(self_type &&mv,
        const allocator_type &alloc=allocator_type()
    ) noexcept :
        c(*this),
        _allocated(mv._allocated),
        _allocator(alloc),
        _begin(mv._begin),
        _end(mv._end)
    {
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
        _allocated(0),
        _allocator(alloc),
        _begin(nullptr),
        _end(nullptr)
    {
        if (init.size() > 0)
        {
            _init(init.size(), false);
            _copy(init.begin(), init.end(), _begin);
        }
    }
# endif /* CPP11 */

// -------------------------------- destructors --------------------------------
    constexpr ~vector_base()
    {
        _deallocate();
    }

// --------------------------------- assigning ---------------------------------
    constexpr vector_base &operator =(const self_type &cpy)
    {
        _deallocate();
        if (cpy.size() > 0)
        {
            _init(cpy.size());
            _copy(cpy._begin, cpy._end, _begin);
        }
    }

// -----------------------------------------------------------------------------
# if CPP11
    constexpr vector_base &operator =(self_type &&mv) noexcept
    {
        _size(mv._size);
        _allocated(mv._allocated);
        _allocator(std::move(mv._allocator));
        _begin(mv._begin);
        _end(mv._end);
        mv._size = 0;
        mv._allocated = 0;
        mv._begin = nullptr;
        mv._end = nullptr;
    }

#endif /* CPP11 */

// -----------------------------------------------------------------------------
# if CPP11
    constexpr vector_base &operator =(init_list_type init_list)
    {
        _deallocate();
        if (init_list.size() > 0)
        {
            _init(init_list.size(), false);
            _copy(init_list.begin(), init_list.end(), _begin);
        }
    }
#endif /* CPP11 */

// -----------------------------------------------------------------------------
    constexpr void assign(size_type count, const_reference value)
    {
    // TODO: assign
//# warning "IMPLEMENT FUNCTION"
        __ABORT("FUNCTION NOT IMPLEMENTED", "");
    }

// -----------------------------------------------------------------------------
    template <class input_it>
    constexpr void assign(input_it first, input_it last)
    {
    // TODO: assign
//# warning "IMPLEMENT FUNCTION"
        __ABORT("FUNCTION NOT IMPLEMENTED", "");
    }

// -----------------------------------------------------------------------------
# if CPP11
    constexpr void assign(init_list_type init_list)
    {
    // TODO: assign
//# warning "IMPLEMENT FUNCTION"
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

    __WUR constexpr reference back() { return _at(size() - 1); }
    __WUR constexpr const_reference back() const { return _at(size() - 1); }

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
    __WUR constexpr bool empty() const noexcept { return !size(); }
    __WUR constexpr size_type size() const noexcept { return _end - _begin; }
    __WUR constexpr size_type max_size() const noexcept { return 16860207025497458688u; }
    constexpr void reserve(size_type capacity)
    {
        if (capacity && capacity >= _allocated)
            _alloc_more(capacity);
    }
    __WUR constexpr size_type capacity() const noexcept { return _allocated; }
# if CPP11
    constexpr void shriknk_to_fit() {} // allocated size is always optimal
# endif /* CPP11 */

// =============================================================================
// --------------------------------- modifiers ---------------------------------
    constexpr void clear() noexcept
    {
        _deallocate(false);
    }

// -----------------------------------------------------------------------------
    constexpr iterator insert(iterator pos, const_reference value)
    {
        pointer start = _insert(pos);
        _construct_at(start, value);
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
        _construct(start, count, value);
    }

// -----------------------------------------------------------------------------
    template <class input_it>
    constexpr void insert(const_iterator pos,
        typename TLU_NAMESPACE::enable_if<not TLU_NAMESPACE::is_integral<
        input_it>::value, input_it>::type first, input_it last)
    {
        _copy(first, last, pos._ptr);
    }

// -----------------------------------------------------------------------------
# if CPP11
    constexpr iterator insert(const_iterator pos, init_list_type init_list)
    {
        iterator ret = _iterator(pos);
        pointer start = _insert(pos, init_list.size());
        _copy(init_list.begin(), init_list.end(), start);
        return ++ret;
    }
# endif /* CPP11 */

// -----------------------------------------------------------------------------
# if CPP11
    template <class ... arg_type>
    constexpr iterator emplace(const_iterator pos, arg_type && ... args)
    {
    // TODO: emplace
//# warning "IMPLEMENT FUNCTION"
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
        _destroy(first, count);
        pointer fin = _erase(first, count);
        return _iterator(fin);
    }

// -----------------------------------------------------------------------------
    constexpr void push_back(const_reference value)
    {
        _append();
        _construct_at(_end++, value);
    }
// -----------------------------------------------------------------------------
# if CPP11
    constexpr void push_back(rvalue_type value)
    {
        _append();
        _construct_at(_end++, std::move(value));
    }
# endif /* CPP11 */

// -----------------------------------------------------------------------------
# if CPP11
    template <class ... arg_type>
//    constexpr reference emplace_back(arg_type && ... args)
//    {
//    // TODO: emplace_back
//# warning "IMPLEMENT FUNCTION"
//        __ABORT("FUNCTION NOT IMPLEMENTED", "");
//    }
# endif /* CPP11 */

// -----------------------------------------------------------------------------
    constexpr void pop_back()
    {
        _pop();
        _destroy_at(--_end);
    }

// -----------------------------------------------------------------------------
    constexpr void resize(size_type count)
    {
        size_type size = this->size();
        if (count >= _allocated)
        {
            _alloc_more(count);
            size_type new_elems = count - size;
            _construct(_begin + size, new_elems);
        }
        else if (count < _allocated)
            _pop(_allocated - count);
        _end = _begin + count; // TODO: maybe this is redundant
    }

// -----------------------------------------------------------------------------
    constexpr void resize(size_type count, const_reference value)
    {
//        difference_type index = _size;
//        resize(count);
//        for (; index < _size; ++index)
//            _begin[index] = value; // TODO: maybe use _allocator.construct() here
    }

// -----------------------------------------------------------------------------
    constexpr void swap(self_type &swp)
    {
        std::swap(_allocated, swp._allocated);
        std::swap(_allocator, swp._allocator);
        std::swap(_begin, swp._begin);
        std::swap(_end, swp._end);
        std::swap(c, swp.c);
        /* TODO: add swaps for other private/protected fields */
    }

// =============================================================================
// ----------------------------- internal methods ------------------------------
PRIVATE:
// ----------------------------- memory allocation -----------------------------
    __WUR static constexpr size_type _upper_bound_grid(size_type req)
    {
        constexpr const size_type grid[] = {
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

        if (req < 7u)
            return 7u;
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
    constexpr void _construct_at(pointer ptr)
    {
#if CPP20
        allocator_traits::construct(_allocator, ptr);
#else
        _allocator.construct(ptr);
#endif
    }

// -----------------------------------------------------------------------------
    constexpr void _construct_at(pointer ptr, const_reference val)
    {
#if CPP20
        allocator_traits::construct(_allocator, ptr, val);
#else
        _allocator.construct(ptr, val);
#endif
    }

// -----------------------------------------------------------------------------
    constexpr void _destroy_at(pointer ptr)
    {
#if CPP20
        std::destroy_at(ptr);
#else
        _allocator.destroy(ptr);
#endif
    }

// -----------------------------------------------------------------------------
    constexpr void _construct(pointer start, size_type cnt)
    {
        while (cnt--)
            _construct_at(start++);
    }

// -----------------------------------------------------------------------------
    constexpr void _construct(pointer start, size_type cnt, const_reference val)
    {
        while (cnt--)
            _construct_at(start++, val);
    }

// -----------------------------------------------------------------------------
    constexpr void _destroy(pointer start, size_type cnt)
    {
        while (cnt--)
            _destroy_at(start++);
    }

// -----------------------------------------------------------------------------
    constexpr void _init(size_type req_size, bool do_construct=true)
    {
        _allocated = _upper_bound_grid(req_size);
        try {
            _begin = _allocate(_allocated);
        } catch (std::bad_alloc &)
        {
            _deallocate();
            throw ;
        }
        _end = _begin + req_size;
        if (do_construct)
            _construct(_begin, req_size);
    }

// -----------------------------------------------------------------------------
    __WUR constexpr pointer _allocate(size_type alloc_size)
    {
#ifdef __DEBUG
        pointer _ret = _allocator.allocate(alloc_size);
        memset(_ret, 0, alloc_size);
        return _ret;
#else
        return _allocator.allocate(alloc_size);
#endif
    }

// -----------------------------------------------------------------------------
    template <class forward_iterator_type>
    constexpr void _copy(typename enable_if<is_iterator<
            forward_iterator_type>::value, forward_iterator_type>::type first,
        const forward_iterator_type &last, pointer dest)
    {
        while (first != last)
            _construct_at(dest++, *first++);
    }

// -----------------------------------------------------------------------------
    constexpr void _copy(pointer first, pointer last, pointer dest)
    {
        _copy(first, dest, last - first);
    }

// -----------------------------------------------------------------------------
    constexpr void _copy(pointer src, pointer dst, size_type cnt)
    {
        if (src > dst)
        {
            while (cnt--)
                _construct_at(dst++, *src++);
        }
        else if (dst > src)
        {
            dst += cnt;
            src += cnt;
            while (cnt--)
                _construct_at(--dst, *--src);
        }
    }

// -----------------------------------------------------------------------------
    constexpr void _deallocate(bool do_deallocate=true)
    {
        size_type size = _end - _begin;
        while (size--)
            _destroy_at(--_end);
        if (do_deallocate)
        {
            _allocator.deallocate(_begin, _allocated);
            _allocated = 0;
            _begin = nullptr;
            _end = nullptr;
        }
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
        if (size() < _allocated / (golden_ratio * golden_ratio) and _allocated > 7)
            _alloc_more(size());
    }

// -----------------------------------------------------------------------------
    constexpr void _reallocate(size_type new_size)
    {
        try {
            pointer new_begin = _allocate(new_size);
            _copy(_begin, _end, new_begin);
            size_type size = _end - _begin;
            _deallocate();
            _allocated = new_size;
            _begin = new_begin;
            _end = _begin + size;
        } catch (std::bad_alloc &) {
            _deallocate();
            throw ;
        }
    }
// ------------------------------ element access -------------------------------
    __WUR constexpr reference _at(difference_type pos) const
    {
        size_type size = this->size();
        if (pos >= static_cast<difference_type>(size) or pos < -static_cast<difference_type>(size))
            throw std::out_of_range("out of range");
        return _begin[(pos + size) % size];
    }

// -----------------------------------------------------------------------------
    __WUR constexpr iterator _iterator(pointer ptr)
    {
        return iterator(ptr);
    }

// -----------------------------------------------------------------------------
    __WUR constexpr reverse_iterator _riterator(pointer ptr)
    {
        return reverse_iterator(ptr);
    }

// -----------------------------------------------------------------------------
    __WUR constexpr pointer _insert(pointer ptr, size_type count=1)
    {
        difference_type index = ptr - _begin;
        _append(count);
        _copy(_begin + index + 1, _end - _begin - index, count);
        return _begin + index;
    }

// -----------------------------------------------------------------------------
    __WUR constexpr pointer _erase(pointer ptr, size_type count=1)
    {
        _destroy(ptr, count);
        _copy(ptr, ptr + count, count);
        _shrink();
    }

// -----------------------------------------------------------------------------
    constexpr void _append(size_type count=1)
    {
        if (size() + count >= _allocated)
            _alloc_more(size() + count);
    }

// -----------------------------------------------------------------------------
    constexpr void _pop(size_type count=1)
    {
        _shrink();
        _end -= count; // TODO: maybe uwe _allocator.destroy(_end) here
    }

// -----------------------------------------------------------------------------
};

TLU_NAMESPACE_END

#endif /* VECTOR_HPP */
