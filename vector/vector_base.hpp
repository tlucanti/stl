/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vector_base.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tlucanti <tlucanti@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/14 23:16:33 by tlucanti          #+#    #+#             */
/*   Updated: 2022/08/14 20:32:39 by tlucanti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef VECTOR_BASE_HPP
# define VECTOR_BASE_HPP

# include <limits>
# include <memory>
# include <cmath>
# include "defs.hpp"
# include "type_traits.hpp"
# include "wrap_iterator.hpp"
# include "utils.hpp"

TLU_NAMESPACE_BEGIN


// =============================================================================
// -------------------------------- vector_base --------------------------------
template <
    class type_T,
    class allocator_T=std::allocator<type_T>
>
class vector_base
{
// --------------------------------- typedefs ----------------------------------
public:
    typedef allocator_T                                 allocator_type;
    typedef std::ptrdiff_t                              size_type;

    typedef type_T                                      value_type;
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
    typedef std::initializer_list<value_type>           init_list_type;
# endif /* CPP11 */

// ------------------------------ private fields -------------------------------
private:
    difference_type _allocated;
    allocator_type  _allocator;
    pointer         _begin;
    pointer         _end;

#if CPP17
    constexpr const static double golden_ratio = 1.618033988749895;
#else
    const static double golden_ratio;
#endif

// =============================================================================
// ------------------------------ initialization -------------------------------
public:
    constexpr vector_base() noexcept :
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
    constexpr explicit vector_base(const allocator_type &alloc) noexcept :
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
    constexpr vector_base(const vector_base &cpy,
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
    constexpr vector_base(vector_base &&mv,
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
        if (LIKELY(init.size() > 0))
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
    constexpr vector_base   &operator =(const vector_base &cpy)
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
        return *this;
    }

// -----------------------------------------------------------------------------
# if CPP11
    constexpr vector_base   &operator =(vector_base &&mv) noexcept
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
    constexpr vector_base   &operator =(init_list_type init_list)
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
    constexpr void      assign(difference_type count, const_reference value)
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
        _init(count, false);
        _construct(_begin, count, value);
    }

// -----------------------------------------------------------------------------
    template <class input_it>
    constexpr void      assign(NOT_INTEGRAL(input_it) first, input_it last)
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
        _init(size, false);
        _copy(first, last, _begin);
    }

// =============================================================================
// ------------------------------ element access -------------------------------
    WUR constexpr allocator_type    get_allocator() const noexcept
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

// -----------------------------------------------------------------------------
    WUR constexpr reference         at(difference_type pos)
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

// -----------------------------------------------------------------------------
    WUR constexpr const_reference   at(difference_type pos) const
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

// -----------------------------------------------------------------------------
    WUR constexpr reference         operator[](difference_type pos)
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

// -----------------------------------------------------------------------------
    WUR constexpr const_reference   operator[](difference_type pos) const
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

// -----------------------------------------------------------------------------
    WUR constexpr reference         front()
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

// -----------------------------------------------------------------------------
    WUR constexpr const_reference   front() const
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

// -----------------------------------------------------------------------------
    WUR constexpr reference         back()
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

// -----------------------------------------------------------------------------
    WUR constexpr const_reference   back() const
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

// -----------------------------------------------------------------------------
    WUR constexpr pointer           data() noexcept
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

// -----------------------------------------------------------------------------
    WUR constexpr const_pointer     data() const noexcept
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
    WUR constexpr iterator          begin() noexcept
    /**
        \brief iterator access function
        \details method returns iterator to first element of vector
    */
    {
        return iterator(_begin);
    }

// -----------------------------------------------------------------------------
    WUR constexpr const_iterator    begin() const noexcept
    {
        return const_iterator(_begin);
    }

// -----------------------------------------------------------------------------
    WUR constexpr iterator          end() noexcept
    {
        return iterator(_end);
    }

// -----------------------------------------------------------------------------
    WUR constexpr const_iterator    end() const noexcept
    {
        return const_iterator(_end);
    }

