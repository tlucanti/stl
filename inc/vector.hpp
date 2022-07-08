/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vector.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tlucanti <tlucanti@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/14 23:16:33 by tlucanti          #+#    #+#             */
/*   Updated: 2022/07/06 18:04:04 by tlucanti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef VECTOR_HPP
# define VECTOR_HPP

# include <limits>
# include "defs.h"
# include "type_traits.hpp"
# include "iterator.hpp"

TLU_NAMESPACE_BEGIN

template <class type_T, class allocator_T=std::allocator<type_T> >
class vector_base
{
// --------------------------------- typedefs ----------------------------------
public:
    typedef allocator_T                                 allocator_type;
    typedef size_t                                      size_type;

    typedef type_T                                      value_type;
    typedef vector_base<value_type, allocator_type>     self_type;
    typedef value_type &                                reference;
    typedef const value_type &                          const_reference;
    typedef std::ptrdiff_t                              difference_type;
    typedef typename allocator_type::pointer            pointer;
    typedef typename allocator_type::const_pointer      const_pointer;
    typedef wrap_iterator<value_type>                   iterator;
    typedef wrap_iterator<const value_type>             const_iterator;
    typedef wrap_reverse_iterator<value_type>           reverse_iterator;
    typedef wrap_reverse_iterator<const value_type>     const_reverse_iterator;

# if CPP11
    typedef value_type &&                               rvalue_type;
    typedef std::initializer_list<value_type>           init_list_type;
#endif /* CPP11 */

// ------------------------------ private fields -------------------------------
PRIVATE:
    difference_type _allocated;
    allocator_type  _allocator;
    pointer         _begin;
    pointer         _end;

    constexpr const static double golden_ratio = 1.618033988749895;

public:
// =============================================================================
// ------------------------------ initialization -------------------------------
    constexpr vector_base() :
        _allocated(0),
        _allocator(allocator_type()),
        _begin(nullptr),
        _end(nullptr)
    /**
        \brief default vector constructor
        \details creates empty container
        \time O(1)
        \memory O(1)

        \author tlucanti
    */
    {}

// -----------------------------------------------------------------------------
    constexpr explicit vector_base(const allocator_type &alloc) :
        _allocated(0),
        _allocator(alloc),
        _begin(nullptr),
        _end(nullptr)
    /**
        \brief default vector constructor
        \details constructor with explicitly set allocator, creates empty
            container
        \param alloc reference to allocator
        \time O(1)
        \memory O(1)

        \author tlucanti
    */
    {}

// -----------------------------------------------------------------------------
    constexpr explicit vector_base(difference_type size,
        const allocator_type &alloc=allocator_type()
    ) :
        _allocated(0),
        _allocator(alloc),
        _begin(nullptr),
        _end(nullptr)
    /**
        \brief vector constructor with set size
        \details constructor with explicitly set allocator, creates container of
            size `size` and fill it with default values
        \param size size of constructing container
        \param alloc reference to allocator
        \time O(size)
        \memory O(size)

        \author tlucanti
    */
    {
        if (LIKELY(size > 0))
            _init(size);
    }

// -----------------------------------------------------------------------------
    constexpr vector_base(difference_type size,
        const_reference value,
        const allocator_type &alloc=allocator_type()
    ) :
        _allocated(0),
        _allocator(alloc),
        _begin(nullptr),
        _end(nullptr)
    /**
        \brief vector constructor with set size
        \details constructor with explicitly set allocator, creates container of
            size `size` and fill with copies of `value`
        \param size size of constructing container
        \param value const reference to the value to be filled into container
        \param alloc reference to allocator
        \time O(size)
        \memory O(size)

        \author tlucanti
    */
    {
        if (LIKELY(size > 0))
            _init(size, false);
        _construct(_begin, size, value);
    }

