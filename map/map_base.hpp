
#ifndef MAP_BASE_HPP
# define MAP_BASE_HPP

# include "defs.hpp"
# include "set_base.hpp"

TLU_NAMESPACE_BEGIN

TLU_NAMESPACE_HIDDEN_BEGIN

template <class key_compare>
struct pair_key_compare : public key_compare
{
    constexpr bool operator()(
            const typename key_compare::value_type &lhs,
            const typename key_compare::value_type &rhs
        ) const
    {
        return key_compare::operator()(lhs.first, rhs.first);
    }
};

TLU_NAMESPACE_HIDDEN_END

template <
        class key_T,
        class val_T,
        class cmp_T=std::less<key_T>,
        class allocator_T=std::allocator<val_T>
    >
class map_base :
    set_base<
        pair_base<const key_T, val_T>,
        TLU_NAMESPACE_HIDDEN::pair_key_compare<key_T>,
        allocator_T
    >
{
public:
    typedef key_T                                       key_type;
    typedef val_T                                       mapped_type;
    typedef pair_base<const key_type, mapped_type>      value_type;
    typedef std::size_t                                 size_type;
    typedef std::ptrdiff_t                              difference_type;
    typedef cmp_T                                       key_compare;
    typedef allocator_T                                 allocator_type;
    typedef value_type                                  &reference;
    typedef const value_type                            &const_reference;
    typedef typename allocator_type::pointer            pointer;
    typedef typename allocator_type::const_pointer      const_pointer;
    typedef rb_tree_iterator<value_type>                iterator;
    typedef rb_tree_iterator<const value_type>          const_iterator;
    typedef rb_tree_reverse_iterator<value_type>        reverse_iterator;
    typedef rb_tree_reverse_iterator<const value_type>  const_reverse_iterator;

    typedef TLU_NAMESPACE_HIDDEN::pair_key_compare<key_T>   value_compare;

private:

    typedef rb_tree<
            value_type,
            value_compare,
            allocator_type
    >   tree_type;
    typedef tree_type                   base_class;
    typedef typename tree_type::rb_node tree_node;

    tree_type       _tree;

public:
    // ============================================================================
// ------------------------------- constructors/destructors
    map_base() : base_class() {}

    explicit map_base(const key_compare &comp, const allocator_type &alloc) :
        base_class(comp, alloc)
    {}

    template <class InputIt>
    map_base(
        InputIt first,
        InputIt last,
        const key_compare &comp,
        const allocator_type &alloc
    ) :
        base_class(first, last, comp, alloc)
    {}

};

TLU_NAMESPACE_END

#endif /* MAP_BASE_HPP */