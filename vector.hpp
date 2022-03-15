/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vector.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tlucanti <tlucanti@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/14 23:16:33 by tlucanti          #+#    #+#             */
/*   Updated: 2022/03/15 22:32:11 by tlucanti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef VECTOR_HPP
# define VECTOR_HPP

# include "defs.h"

# define __NOT_IMPLEMENTED \
#  warning "IMPLEMENT FUNCTION"\
	ABORT("FUNCTION NOT IMPLEMENTED")

_TLU_NAMESPACE_BEGIN

template <class type_T, class allocator_T=std::allocator>
class vector_base
{
public:
	typedef allocator_T									allocator_type;
	typedef allocator_traits<allocator_type>			allocator_traits;
	typedef allocator_traits::size_type					size_type;
protected:
	typedef type_T										value_type;
	typedef value_type &								reference;
	typedef const value_type &							const_reference;
	typedef typename allocator_traits::difference_type	difference_type;
	typedef typename allocator_traits::pointer			pointer;
	typedef typename allocator_traits::const_pointer	const_pointer;
	typedef int											iterator;
	typedef int											const_iterator;
	typedef int											reverse_iterator;
	typedef int											reverse_const_iterator;

// ----------------------------- protected fields ------------------------------
	size_type		_size;
	size_type		_allocated;
	allocator_type	_allocator;
	pointer			_begin;
	pointer			_end;


public:
// =============================================================================
// ------------------------------ initialization -------------------------------
	constexpr vector_base()
	{
		__NOT_IMPLEMENTED;
	}

// -----------------------------------------------------------------------------
	constexpr vector_base(const allocator_type &alloc)
	{
		__NOT_IMPLEMENTED;
	}

// -----------------------------------------------------------------------------
	constexpr vector_base(size_type n,
		const allocator_type &alloc=allocator_type()
	) :
		_size(size),
		_allocator(alloc),
		_begin(nullptr),
		_end(nullptr)
	{
		if (n > 0)
		{
			_begin = _allocate(size, _allocated);
			_end = _begin + _size;
		}
	}

// -----------------------------------------------------------------------------
	constexpr vector_base(size_type size,
		const_reference value=value_type(),
		const allocator_type &alloc=allocator_type()
	)
	{
		__NOT_IMPLEMENTED;
	}

// -----------------------------------------------------------------------------
	template <class input_it>
	constexpr vector_base(input_it first,
		input_it last,
		const allocator_type &alloc=allocator_type()
	)
	{
		__NOT_IMPLEMENTED;
	}

// -----------------------------------------------------------------------------
	constexpr vector_base(const vector &cpy, const allocator_type &alloc)
	{
		__NOT_IMPLEMENTED;
	}

// -------------------------------- destructors --------------------------------
	constexpr ~vector_base()
	{
		__NOT_IMPLEMENTED;
	}

// --------------------------------- assigning ---------------------------------
	constexpr vector_base &operator =(const vector_base &cpy)
	{
		__NOT_IMPLEMENTED;
	}

// -----------------------------------------------------------------------------
	constexpr void assign(size_type count, const_reference value)
	{
		__NOT_IMPLEMENTED;
	}

// -----------------------------------------------------------------------------
	template <typename input_it>
	constexpr assign(input_it first, input_it last)
	{
		__NOT_IMPLEMENTED;
	}

// =============================================================================
// ------------------------------ element accces -------------------------------
	__WUR constexpr allocator_type get_allocator() const noexcept { return allocator; }

	__WUR constexpr reference at(size_type pos) { return _at(pos); }
	__WUR constexpr const_reference at(size_type pos) const { return _at(pos); }

	__WUR constexpr reference operator[](size_type pos) { return _at(pos); }
	__WUR constexpr const_reference operator[](size_type pos) const {return _at(pos);}

	__WUR constexpr reference front() { return _at(0); }
	__WUR constexpr const_reference front() const { return _at(0); }

	__WUR constexpr reference back() { return _at(_size - 1); }
	__WUR constexpr const_reference back() const { return _at(_size - 1); }

	__WUR constexpr pointer data() { return _begin; }
	__WUR constexpr const_pointer data() const { return _begin; }

// =============================================================================
// --------------------------------- iterators ---------------------------------
	__WUR constexpr iterator begin() noexcept { return _iterator(_begin); }
	__WUR constexpr const_iterator begin() const noexcept { return _iterator(_begin); }

	__WUR constexpr const_iterator cbegin() const noexcept {return _iterator(_begin);}

	__WUR constexpr iterator end() noexcept { return _iterator(_end); }
	__WUR constexpr const_iterator end() const noexcept { return _iterator(_end); }

	__WUR constexpr const_iterator cend() const noexcept { return _iterator(_end); }

	__WUR constexpr reverse_iterator rbegin() noexcept { return _riterator(_end - 1);}
	__WUR constexpr const_reverse_iterator rbegin() const noexcept
	{ return _riterator(_end - 1); }

	__WUR constexpr const_reverse_iterator crbegin() const noexcept
	{ return _riterator(_end - 1)}

	__WUR constexpr reverse_iterator rend() noexcept { return _riterator(_begin - 1)}
	__WUR constexpr const_reverse_iterator rend() const noexcept
	{ return _riterator(_begin - 1); }

	__WUR constexpr const_reverse_iterator crend() const noexcept
	{ return _riterator(_begin - 1); }

// =============================================================================
// --------------------------------- capacity ----------------------------------
	__WUR constexpr bool empy() const noexcept { return !_size; }
	__WUR constexpr size_type size() const noexcept { return _size; }
	constexpr max_size() const noexcept
	{
		__NOT_IMPLEMENTED;
		/* not this */ return allocator_traits::max_size();
		/* last element in golden ratio array */
	}
	constexpr void reserve(size_type capacity)
	{
		__NOT_IMPLEMENTED;
	}



// =============================================================================
// ----------------------------- internal methods ------------------------------
private:
// ----------------------------- memory allocation -----------------------------
	pointer _allocate(size_type req_size, size_type &alloc_size)
	{
		__NOT_IMPLEMENTED;
	}
// -----------------------------------------------------------------------------
	void _construct(pointer start, size_type construct_size)
	{
		__NOT_IMPLEMENTED;
	}
// -----------------------------------------------------------------------------
	void _deallocate(pointer start)
	{
		__NOT_IMPLEMENTED;
	}
// -----------------------------------------------------------------------------
	void _destruct(pointer start, size_type destruct_size)
	{
		__NOT_IMPLEMENTED;
	}

// ------------------------------ element accces -------------------------------
	reference _at(size_type pos) const
	{
		if (pos >= _size or pos < -_size)
			_out_of_range();
		return _begin[(pos + _size) % _size];
	}
};

_TLU_NAMESPACE_END

#endif VECTOR_HPP
