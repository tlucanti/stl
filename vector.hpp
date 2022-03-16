/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vector.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tlucanti <tlucanti@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/14 23:16:33 by tlucanti          #+#    #+#             */
/*   Updated: 2022/03/16 16:25:28 by tlucanti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef VECTOR_HPP
# define VECTOR_HPP

# include "defs.h"
# include <iterator>

# define __NOT_IMPLEMENTED \
#  warning "IMPLEMENT FUNCTION"\
	__ABORT("FUNCTION NOT IMPLEMENTED", "")

_TLU_NAMESPACE_BEGIN

# define __wrap_iterator_bool_operator_macro(__op) constexpr bool operator op( \
	const self_type &cmp) const { return _ptr op cmp._ptr; }

template <class type_T>
class wrap_iterator : std::iterator<std::random_access_iterator_tag, type_T>
{
// --------------------------------- typedefs ----------------------------------
public:
	typedef type_T										iterator_type;
	typedef std::iterator_traits<iterator_type>			iterator_traits;
	typedef typename iterator_traits::iterator_category	iterator_category;
	typedef typename iterator_traits::iterator_value	value_type;
	typedef typename iterator_traits::difference_type	difference_type;
	typedef typename iterator_traits::pointer			pointer;
	typedef typename iterator_traits::reference			reference;

private:
	typedef wrap_iterator<iterator_type>				self_type;

// ------------------------------ private fields -------------------------------
	pointer	_ptr;


// =============================================================================
// ------------------------------ initialization -------------------------------
private:
	constexpr wrap_iterator(pointer ptr) : _ptr(ptr) {}
public:
	constexpr wrap_iterator() : _ptr(nullptr) {}
	constexpr wrap_iterator(const self_type &cpy) : _ptr(cpy._ptr) {}

# if CPP11
	constexpr wrap_iterator(self_type &&mv)
		: _ptr(mv._ptr)
	{
		mv._ptr = nullptr;	
	}
# endif /* CPP11 */

// =============================================================================
// --------------------------------- operators ---------------------------------
	__wrap_iterator_operator_macro(==);
	__wrap_iterator_operator_macro(!=);
	__wrap_iterator_operator_macro(> );
	__wrap_iterator_operator_macro(>=);
	__wrap_iterator_operator_macro(< );
	__wrap_iterator_operator_macro(<=);

# if CPP20
	constexpr bool operator <=>(const self_type &cmp)
	{
		__NOT_IMPLEMENTED;
	}
# endif /* CPP20 */


};

template <class type_T, class allocator_T=std::allocator>
class vector_base
{
// --------------------------------- typedefs ----------------------------------
public:
	typedef allocator_T									allocator_type;
	typedef std::allocator_traits<allocator_type>		allocator_traits;
	typedef allocator_traits::size_type					size_type;
private:
	typedef type_T										value_type;
	typedef vector_base<value_type>						self_type;
	typedef value_type &								reference;
	typedef const value_type &							const_reference;
	typedef value_type &&								rvalue_type;
	typedef typename allocator_traits::difference_type	difference_type;
	typedef typename allocator_traits::pointer			pointer;
	typedef typename allocator_traits::const_pointer	const_pointer;
	typedef int											iterator;
	typedef int											const_iterator;
	typedef int											reverse_iterator;
	typedef int											reverse_const_iterator;
	typedef std::initializer_list<value_type>			init_list_type;

// ----------------------------- protected fields ------------------------------
protected:
	self_type	&c;

// ------------------------------ private fields -------------------------------
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
		c(*this),
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
	constexpr vector_base(const self_type &cpy, const allocator_type &alloc)
	{
		__NOT_IMPLEMENTED;
	}

// -----------------------------------------------------------------------------
# if __CPP11
	constexpr vector_base(self_type &&mv,
		const allocator_type &alloc=allocator_type()
	)
	{
		__NOT_IMPLEMENTED;
	}

// -----------------------------------------------------------------------------
	constexpr vector_base(init_list_type init,
		const allocator_type &alloc=allocator_type()
	)
	{
		__NOT_IMPLEMENTED;
	}
# endif /* CPP11 */

// -------------------------------- destructors --------------------------------
	constexpr ~vector_base()
	{
		__NOT_IMPLEMENTED;
	}

// --------------------------------- assigning ---------------------------------
	constexpr vector_base &operator =(const self_type &cpy)
	{
		__NOT_IMPLEMENTED;
	}

// -----------------------------------------------------------------------------
# if __CPP11
	constexpr vector_base &operator =(self_type &&mv)
	{
		__NOT_IMPLEMENTED;
	}

// -----------------------------------------------------------------------------
	constexpr vector_base &operator =(init_list_type init_list)
	{
		__NOT_IMPLEMENTED;
		/* use for (auto &&value : init_list) */
	}
#endif /* CPP11 */

// -----------------------------------------------------------------------------
	constexpr void assign(size_type count, const_reference value)
	{
		__NOT_IMPLEMENTED;
	}

// -----------------------------------------------------------------------------
	template <class input_it>
	constexpr assign(input_it first, input_it last)
	{
		__NOT_IMPLEMENTED;
	}

// -----------------------------------------------------------------------------
# if CPP11
	constexpr void assign(init_list_type init_list)
	{
		__NOT_IMPLEMENTED;
	}
# endif /* CPP11 */

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