// -----------------------------------------------------------------------------
    WUR constexpr reverse_iterator  rbegin() noexcept
    {
        return reverse_iterator(_end - 1);
    }

// -----------------------------------------------------------------------------
    WUR constexpr const_reverse_iterator    rbegin() const noexcept
    {
        return const_reverse_iterator(_end - 1);
    }

// -----------------------------------------------------------------------------
    WUR constexpr reverse_iterator          rend() noexcept
    {
        return reverse_iterator(_begin - 1);
    }

// -----------------------------------------------------------------------------
    WUR constexpr const_reverse_iterator    rend() const noexcept
    {
        return const_reverse_iterator(_begin - 1);
    }

// -----------------------------------------------------------------------------
# if CPP11
    WUR constexpr const_iterator            cbegin() const noexcept
    {
        return const_iterator(_begin);
    }

// -----------------------------------------------------------------------------
    WUR constexpr const_iterator            cend() const noexcept
    {
        return const_iterator(_end);
    }

// -----------------------------------------------------------------------------
    WUR constexpr const_reverse_iterator    crbegin() const noexcept
    {
        return const_reverse_iterator(_end - 1);
    }

// -----------------------------------------------------------------------------
    WUR constexpr const_reverse_iterator    crend() const noexcept
    {
        return const_reverse_iterator(_begin - 1);
    }
# endif /* CPP11 */

// =============================================================================
// --------------------------------- capacity ----------------------------------
    WUR constexpr bool      empty() const noexcept
    {
        return !size();
    }

// -----------------------------------------------------------------------------
    WUR constexpr difference_type   size() const noexcept
    {
        return _end - _begin;
    }

// -----------------------------------------------------------------------------
    WUR constexpr difference_type   max_size() const noexcept
    {
        return std::numeric_limits<difference_type>::max();
    }

// -----------------------------------------------------------------------------
    constexpr void      reserve(difference_type capacity)
    {
        if (capacity && capacity >= _allocated)
            _alloc_more(capacity);
    }

// -----------------------------------------------------------------------------
    WUR constexpr difference_type   capacity() const noexcept
    {
        return _allocated;
    }

// -----------------------------------------------------------------------------
# if CPP11
    constexpr   void shrink_to_fit() const noexcept
    /*
        allocated size is always optimal
    */
    {}
# endif /* CPP11 */

// =============================================================================
// --------------------------------- modifiers ---------------------------------
    constexpr   void clear()
    {
        _deallocate(false);
    }

// -----------------------------------------------------------------------------
    constexpr iterator  insert(iterator pos, const_reference value)
    {
        if (UNLIKELY(_begin == nullptr))
        {
            _init(1, false);
            _construct_at(_begin, value);
            return begin();
        }
        pointer start = _insert(pos.get_ptr());
        *start = value;
        return iterator(start);
    }

// -----------------------------------------------------------------------------
    constexpr void      insert(
            iterator pos,
            difference_type count,
            const_reference value
        )
    {
        if (UNLIKELY(count <= 0))
            return ;
        if (UNLIKELY(_begin == nullptr))
        {
            _init(count, false);
            _construct(_begin, count, value);
            return ;
        }
        pointer start = _insert(pos.get_ptr(), count);
        difference_type movable = min(_end - pos.get_ptr(), count);
        for (difference_type i=0; i < movable; ++i)
            start[i] = value;
        for (difference_type i=movable; i < count; ++i)
            _construct_at(start, value);
    }

// -----------------------------------------------------------------------------
    template <class input_it>
    constexpr void      insert(
            iterator pos,
            NOT_INTEGRAL(input_it) first,
            input_it last
        )
    {
        if (UNLIKELY(_begin == nullptr))
        {
            assign(first, last);
            return ;
        }
        difference_type size = iterator::distance(first, last);
        difference_type movable = min(_end - pos.get_ptr(), size);
        pointer begin = _insert(pos.get_ptr(), size);
        input_it non_movable = _move(first, begin, movable);
        _copy(non_movable, last, begin + movable);
    }

