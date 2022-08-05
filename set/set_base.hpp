
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

template <
        class key_T,
        class cmp_T=std::less<key_T>,
        class allocator_T=std::allocator<key_T>,
        class pair_T=pair_base<int, int>
    >
class set_base
{
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

    typedef rb_tree_iterator<value_type, value_compare, allocator_type>                iterator;
    typedef rb_tree_iterator<const value_type, value_compare, allocator_type>          const_iterator;
    typedef rb_tree_reverse_iterator<value_type, value_compare, allocator_type>        reverse_iterator;
    typedef rb_tree_reverse_iterator<const value_type, value_compare, allocator_type>  const_reverse_iterator;

protected:
    typedef rb_tree<
        value_type,
        key_compare,
        allocator_type
    >   tree_type;
    typedef typename iterator::rb_node                                  tree_node;
    typedef typename change_pair_type<pair_T, iterator, bool>::type     insert_pair;
    typedef typename change_pair_type<pair_T, iterator, iterator>::type range_pair;
    typedef typename change_pair_type<pair_T, const_iterator, const_iterator>::type const_range_pair;

    tree_type       _tree;

public:
    // ============================================================================
// ------------------------------- constructors/destructors
    set_base() :
        _tree(tree_type())
    {}

    explicit set_base(
        const key_compare &comp,
        const allocator_type &alloc=allocator_type()
    ) :
        _tree(comp, alloc)
    {}

    template<class iterator_type>
    set_base(
        iterator_type first,
        iterator_type last,
        const key_compare &comp=key_compare(),
        const allocator_type &alloc=allocator_type()
    ) :
        _tree(comp, alloc)
    {
        bool _;
        while (first != last)
        {
            _tree.insert(*first, _);
            ++first;
        }
    }

    set_base(const set_base &cmp) :
        _tree(cmp._tree)
    {}

#if CPP11
    set_base(set_base &&mv) :
        _tree(std::move(mv._tree))
    {}

    set_base(set_base &&mv, const allocator_type &alloc)
        _tree(std::move(mv._tree))
    {}
#endif /* CPP11 */

    ~set_base() DEFAULT

    set_base &operator =(const set_base &cmp)
    {
        _tree.assign(cmp._tree);
        return *this;
    }

    allocator_type get_allocator() const noexcept
    {
        return _tree.get_allocator();
    }
    // ============================================================================
// -------------------------------
    iterator begin()
    {
        return iterator(_tree.begin(), false);
    }

    const_iterator begin() const
    {
        return const_iterator(_tree.begin(), false);
    }

    iterator end()
    {
        return iterator(_tree.end(), true);
    }

    const_iterator end() const
    {
        return const_iterator(_tree.end(), true);
    }

    reverse_iterator rbegin()
    {
        return reverse_iterator(_tree.end(), false);
    }

    const_reverse_iterator rbegin() const
    {
        return const_reverse_iterator(_tree.end(), false);
    }

    reverse_iterator rend()
    {
        return reverse_iterator(_tree.begin(), true);
    }

    const_reverse_iterator rend() const
    {
        return const_reverse_iterator(_tree.begin(), true);
    }

    // ============================================================================
// -------------------------------
    WUR bool empty() const noexcept
    {
        return _tree.size() == 0;
    }

    size_type size() const noexcept
    {
        return _tree.size();
    }

    size_type max_size() const noexcept
    {
        return std::numeric_limits<size_type>::max();
    }

    // ============================================================================
// -------------------------------
    void clear() noexcept
    {
        _tree.destroy();
    }

    insert_pair insert(const_reference value)
    {
        bool was_inserted;
        iterator node = iterator(_tree.insert(value, was_inserted));
        return insert_pair(node, was_inserted);
    }

    iterator insert(iterator hint, const_reference value)
    {
        bool _;
        return iterator(_tree.insert(hint.get_node(), value, _), false);
    }

    template <class InputIt>
    void insert(InputIt first, InputIt last)
    {
        bool _;
        while (first != last)
        {
            _tree.insert(*first, _);
            ++first;
        }
    }

    iterator erase(iterator pos)
    {
        tree_node *ret = _tree.remove_node(pos.get_node());
        if (ret == _tree.end())
            return iterator(ret, true);
        return iterator(ret, false);
    }

    size_type erase(const value_type &value)
    {
        size_t prev_size = _tree.size();
        _tree.remove(value);
        return prev_size - _tree.size();
    }

    template <class InputIt>
    void erase(InputIt first, InputIt last)
    {
        while (first != last)
            _tree.remove(*first);
    }

    void swap(set_base &other)
    {
        _tree.swap(other._tree);
    }

    // ============================================================================
// -------------------------------
    size_type count(const_reference value) const
    {
        return _tree.find(value) != nullptr;
    }

    iterator find(const_reference value)
    {
        tree_node *ret = _tree.find(value);
        if (ret == nullptr)
            return iterator(_tree.end(), true);
        return iterator(ret, true);
    }

    const_iterator find(const_reference value) const
    {
        tree_node *ret = _tree.find(value);
        if (ret == nullptr)
            return const_iterator(_tree.end(), true);
        return const_iterator(ret, true);
    }

#if CPP20
    bool contains(const_reference value) const
    {
        return count(value);
    }
#endif /* CPP20 */

    range_pair equal_range(const_reference value)
    {
        return range_pair(lower_bound(value), upper_bound(value));
    }

    const_range_pair equal_range(const_reference value) const
    {
        return const_range_pair(lower_bound(value), upper_bound(value));
    }

    iterator lower_bound(const_reference value)
    {
        tree_node *ret = _tree.lower_bound(value);
        if (ret == nullptr)
            return iterator(_tree.end(), true);
        return iterator(ret, false);
    }

    const_iterator lower_bound(const_reference value) const
    {
        tree_node *ret = _tree.lower_bound(value);
        if (ret == nullptr)
            return const_iterator(_tree.end(), true);
        return const_iterator(ret, false);    }

    iterator upper_bound(const_reference value)
    {
        tree_node *ret = _tree.upper_bound(value);
        if (ret == nullptr)
            return iterator(_tree.end(), true);
        return iterator(ret, false);
    }

    const_iterator upper_bound(const_reference value) const
    {
        tree_node *ret = _tree.upper_bound(value);
        if (ret == nullptr)
            return const_iterator(_tree.end(), true);
        return const_iterator(ret, false);
    }

    // ============================================================================
// -------------------------------
    key_compare key_comp() const
    {
        return _tree.get_cmp();
    }

    value_compare value_comp() const
    {
        return _tree.get_cmp();
    }

    friend bool operator ==(const set_base &s1, const set_base &s2)
    {
        return s1._tree == s2._tree;
    }

    friend bool operator !=(const set_base &s1, const set_base &s2)
    {
        return s1._tree != s2._tree;
    }

    friend bool operator >(const set_base &s1, const set_base &s2)
    {
        return s1._tree > s2._tree;
    }

    friend bool operator >=(const set_base &s1, const set_base &s2)
    {
        return s1._tree >= s2._tree;
    }

    friend bool operator <(const set_base &s1, const set_base &s2)
    {
        return s1._tree < s2._tree;
    }

    friend bool operator <=(const set_base &s1, const set_base &s2)
    {
        return s1._tree <= s2._tree;
    }
};

TLU_NAMESPACE_END

#endif /* SET_BASE_HPP */