// -----------------------------------------------------------------------------
    template <class input_it>
    explicit constexpr vector_base(NOT_INTEGRAL(input_it) first,
            input_it last, const allocator_type &alloc=allocator_type()
    ) :
        _allocated(0),
        _allocator(alloc),
        _begin(nullptr),
        _end(nullptr)
    /**
        \brief vector constructor iterator range
        \details constructor with explicitly set allocator, creates container of
            size `size` and fill it with copies of values in iterator range from
            `first` to `end`
        \param first iterator to first element of copy range
        \param last iterator to last element of copy range (included)
        \param alloc reference to allocator
        \time O(distance(first, last))
        \memory O(distance(first, last))

        \author tlucanti
    */
    {
        difference_type n = iterator::distance(first, last);
        if (LIKELY(n > 0))
        {
            _init(n, false);
            _copy(first, last, _begin);
        }
    }

// -----------------------------------------------------------------------------
    constexpr vector_base(const self_type &cpy,
            const allocator_type &alloc=allocator_type()
    ) :
        _allocated(0),
        _allocator(alloc),
        _begin(nullptr),
        _end(nullptr)
    /**
        \brief vector copy constructor
        \defails constructor with explicitly set allocator, creates deep copy of
            vector `cpy`
        \param cpy const reference to copying container (with same type)
        \param alloc reference to allocator
        \time O(size(cpy))
        \memory O(size(cpy))

        \author tlucanti
    */
    {
        if (LIKELY(cpy.size() > 0))
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
        _allocated(mv._allocated),
        _allocator(alloc),
        _begin(mv._begin),
        _end(mv._end)
    /**
        \brief vector move constructor
        \details constructor with explicitly set allocator, creates container
            from content of vector `mv` and destroys vector `mv`
        \param mv rvalue reference to source container
        \param alloc reference to allocator
        \time O(1)
        \memory O(1)

        \author tlucanti
    */
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
        _allocated(0),
        _allocator(alloc),
        _begin(nullptr),
        _end(nullptr)
    /**
        \brief initializer list constructor
        \details constructor with explicitly set allocator, creates container
            from initializer list `init`
        \param init initializer list
        \param alloc reference to allocator
        \time O(size(init))
        \memory O(size(init))

        \author tlucanti
    */
    {
        if (init.size() > 0)
        {
            _init(static_cast<difference_type>(init.size()), false);
            _copy(init.begin(), init.end(), _begin);
        }
    }
# endif /* CPP11 */

// -------------------------------- destructors --------------------------------
    ~vector_base()
    /**
        \brief container destructor
        \details destructor destroys all elements in container and deallocates
            memory from vector
        \time O(size(self))
        \memory O(1)

        \author tlucanti
    */
    {
        _deallocate();
    }

// --------------------------------- assigning ---------------------------------
    constexpr vector_base &operator =(const self_type &cpy)
    /**
        \brief copy operator
        \details operator destroys current content of vector (and deallocates
            memory) and makes deep copy of vector `cpy`
        \param cpy const reference to source container (with same type)
        \time O(size(self) + size(cpy))
        \memory O(size(cpy))

        \author tlucanti
    */
    {
        _deallocate();
        if (LIKELY(cpy.size() > 0))
        {
            _init(cpy.size());
            _copy(cpy._begin, cpy._end, _begin);
        }
    }

// -----------------------------------------------------------------------------
# if CPP11
    constexpr vector_base &operator =(self_type &&mv) noexcept
    /**
        \brief move operator
        \details operator destroys current content of vector (and deallocates
            memory) and makes its content from vector `mv` and then destroys
            container `mv`
        \param mv rvalue reference to source container
        \time O(size(self))
        \memory O(1)

        \author tlucanti
    */
    {
        _deallocate();
        _allocated = mv._allocated;
        _allocator = std::move(mv._allocator);
        _begin = mv._begin;
        _end = mv._end;
        mv._allocated = 0;
        mv._begin = nullptr;
        mv._end = nullptr;
        return *this;
    }

#endif /* CPP11 */

// -----------------------------------------------------------------------------
# if CPP11
    constexpr vector_base &operator =(init_list_type init_list)
    /**
        \brief initializer list copy operator
        \details operator destroys current content of vector (and deallocates
            memory) and makes deep copy of initializer list `init_list`
        \param init_list source container
        \time O(size(self) + size(init_list))
        \memory O(size(init_list))

        \author tlucanti
    */
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
    constexpr void assign(difference_type count, const_reference value)
    /**
        \brief assign member function
        \details this method destroys current content of vector (and deallocates
            memory) and then fills current vector with `count` copies of `value`
            values
        \param count number of filled copies
        \param value const reference to value to be coped to container
        \time O(size(self) + count)
        \memory O(count)

        \author tlucanti
    */
    {
        _deallocate();
        _init(count);
        _construct(_begin, count, value);
    }

// -----------------------------------------------------------------------------
    template <class input_it>
    constexpr void assign(NOT_INTEGRAL(input_it) first, input_it last)
    /**
        \brief assign member function
        \details this method destroys current content of vector (and deallocates
            memory) and then fills current vector with copies of valus in
            iterator range from `first` (included) to `last` (not included)
        \param first iterator to first element of copy range
        \param last iterator after last element of copy range
        \time O(size(self) + distance(first, last))
        \memory O(distance(first, last))

        \author tlucanti
    */
    {
        _deallocate();
        difference_type size = iterator::distance(first, last);
        _init(size);
        _copy(first, last, _begin);
    }

// -----------------------------------------------------------------------------
# if CPP11
    constexpr void assign(init_list_type init_list)
    /*

    */
    {
    // TODO: assign
        (void)init_list;
//# warning "IMPLEMENT FUNCTION"
        ABORT("FUNCTION NOT IMPLEMENTED", "");
    }
# endif /* CPP11 */

// =============================================================================
// ------------------------------ element access -------------------------------
    WUR constexpr allocator_type get_allocator() const noexcept
    /**
        \brief allocator getter member function
        \details function returns copy of container allocator
        \return copy of allocator
        \time O(1)
        \memory O(1)

        \author tlucanti
    */
    {
        return _allocator;
    }

    WUR constexpr reference at(difference_type pos)
    /**
        \brief element access function
        \details return elemnt in `pos` index if pos > 0 (counting from 0), or
            negative `pos` element from end of array (if pos < 0) (or
            `pos + size` element from begining (counting from 0)). If `pos` is
            out of range - std::out_of_range exception will be thrown
        \return const reference to element
        \time O(1)
        \memory O(1)

        \author tlucanti
    */
    {
        return _at(pos);
    }

    WUR constexpr const_reference at(difference_type pos) const
    /**
        \brief element access function for const container
        \details return elemnt in `pos` index if pos > 0 (counting from 0), or
            negative `pos` element from end of array (if pos < 0) (or
            `pos + size` element from begining (counting from 0)). If `pos` is
            out of range - std::out_of_range exception will be thrown
        \return const reference to element
        \time O(1)
        \memory O(1)

        \author tlucanti
    */
    {
        return _at(pos);
    }

    WUR constexpr reference operator[](difference_type pos)
    /**
        \brief element access operator
        \details (see .at() member function)
        \return non const reference to element
        \time O(1)
        \memory O(1)

        \author tlucanti
    */
    {
        return _at(pos);
    }

    WUR constexpr const_reference operator[](difference_type pos) const
    /**
        \brief element access operator for const container
        \details (see .at() const member function)
        \return const reference to element
        \time O(1)
        \memory O(1)

        \author tlucanti
    */
    {
        return _at(pos);
    }

    WUR constexpr reference front()
    /**
        \brief element access function
        \details method returns first element of array, if array is empty -
            std::out_of_range exception will be thrown
        \return non const reference to first element
        \time O(1)
        \memory O(1)

        \author tlucanti
    */
    {
        return _at(0);
    }

    WUR constexpr const_reference front() const
    /**
        \brief element access function for const container
        \details method returns first element of array, if array is empty -
            std::out_of_range exception will be thrown
        \return const reference to first element
        \time O(1)
        \memory O(1)

        \author tlucanti
    */
    {
        return _at(0);
    }

    WUR constexpr reference back()
    /**
        \brief element access function
        \details method returns last element of array, if array is empty -
            std::out_of_range exception will be thrown
        \return non const reference to last element
        \time O(1)
        \memory O(1)

        \author tlucanti
    */
    {
        return _at(size() - 1);
    }

    WUR constexpr const_reference back() const
    /**
        \brief element access function for const container
        \details method returns last element of array, if array is empty -
            std::out_of_range exception will be thrown
        \return const reference to last element
        \time O(1)
        \memory O(1)

        \author tlucanti
    */
    {
        return _at(size() - 1);
    }

    WUR constexpr pointer data()
    /**
        \brief data access function
        \details method returns pointer to begining of internal data container
        \return pointer to element storage array. If vector is empty - nullptr
            will returned
        \time O(1)
        \memory O(1)

        \author tlucanti
    */
    {
        return _begin;
    }

    WUR constexpr const_pointer data() const
    /**
        \brief data access function for const container
        \details (see .data() member function)
        \return const pointer to element storage array. If vector is empty -
            nullptr will returned
        \time O(1)
        \memory O(1)

        \author tlucanti
    */
    {
        return _begin;
    }

// =============================================================================
// --------------------------------- iterators ---------------------------------
    WUR constexpr iterator begin() noexcept
    /**
        \brief iterator access function
        \details method returns iterator to first element of vector
    */
    {
        return _iterator(_begin);
    }

    WUR constexpr const_iterator begin() const noexcept { return _iterator(_begin); }

    WUR constexpr iterator end() noexcept { return _iterator(_end); }
    WUR constexpr const_iterator end() const noexcept { return _iterator(_end); }

    WUR constexpr reverse_iterator rbegin() noexcept { return _riterator(_end - 1); }
    WUR constexpr const_reverse_iterator rbegin() const noexcept
    { return _riterator(_end - 1); }

    WUR constexpr reverse_iterator rend() noexcept { return _riterator(_begin - 1); }
    WUR constexpr const_reverse_iterator rend() const noexcept
    { return _riterator(_begin - 1); }

# if CPP11
    WUR constexpr const_iterator cbegin() const noexcept {return _iterator(_begin); }
    WUR constexpr const_iterator cend() const noexcept { return _iterator(_end); }

    WUR constexpr const_reverse_iterator crbegin() const noexcept
    { return _riterator(_end - 1); }
    WUR constexpr const_reverse_iterator crend() const noexcept
    { return _riterator(_begin - 1); }
# endif /* CPP11 */

// =============================================================================
// --------------------------------- capacity ----------------------------------
    WUR constexpr bool empty() const noexcept { return !size(); }
    WUR constexpr difference_type size() const noexcept { return _end - _begin; }
    WUR constexpr difference_type max_size() const noexcept { return std::numeric_limits<difference_type>::max(); }
    constexpr void reserve(difference_type capacity)
    {
        if (capacity && capacity >= _allocated)
            _alloc_more(capacity);
    }
    WUR constexpr difference_type capacity() const noexcept { return _allocated; }
# if CPP11
    constexpr void shrink_to_fit() {} // allocated size is always optimal
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
        pointer start = _insert(pos._ptr);
        _construct_at(start, value);
        return _iterator(start);
    }

