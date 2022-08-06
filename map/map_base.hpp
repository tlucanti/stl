
#ifndef MAP_BASE_HPP
# define MAP_BASE_HPP

# include "defs.hpp"
# include "set_base.hpp"
# include <stdexcept>

TLU_NAMESPACE_BEGIN

TLU_NAMESPACE_HIDDEN_BEGIN

template <class key_compare, class value_type>
struct pair_key_compare : public key_compare
{
    typedef bool        result_type;
    typedef value_type  first_argument_type;
    typedef value_type  second_argument_type;

    constexpr pair_key_compare() : key_compare() {}
    constexpr pair_key_compare(const key_compare &cmp) : key_compare(cmp) {}

    constexpr bool operator()(const value_type &lhs, const value_type &rhs) const
    {
        return key_compare::operator()(lhs.first, rhs.first);
    }
};

TLU_NAMESPACE_HIDDEN_END

template <
        class key_T,
        class val_T,
        class cmp_T=std::less<key_T>,
        class allocator_T=std::allocator<val_T>,
        class pair_T=pair_base<int, int>
    >
class map_base : public
    set_base<
        typename change_pair_type<pair_T, const key_T, val_T>::type, /* key_type = pair<key_type, value_type> */
        TLU_NAMESPACE_HIDDEN::pair_key_compare<
                cmp_T,
                typename change_pair_type<pair_T, const key_T, val_T>::type
            >, /* key_compare = value_compare<key_compare, value_type> */
        allocator_T, /* allocator_type = allocator_type */
        pair_T /* pair_T = pair_T */
    >
{
public:
    typedef key_T                                       key_type;
    typedef val_T                                       mapped_type;
    typedef typename change_pair_type<
            pair_T, const key_type, mapped_type
        >::type                                         value_type;
    typedef std::size_t                                 size_type;
    typedef std::ptrdiff_t                              difference_type;
    typedef cmp_T                                       key_compare;
    typedef allocator_T                                 allocator_type;
    typedef value_type                                  &reference;
    typedef const value_type                            &const_reference;
    typedef typename allocator_type::pointer            pointer;
    typedef typename allocator_type::const_pointer      const_pointer;

    typedef TLU_NAMESPACE_HIDDEN::pair_key_compare<key_compare, value_type>  value_compare;

    typedef rb_tree_iterator<value_type, value_compare, allocator_type>                iterator;
    typedef rb_tree_iterator<const value_type, value_compare, allocator_type>          const_iterator;
    typedef rb_tree_reverse_iterator<value_type, value_compare, allocator_type>        reverse_iterator;
    typedef rb_tree_reverse_iterator<const value_type, value_compare, allocator_type>  const_reverse_iterator;


private:

    typedef set_base<
        value_type,
        value_compare,
        allocator_type,
        pair_T
    > base_class;
    typedef typename iterator::rb_node                                tree_node;
    typedef typename change_pair_type<pair_T, iterator, bool>::type     insert_pair;
    typedef typename change_pair_type<pair_T, iterator, iterator>::type range_pair;
    typedef typename change_pair_type<pair_T, const_iterator, const_iterator>::type const_range_pair;

    mapped_type _dummy_val;

public:
    // ============================================================================
// ------------------------------- constructors/destructors
    map_base() : base_class() {}

    explicit map_base(const key_compare &comp, const allocator_type &alloc=allocator_type()) :
        base_class(value_compare(comp), alloc)
    {}

    template <class InputIterator>
    map_base(
        InputIterator first,
        InputIterator last,
        const key_compare &comp=key_compare(),
        const allocator_type &alloc=allocator_type()
    ) : base_class(first, last, value_compare(comp), alloc) {}

    map_base(const map_base &cpy) : base_class(cpy) {}

    // =========================================================================
    // -------------------------- element access -------------------------------
    mapped_type &at(const key_type &key)
    {
        return _at(key);
    }

    const mapped_type &at(const key_type &key) const
    {
        return _at(key);
    }

    mapped_type &operator [](const key_type &key)
    {
        return this->_tree.insert(value_type(key, mapped_type()))->get_key().second;
    }

    size_type count(const key_type &key) const
    {
        return base_class::count(value_type(key, _dummy_val));
    }

    iterator find(const key_type &key)
    {
        return base_class::find(value_type(key, _dummy_val));
    }

    const_iterator find(const key_type &key) const
    {
        return base_class::find(value_type(key, _dummy_val));
    }

    range_pair equal_range(const key_type &key)
    {
        return range_pair(lower_bound(key), upper_bound(key));
    }

    const_range_pair equal_range(const key_type &key) const
    {
        return const_range_pair(lower_bound(key), upper_bound(key));
    }

    iterator lower_bound(const key_type &key)
    {
        return base_class::lower_bound(value_type(key, _dummy_val));
    }

    const_iterator lower_bound(const key_type &key) const
    {
        return base_class::lower_bound(value_type(key, _dummy_val));
    }

    iterator upper_bound(const key_type &key)
    {
        return base_class::upper_bound(value_type(key, _dummy_val));
    }

    const_iterator upper_bound(const key_type &key) const
    {
        return base_class::upper_bound(value_type(key, _dummy_val));
    }

    // ===========================================================================
    // ------------------------- modifiers --------------------

private:
    mapped_type &_at(const key_type &key) const
    {
        AUTO(tree_node *) nd = reinterpret_cast<tree_node *>(this->_tree.find(value_type(key, _dummy_val)));
        if (nd == nullptr)
            throw std::out_of_range("map_base::_at()");
        return nd->get_key().second;
    }
};

TLU_NAMESPACE_END

#endif /* MAP_BASE_HPP */