// -----------------------------------------------------------------------------
# if CPP11
    constexpr iterator  insert(iterator pos, init_list_type init_list)
    {
        iterator ret = iterator(pos);
        pointer start = _insert(
            pos._ptr,
            static_cast<difference_type>(init_list.size())
        );
        _copy(init_list.begin(), init_list.end(), start);
        return ++ret;
    }
# endif /* CPP11 */

// -----------------------------------------------------------------------------
    constexpr iterator  erase(iterator pos)
    {
        if (pos == end())
            return end();
        pointer fin = _erase(pos.get_ptr());
        return iterator(fin);
    }

// -----------------------------------------------------------------------------
    constexpr iterator  erase(iterator first, iterator last)
    {
        difference_type count = iterator::distance(first, last);
        if (count <= 0)
            return last;
        pointer fin = _erase(first.get_ptr(), count);
        return iterator(fin);
    }

// -----------------------------------------------------------------------------
    constexpr void      push_back(const_reference value)
    {
        _append();
        _construct_at(_end++, value);
    }

// -----------------------------------------------------------------------------
    constexpr void      pop_back()
    {
        _destroy_at(_end - 1);
        _pop();
    }

// -----------------------------------------------------------------------------
    constexpr void      resize(difference_type count)
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
        _end = _begin + count;
    }

// -----------------------------------------------------------------------------
    constexpr void      resize(difference_type count, const_reference value)
    {
        difference_type index = size();
        resize(count);
        for (difference_type size = this->size(); index < size; ++index)
            _begin[index] = value; // TODO: maybe use _allocator.construct() here
    }

// -----------------------------------------------------------------------------
    constexpr void      swap(vector_base &swp)
    {
        std::swap(_allocated, swp._allocated);
        std::swap(_allocator, swp._allocator);
        std::swap(_begin, swp._begin);
        std::swap(_end, swp._end);
    }

// =============================================================================
// ----------------------------- internal methods ------------------------------
private:
// ----------------------------- memory allocation -----------------------------
    WUR static constexpr difference_type    _upper_bound_grid(
            difference_type req
        )
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
		int r = 20;

        if (req < 7u)
            return 7u;
        if (UNLIKELY(req > 103682))
        {
            l = 21;
            r = 88;
        }
        while (r - l > 1)
        {
            int mid = (l + r) / 2;
            if (req >= grid[mid])
                l = (l + r) / 2;
            else
                r = (l + r) / 2;
        }
        return grid[r];
    }

// -----------------------------------------------------------------------------
    constexpr void  _construct_at(pointer ptr)
    {
        _allocator.construct(ptr, value_type());
    }

// -----------------------------------------------------------------------------
    constexpr void  _construct_at(pointer ptr, const_reference val)
    {
        _allocator.construct(ptr, val);
    }

// -----------------------------------------------------------------------------
    constexpr void  _destroy_at(pointer ptr)
    {
        _allocator.destroy(ptr);
    }

// -----------------------------------------------------------------------------
    constexpr void  _construct(
            pointer start,
            difference_type cnt,
            const_reference val=value_type()
        )
    {
        while (cnt-- > 0)
            _construct_at(start++, val);
    }

// -----------------------------------------------------------------------------
    constexpr void  _destroy(pointer start, difference_type cnt)
    {
        while (cnt-- > 0)
            _destroy_at(start++);
    }

// -----------------------------------------------------------------------------
    constexpr void  _init(difference_type req_size, bool do_construct=true)
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
    WUR constexpr pointer   _allocate(difference_type alloc_size)
    {
        AUTO(std::size_t) alloc_size_unsigned = static_cast<std::size_t>(alloc_size);
#ifdef __DEBUG
        pointer P_ptr = _allocator.allocate(alloc_size_unsigned);
        memset(static_cast<void *>(P_ptr), 0, alloc_size_unsigned);
        return P_ptr;
#else
        return _allocator.allocate(alloc_size_unsigned);
#endif
    }