// -----------------------------------------------------------------------------
//# if CPP11
//    constexpr iterator insert(iterator pos, rvalue_type value)
//    {
//        pointer start = _insert(pos._ptr);
//        *start = std::move(value);
//    }
//# endif /* CPP11 */

// -----------------------------------------------------------------------------
    constexpr void insert(iterator pos, difference_type count, const_reference value)
    {
        if (UNLIKELY(count <= 0))
            return ;
        pointer start = _insert(pos._ptr, count);
        _construct(start, count, value);
    }

// -----------------------------------------------------------------------------
    template <class input_it>
    constexpr void insert(iterator pos,
        NOT_INTEGRAL(input_it) first, input_it last)
    {
        pointer begin = _insert(pos._ptr, iterator::distance(first, last));
        _copy(first, last, begin);
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
        (void)pos;
        void *a[] = {args...};
//# warning "IMPLEMENT FUNCTION"
        ABORT("FUNCTION NOT IMPLEMENTED", "");
    }
# endif /* CPP11 */

// -----------------------------------------------------------------------------
    constexpr iterator erase(iterator pos)
    {
        if (pos == end())
            return end();
        pointer fin = _erase(pos._ptr);
        return _iterator(fin);
    }

// -----------------------------------------------------------------------------
    constexpr iterator erase(iterator first, iterator last)
    {
        difference_type count = iterator::distance(first, last);
        if (count <= 0)
            return last;
        _destroy(first._ptr, count);
        pointer fin = _erase(first._ptr, count);
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
//        ABORT("FUNCTION NOT IMPLEMENTED", "");
//    }
# endif /* CPP11 */

// -----------------------------------------------------------------------------
    constexpr void pop_back()
    {
        _destroy_at(_end - 1);
        _pop();
    }

// -----------------------------------------------------------------------------
    constexpr void resize(difference_type count)
    {
        if (UNLIKELY(count < 0))
            count = 0;
        difference_type size = this->size();
        if (count >= _allocated)
        {
            _alloc_more(count);
            difference_type new_elems = count - size;
            _construct(_begin + size, new_elems);
        }
        else
            _pop(_allocated - count);
        _end = _begin + count; // TODO: maybe this is redundant
    }

// -----------------------------------------------------------------------------
    constexpr void resize(difference_type count, const_reference value)
    {
        difference_type index = size();
        resize(count);
        for (difference_type size = this->size(); index < size; ++index)
            _begin[index] = value; // TODO: maybe use _allocator.construct() here
    }

// -----------------------------------------------------------------------------
    constexpr void swap(self_type &swp)
    {
        std::swap(_allocated, swp._allocated);
        std::swap(_allocator, swp._allocator);
        std::swap(_begin, swp._begin);
        std::swap(_end, swp._end);
        /* TODO: add swaps for other private/protected fields */
    }

// =============================================================================
// ----------------------------- internal methods ------------------------------
PRIVATE:
// ----------------------------- memory allocation -----------------------------
    WUR static constexpr difference_type _upper_bound_grid(difference_type req)
    {
#if CPP17
        constexpr const difference_type grid[] = {
#else
        const static difference_type grid[] = {
#endif
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
            3980154972736929792u, 6440026026380264448u
        };
        int l = 0;
		int r = 0;
		int mid = 0;

        if (req < 7u)
            return 7u;
        if (LIKELY(req <= 103682))
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
    constexpr void _construct_at(pointer ptr, const_reference val=value_type())
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
    constexpr void _construct(pointer start, difference_type cnt)
    {
        while (cnt-- > 0)
            _construct_at(start++);
    }

// -----------------------------------------------------------------------------
    constexpr void _construct(pointer start, difference_type cnt, const_reference val)
    {
        while (cnt-- > 0)
            _construct_at(start++, val);
    }

// -----------------------------------------------------------------------------
    constexpr void _destroy(pointer start, difference_type cnt)
    {
        while (cnt-- > 0)
            _destroy_at(start++);
    }

// -----------------------------------------------------------------------------
    constexpr void _init(difference_type req_size, bool do_construct=true)
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
    WUR constexpr pointer _allocate(difference_type alloc_size)
    {
        size_type alloc_size_unsigned = static_cast<size_type>(alloc_size);
        return _allocator.allocate(alloc_size_unsigned);
    }

// -----------------------------------------------------------------------------
    template <class forward_iterator_type>
    constexpr void _copy(IS_ITERATOR(forward_iterator_type) first,
        const forward_iterator_type &last, pointer dest)
    {
        while (first != last)
        {
            _construct_at(dest++, *first);
            ++first;
        }
    }

// -----------------------------------------------------------------------------
    constexpr void _copy(const_pointer first, const_pointer last, pointer dest)
    {
        difference_type diff = last - first;
        if (UNLIKELY(diff <= 0))
            return ;
        _copy(first, dest, diff);
    }

// -----------------------------------------------------------------------------
    constexpr void _copy(const_pointer src_ptr, pointer dst, difference_type cnt)
    {
        if (UNLIKELY(cnt <= 0))
            return ;
        pointer src = const_cast<pointer>(src_ptr);
        if (src > dst)
        {
            while (cnt-- > 0)
                _construct_at(dst++, *src++);
        }
        else if (dst > src)
        {
            dst += cnt;
            src += cnt;
            while (cnt-- > 0)
                _construct_at(--dst, *--src);
        }
    }

// -----------------------------------------------------------------------------
    constexpr void _deallocate(bool do_deallocate=true)
    {
        difference_type size = this->size();
        while (size-- > 0)
            _destroy_at(--_end);
        if (do_deallocate)
        {
            size_type allocated_unsigned = static_cast<size_type>(_allocated);
            _allocator.deallocate(_begin, allocated_unsigned);
            _allocated = 0;
            _begin = nullptr;
            _end = nullptr;
        }
    }

// -----------------------------------------------------------------------------
    constexpr void _alloc_more(difference_type least)
    {
        _allocated = _upper_bound_grid(least);
        _reallocate(_allocated);
    }

// -----------------------------------------------------------------------------
    constexpr void _shrink()
    {
        difference_type less_size = static_cast<difference_type>(
                static_cast<double>(_allocated) / (golden_ratio * golden_ratio)
        );
        if (size() < less_size and _allocated > 7)
            _alloc_more(size());
    }

// -----------------------------------------------------------------------------
    constexpr void _reallocate(difference_type new_size)
    {
        if (UNLIKELY(new_size <= 0))
        {
            _deallocate();
            return ;
        }
        try {
            pointer new_begin = _allocate(new_size);
            _copy(_begin, _end, new_begin);
            difference_type size = _end - _begin;
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
    WUR constexpr reference _at(difference_type pos) const
    {
        difference_type size = this->size();
        if (pos >= size or pos < -size)
            throw std::out_of_range("out of range");
        return _begin[(pos + size) % size];
    }

// -----------------------------------------------------------------------------
    WUR constexpr iterator _iterator(pointer ptr)
    {
        return iterator(ptr);
    }

// -----------------------------------------------------------------------------
    WUR constexpr reverse_iterator _riterator(pointer ptr)
    {
        return reverse_iterator(ptr);
    }

// -----------------------------------------------------------------------------
    WUR constexpr pointer _insert(pointer ptr, difference_type count=1)
    /*
        function inserts `count` empty places before ptr position in vector
        returns pointer to first empty place
    */
    {
        // here count cannot be negative, bc we checked it already
        difference_type index = ptr - _begin;
        difference_type move_cnt = _end - ptr;
        _append(count);
        _copy(_begin + index, _begin + index + count, move_cnt);
        _end += count;
        return _begin + index;
    }

// -----------------------------------------------------------------------------
    WUR constexpr pointer _erase(pointer ptr, difference_type count=1)
    {
        // here count cannot be negative, bc we checked it already
        difference_type index = ptr - _begin;
        _destroy(ptr, count);
        _copy(ptr, ptr + count, count);
        _shrink();
        return _begin + index;
    }

// -----------------------------------------------------------------------------
    constexpr void _append(difference_type count=1)
    {
        // here count cannot be negative, bc we checked it already
        if (size() + count >= _allocated)
            _alloc_more(size() + count);
    }

// -----------------------------------------------------------------------------
    constexpr void _pop(difference_type count=1)
    {
        // here count cannot be negative, bc we checked it already
        _shrink();
        _end -= count; // TODO: maybe uwe _allocator.destroy(_end) here
    }

// -----------------------------------------------------------------------------
};

TLU_NAMESPACE_END

#endif /* VECTOR_HPP */