	__WUR constexpr iterator end() noexcept { return _iterator(_end); }
	__WUR constexpr const_iterator end() const noexcept { return _iterator(_end); }

	__WUR constexpr reverse_iterator rbegin() noexcept { return _riterator(_end - 1);}
	__WUR constexpr const_reverse_iterator rbegin() const noexcept
	{ return _riterator(_end - 1); }

	__WUR constexpr reverse_iterator rend() noexcept { return _riterator(_begin - 1)}
	__WUR constexpr const_reverse_iterator rend() const noexcept
	{ return _riterator(_begin - 1); }

# if CPP11
	__WUR constexpr const_iterator cbegin() const noexcept {return _iterator(_begin);}
	__WUR constexpr const_iterator cend() const noexcept { return _iterator(_end); }

	__WUR constexpr const_reverse_iterator crbegin() const noexcept
	{ return _riterator(_end - 1)}
	__WUR constexpr const_reverse_iterator crend() const noexcept
	{ return _riterator(_begin - 1); }
# endif /* CPP11 */

// =============================================================================
// --------------------------------- capacity ----------------------------------
	__WUR constexpr bool empy() const noexcept { return !_size; }
	__WUR constexpr size_type size() const noexcept { return _size; }
	__WUR constexpr size_type max_size() const noexcept
	{
		__NOT_IMPLEMENTED;
		/* not this */ return allocator_traits::max_size();
		/* last element in golden ratio array */
	}
	constexpr void reserve(size_type capacity)
	{
		__NOT_IMPLEMENTED;
	}

# if CPP11
	constexpr void shriknk_to_fit()
	{
		__NOT_IMPLEMENTED;
	}
# endif /* CPP11 */

// =============================================================================
// --------------------------------- modifiers ---------------------------------
	constexpr void clear() noexcept
	{
		__NOT_IMPLEMENTED;
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
		__NOT_IMPLEMENTED;
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
	template <class arg_type>
	constexpr reference emplace_back(arg_type && ... args)
	{
		__NOT_IMPLEMENTED;
	}
# endif /* CPP11 */

// -----------------------------------------------------------------------------
	constexpr void pop_back();
	{
		_pop();
	}

// -----------------------------------------------------------------------------
	constexpr void resize(size_type count, const_reference value=value_type())
	{
		if (_size > count)
			_pop(_size - count);
		else
			_append(value, count - size);
	}

// -----------------------------------------------------------------------------
	constexpr void swap(self_type &swp)
	{
		std::swap(_size, swp._size);
		std::swap(_allocated, swp._allocated);
		std::swap(_allocator, swp._allocator);
		std::swap(_begin, swp._begin);
		std::swap(_end, swp._end);
		__NOT_IMPLEMENTED;
		/* add swaps for other private/protected fields */
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

// -----------------------------------------------------------------------------
	iterator _iterator(const_pointer ptr)
	{
		__NOT_IMPLEMENTED;
	}

// -----------------------------------------------------------------------------
	reverse_iterator _riterator(const_pointer ptr)
	{
		__NOT_IMPLEMENTED;
	}

// -----------------------------------------------------------------------------
	pointer _insert(const_pointer ptr, size_type count=1)
	{
		__NOT_IMPLEMENTED;
		// if ptr == _end -> call _append
	}

// -----------------------------------------------------------------------------
	pointer _erase(const_pointer ptr, size_type count=1)
	{
		__NOT_IMPLEMENTED;
		// if ptr == _end -> call _pop
	}

// -----------------------------------------------------------------------------
	void _append(const_reference value, size_type count=1)
	{
		__NOT_IMPLEMENTED;
	}

// -----------------------------------------------------------------------------
	void _pop(size_type count=1)
	{
		__NOT_IMPLEMENTED;
	}
};

_TLU_NAMESPACE_END

#endif VECTOR_HPP
