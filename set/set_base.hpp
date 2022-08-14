/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   set_base.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tlucanti <tlucanti@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/14 11:26:42 by tlucanti          #+#    #+#             */
/*   Updated: 2022/08/14 20:09:13 by tlucanti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SET_BASE_HPP
# define SET_BASE_HPP

# include "defs.hpp"
# include "rb_tree.hpp"
# include "rb_tree_iterator.hpp"
# include "pair_base.hpp"
# include <functional>
# include <memory>
# include <limits>

TLU_NAMESPACE_BEGIN


// =============================================================================
// --------------------------------- set_base ----------------------------------
template <
    class key_T,
    class cmp_T=std::less<key_T>,
    class allocator_T=std::allocator<key_T>,
    class pair_T=pair_base<int, int>
>
class set_base
{
// --------------------------------- typedefs ----------------------------------
public:
    typedef key_T                                       key_type;
    typedef key_T                                       value_type;
    typedef std::size_t                                 size_type;
    typedef std::ptrdiff_t                              difference_type;
    typedef cmp_T                                       key_compare;
    typedef cmp_T                                       value_compare;
    typedef allocator_T                                 allocator_type;
    typedef value_type                                  &reference;
    typedef const value_type                            &const_reference;
    typedef typename allocator_type::pointer            pointer;
    typedef typename allocator_type::const_pointer      const_pointer;

    typedef rb_tree_iterator<
        value_type,
        value_compare,
        allocator_type
    >                           iterator;
    typedef rb_tree_iterator<
        const value_type,
        value_compare,
        allocator_type
    >                           const_iterator;
    typedef rb_tree_reverse_iterator<
        value_type,
        value_compare,
        allocator_type
    >                           reverse_iterator;
    typedef rb_tree_reverse_iterator<
        const value_type,
        value_compare,
        allocator_type
    >                           const_reverse_iterator;

protected:
    typedef rb_tree<
        value_type,
        key_compare,
        allocator_type
    >                           tree_type;

    typedef typename
        iterator::rb_node       tree_node;
    typedef typename change_pair_type<
        pair_T,
        iterator,
        bool
    >::type                     insert_pair;
    typedef typename change_pair_type<
        pair_T,
        iterator,
        iterator
    >::type                     range_pair;
    typedef typename change_pair_type<
        pair_T,
        const_iterator,
        const_iterator
    >::type                     const_range_pair;

// ----------------------------- protected fields -----------------------------
protected:
    tree_type       _tree;

// =============================================================================
// ------------------------------ initialization -------------------------------
public:
    constexpr set_base() noexcept :
        _tree(tree_type())
    {}

// -----------------------------------------------------------------------------
    explicit constexpr set_base(
        const key_compare &comp,
        const allocator_type &alloc=allocator_type()
    ) noexcept
        : _tree(comp, alloc)
    {}

// -----------------------------------------------------------------------------
    template<class iterator_type>
    constexpr set_base(
        iterator_type first,
        iterator_type last,
        const key_compare &comp=key_compare(),
        const allocator_type &alloc=allocator_type()
    )
        : _tree(comp, alloc)
    {
        bool _;
        while (first != last)
        {
            _tree.insert(*first, _);
            ++first;
        }
    }

// -----------------------------------------------------------------------------
    constexpr set_base(const set_base &cmp)
        : _tree(cmp._tree)
    {}

// -----------------------------------------------------------------------------
#if CPP11
    constexpr set_base(set_base &&mv) noexcept
        : _tree(std::move(mv._tree))
    {}

// -----------------------------------------------------------------------------
    constexpr set_base(set_base &&mv, const allocator_type &alloc) noexcept
        : _tree(std::move(mv._tree))
    {}
#endif /* CPP11 */

// -------------------------------- destructors --------------------------------
    ~set_base() DEFAULT

// --------------------------------- assigning ---------------------------------
    constexpr set_base  &operator =(const set_base &cmp)
    {
        if (&cmp != this)
            _tree.assign(cmp._tree);
        return *this;
    }

// =============================================================================
// --------------------------------- iterators ---------------------------------
    constexpr iterator          begin() noexcept
    {
        return iterator(_tree.begin(), false);
    }

// -----------------------------------------------------------------------------
    constexpr const_iterator    begin() const noexcept
    {
        return const_iterator(_tree.begin(), false);
    }

// -----------------------------------------------------------------------------
    constexpr iterator          end() noexcept
    {
        return iterator(_tree.end(), true);
    }

// -----------------------------------------------------------------------------
    constexpr const_iterator    end() const noexcept
    {
        return const_iterator(_tree.end(), true);
    }

// -----------------------------------------------------------------------------
    constexpr reverse_iterator  rbegin() noexcept
    {
        return reverse_iterator(_tree.end(), false);
    }

// -----------------------------------------------------------------------------
    constexpr const_reverse_iterator    rbegin() const noexcept
    {
        return const_reverse_iterator(_tree.end(), false);
    }

// -----------------------------------------------------------------------------
    constexpr reverse_iterator  rend() noexcept
    {
        return reverse_iterator(_tree.begin(), true);
    }

// -----------------------------------------------------------------------------
    constexpr const_reverse_iterator    rend() const noexcept
    {
        return const_reverse_iterator(_tree.begin(), true);
    }

// =============================================================================
// --------------------------------- capacity ----------------------------------
    WUR constexpr bool      empty() const noexcept
    {
        return _tree.size() == 0;
    }

// -----------------------------------------------------------------------------
    constexpr size_type     size() const noexcept
    {
        return _tree.size();
    }

// -----------------------------------------------------------------------------
    constexpr size_type     max_size() const noexcept
    {
        return std::numeric_limits<size_type>::max();
    }