// -----------------------------------------------------------------------------
    template <class forward_iterator_type>
    constexpr void      _copy(
            IS_ITERATOR(forward_iterator_type) first,
            const forward_iterator_type &last, pointer dest
        )
    {
        while (first != last)
        {
            _construct_at(dest, *first);
            ++dest;
            ++first;
        }
    }

// -----------------------------------------------------------------------------
    constexpr void      _copy(const_pointer first, const_pointer last, pointer dest)
    {
        difference_type diff = last - first;
        if (UNLIKELY(diff <= 0))
            return ;
        _copy(first, dest, diff);
    }

// -----------------------------------------------------------------------------
    constexpr void      _copy(
            const_pointer src_ptr,
            pointer dst,
            difference_type cnt
        )
    {
        if (UNLIKELY(cnt <= 0))
            return ;
        AUTO(pointer) src = const_cast<pointer>(src_ptr);
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
    constexpr void      _move(pointer src, pointer dst, difference_type cnt)
    {
        if (UNLIKELY(cnt <= 0))
            return ;
        if (src > dst)
        {
            while (cnt-- > 0)
                *dst++ = *src++;
        }
        else if (dst > src)
        {
            dst += cnt;
            src += cnt;
            while (cnt-- > 0)
                *(--dst) = *(--src);
        }
    }

// -----------------------------------------------------------------------------
    template <typename Iter_T>
    WUR constexpr Iter_T    _move(Iter_T src, pointer dst, difference_type cnt)
    {
        while (cnt-- > 0)
        {
            *dst++ = *src;
            ++src;
        }
        return src;
    }

// -----------------------------------------------------------------------------
    constexpr void      _deallocate(bool do_deallocate=true)
    {
        difference_type size = this->size();
        while (size-- > 0)
            _destroy_at(--_end);
        if (do_deallocate && _allocated)
        {
            AUTO(std::size_t) allocated_unsigned = static_cast<std::size_t>(
                _allocated
            );
            _allocator.deallocate(_begin, allocated_unsigned);
            _allocated = 0;
            _begin = nullptr;
            _end = nullptr;
        }
    }

// -----------------------------------------------------------------------------
    constexpr void      _alloc_more(difference_type least)
    {
        _allocated = _upper_bound_grid(least);
        _reallocate(_allocated);
    }

// -----------------------------------------------------------------------------
    constexpr void      _shrink()
    {
        AUTO(difference_type) less_size = lround(
            static_cast<double>(_allocated)
            / (golden_ratio * golden_ratio) + 0.5
        );
        if (size() < less_size and _allocated > 7)
            _alloc_more(size());
    }

// -----------------------------------------------------------------------------
    constexpr void      _reallocate(difference_type new_size)
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
    WUR constexpr reference     _at(difference_type pos) const
    {
        difference_type size = this->size();
        if (pos >= size or pos < -size)
            throw std::out_of_range("out of range");
        return _begin[(pos + size) % size];
    }

// -----------------------------------------------------------------------------
    WUR constexpr pointer       _insert(pointer ptr, difference_type count=1)
    /*
        function inserts `count` empty places before ptr position in vector
        returns pointer to first empty place
    */
    {
        // here count cannot be negative, bc we checked it already
        difference_type index = ptr - _begin;
        _append(count);
        ptr = _begin + index;
        difference_type move_cnt = _end - ptr - count;
        if (move_cnt <= 0)
            _copy(ptr, ptr + count, _end - ptr);
        else
        {
            _copy(ptr + move_cnt, _end, count);
            _move(ptr, ptr + count, move_cnt);
        }
        _end += count;
        return ptr;
    }

// -----------------------------------------------------------------------------
    WUR constexpr pointer       _erase(pointer ptr, difference_type count=1)
    {
        // here count cannot be negative, bc we checked it already
        difference_type index = ptr - _begin;
        difference_type move_cnt = _end - ptr - count;
        _move(ptr + count, ptr, move_cnt);
        _destroy(_end - count, count);
        _end -= count;
        _shrink();
        return _begin + index;
    }

// -----------------------------------------------------------------------------
    constexpr void      _append(difference_type count=1)
    {
        // here count cannot be negative, bc we checked it already
        if (size() + count >= _allocated)
            _alloc_more(size() + count);
    }

// -----------------------------------------------------------------------------
    constexpr void      _pop(difference_type count=1)
    {
        // here count cannot be negative, bc we checked it already
        _end -= count;
        _shrink();
    }

}; /* vector_base */