// =============================================================================
// --------------------------------- modifiers ---------------------------------
    constexpr void  clear()
    {
        _tree.destroy();
    }

// -----------------------------------------------------------------------------
    constexpr insert_pair   insert(const_reference value)
    {
        bool was_inserted;
        iterator node = iterator(_tree.insert(value, was_inserted));
        return insert_pair(node, was_inserted);
    }

// -----------------------------------------------------------------------------
    constexpr iterator      insert(iterator hint, const_reference value)
    {
        bool _;
        if (not hint.is_end())
            return iterator(_tree.insert(hint.get_node(), value, _), false);
        return iterator(_tree.insert(value), false);
    }

// -----------------------------------------------------------------------------
    template <class InputIt>
    constexpr void          insert(InputIt first, InputIt last)
    {
        bool _;
        while (first != last)
        {
            _tree.insert(*first, _);
            ++first;
        }
    }

// -----------------------------------------------------------------------------
    constexpr iterator      erase(iterator pos)
    {
        tree_node *ret = _tree.remove_node(pos.get_node());
        if (ret == _tree.end())
            return iterator(ret, true);
        return iterator(ret, false);
    }

// -----------------------------------------------------------------------------
    constexpr size_type     erase(const value_type &value)
    {
        size_t prev_size = _tree.size();
        _tree.remove(value);
        return prev_size - _tree.size();
    }

// -----------------------------------------------------------------------------
    template <class InputIt>
    constexpr void          erase(InputIt first, InputIt last)
    {
        while (first != last)
            _tree.remove(*first++);
    }

// -----------------------------------------------------------------------------
    constexpr void          swap(set_base &other) noexcept
    {
        _tree.swap(other._tree);
    }

// =============================================================================
// ---------------------------------- lookup -----------------------------------
    constexpr size_type     count(const_reference value) const noexcept
    {
        return _tree.find(value) != nullptr;
    }

// -----------------------------------------------------------------------------
    constexpr iterator      find(const_reference value) noexcept
    {
        tree_node *ret = _tree.find(value);
        if (ret == nullptr)
            return iterator(_tree.end(), true);
        return iterator(ret, false);
    }

// -----------------------------------------------------------------------------
    constexpr const_iterator    find(const_reference value) const noexcept
    {
        tree_node *ret = _tree.find(value);
        if (ret == nullptr)
            return const_iterator(_tree.end(), true);
        return const_iterator(ret, false);
    }

// -----------------------------------------------------------------------------
#if CPP20
    constexpr bool      contains(const_reference value) const noexcept
    {
        return count(value);
    }
#endif /* CPP20 */

// -----------------------------------------------------------------------------
    constexpr range_pair    equal_range(const_reference value) noexcept
    {
        return range_pair(lower_bound(value), upper_bound(value));
    }

// -----------------------------------------------------------------------------
    constexpr const_range_pair equal_range(const_reference value) const noexcept
    {
        return const_range_pair(lower_bound(value), upper_bound(value));
    }

// -----------------------------------------------------------------------------
    constexpr iterator  lower_bound(const_reference value) noexcept
    {
        tree_node *ret = _tree.lower_bound(value);
        if (ret == nullptr)
            return iterator(_tree.end(), true);
        return iterator(ret, false);
    }

// -----------------------------------------------------------------------------
    constexpr const_iterator   lower_bound(const_reference value) const noexcept
    {
        tree_node *ret = _tree.lower_bound(value);
        if (ret == nullptr)
            return const_iterator(_tree.end(), true);
        return const_iterator(ret, false);    }

// -----------------------------------------------------------------------------
    constexpr iterator  upper_bound(const_reference value) noexcept
    {
        tree_node *ret = _tree.upper_bound(value);
        if (ret == nullptr)
            return iterator(_tree.end(), true);
        return iterator(ret, false);
    }

// -----------------------------------------------------------------------------
    constexpr const_iterator   upper_bound(const_reference value) const noexcept
    {
        tree_node *ret = _tree.upper_bound(value);
        if (ret == nullptr)
            return const_iterator(_tree.end(), true);
        return const_iterator(ret, false);
    }

// =============================================================================
// --------------------------------- observers ---------------------------------
    constexpr key_compare       key_comp() const noexcept
    {
        return _tree.get_cmp();
    }

// -----------------------------------------------------------------------------
    constexpr value_compare     value_comp() const noexcept
    {
        return _tree.get_cmp();
    }

// -----------------------------------------------------------------------------
    constexpr allocator_type    get_allocator() const noexcept
    {
        return _tree.get_allocator();
    }

// =============================================================================
// -------------------------- lexicographical compare --------------------------
    friend constexpr bool   operator ==(const set_base &s1, const set_base &s2)
    {
        return s1._tree == s2._tree;
    }

// -----------------------------------------------------------------------------
    friend constexpr bool   operator !=(const set_base &s1, const set_base &s2)
    {
        return s1._tree != s2._tree;
    }

// -----------------------------------------------------------------------------
    friend constexpr bool   operator >(const set_base &s1, const set_base &s2)
    {
        return s1._tree > s2._tree;
    }

// -----------------------------------------------------------------------------
    friend constexpr bool   operator >=(const set_base &s1, const set_base &s2)
    {
        return s1._tree >= s2._tree;
    }

// -----------------------------------------------------------------------------
    friend constexpr bool   operator <(const set_base &s1, const set_base &s2)
    {
        return s1._tree < s2._tree;
    }

// -----------------------------------------------------------------------------
    friend constexpr bool   operator <=(const set_base &s1, const set_base &s2)
    {
        return s1._tree <= s2._tree;
    }

}; /* set_base */

TLU_NAMESPACE_END

#endif /* SET_BASE_HPP */