// ----------------------------------------------------------------------------
# if PRECPP17
template <class type_T, class allocator_T>
const double vector_base<type_T, allocator_T>::golden_ratio = 1.618033988749895;
# endif /* PRECPP17 */

// ----------------------------------------------------------------------------
TLU_NAMESPACE_HIDDEN_BEGIN

template <class type_T, class allocator_T>
constexpr int   vector_base_compare(
        const TLU_NAMESPACE::vector_base<type_T, allocator_T> &lhs,
        const TLU_NAMESPACE::vector_base<type_T, allocator_T> &rhs
    )
{

    if (lhs.size() < rhs.size())
        return -1;
    else if (lhs.size() > rhs.size())
        return 1;
    else
        return TLU_NAMESPACE::vector_base<type_T>::iterator::compare(
            lhs.begin(), rhs.begin(), lhs.size()
        );
}
TLU_NAMESPACE_HIDDEN_END

TLU_NAMESPACE_END

// =============================================================================
// --------------------------- non-member operators ----------------------------
template <class type_T, class allocator_T>
constexpr bool      operator ==(
        const TLU_NAMESPACE::vector_base<type_T, allocator_T> &lhs,
        const TLU_NAMESPACE::vector_base<type_T, allocator_T> &rhs
    )
{
    return TLU_NAMESPACE::TLU_NAMESPACE_HIDDEN::vector_base_compare<type_T>(
        lhs, rhs) == 0;
}

// ----------------------------------------------------------------------------
template <class type_T, class allocator_T>
constexpr bool      operator !=(
        const TLU_NAMESPACE::vector_base<type_T, allocator_T> &lhs,
        const TLU_NAMESPACE::vector_base<type_T, allocator_T> &rhs
    )
{
    return TLU_NAMESPACE::TLU_NAMESPACE_HIDDEN::vector_base_compare<type_T>(
        lhs, rhs) != 0;
}

// ----------------------------------------------------------------------------
template <class type_T, class allocator_T>
constexpr bool      operator >(
        const TLU_NAMESPACE::vector_base<type_T, allocator_T> &lhs,
        const TLU_NAMESPACE::vector_base<type_T, allocator_T> &rhs
    )
{
    return TLU_NAMESPACE::TLU_NAMESPACE_HIDDEN::vector_base_compare<type_T>(
        lhs, rhs) > 0;
}

// ----------------------------------------------------------------------------
template <class type_T, class allocator_T>
constexpr bool      operator >=(
        const TLU_NAMESPACE::vector_base<type_T, allocator_T> &lhs,
        const TLU_NAMESPACE::vector_base<type_T, allocator_T> &rhs
    )
{
    return TLU_NAMESPACE::TLU_NAMESPACE_HIDDEN::vector_base_compare<type_T>(
        lhs, rhs) >= 0;
}

// ----------------------------------------------------------------------------
template <class type_T, class allocator_T>
constexpr bool      operator <(
        const TLU_NAMESPACE::vector_base<type_T, allocator_T> &lhs,
        const TLU_NAMESPACE::vector_base<type_T, allocator_T> &rhs
    )
{
    return TLU_NAMESPACE::TLU_NAMESPACE_HIDDEN::vector_base_compare<type_T>(
        lhs, rhs) < 0;
}

// ----------------------------------------------------------------------------
template <class type_T, class allocator_T>
constexpr bool      operator <=(
        const TLU_NAMESPACE::vector_base<type_T, allocator_T> &lhs,
        const TLU_NAMESPACE::vector_base<type_T, allocator_T> &rhs
    )
{
    return TLU_NAMESPACE::TLU_NAMESPACE_HIDDEN::vector_base_compare<type_T>(
        lhs, rhs) <= 0;
}

#endif /* VECTOR_BASE_HPP */
