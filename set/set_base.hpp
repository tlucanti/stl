
#ifndef SET_BASE_HPP
# define SET_BASE_HPP

# include "defs.h"
# include "rb_tree.hpp"
# include "rb_tree_iterator.hpp"
# include <functional>

TLU_NAMESPACE_BEGIN

template <
        class key_T,
        class cmp_T=std::less<key_T>,
        class allocator_T=std::allocator<key_T>
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
    typedef rb_tree_iterator<value_type>                iterator;
    typedef rb_tree_iterator<const value_type>          const_iterator;
    typedef rb_tree_reverse_iterator<value_type>        reverse_iterator;
    typedef rb_tree_reverse_iterator<const value_type>  const_reverse_iterator;

private:
    typedef rb_tree<value_type, key_compare>            tree_type;
    typedef typename tree_type::rb_node                 tree_node;

    tree_type       _tree;
    tree_node       *_begin;
    tree_node       *_end;
    size_type       _size;
    allocator_type  _alloc;
    key_compare     _cmp;

// ============================================================================
// ------------------------------- constructors/destructors
public:
    set_base() :
        _tree(tree_type()),
        _begin(nullptr),
        _end(nullptr),
        _size(0),
        _alloc(allocator_type()),
        _cmp(key_compare())
    {}

    explicit set_base(
        const key_type &comp,
        const allocator_type &alloc=allocator_type()
    ) :
        _tree(tree_type()),
        _begin(nullptr),
        _end(nullptr),
        _size(0),
        _alloc(allocator_type()),
        _cmp(key_compare())
    {}

    template<class iterator_type>
    set_base(
        iterator_type first,
        iterator_type last,
        const key_type &comp=key_type(),
        const allocator_type &alloc=allocator_type()
    ) :
        _tree(tree_type()),
        _begin(nullptr),
        _end(nullptr),
        _size(0),
        _alloc(allocator_type()),
        _cmp(key_compare())
    {
        while (first != last)
            insert(*first++);
    }

    set_base(const set_base &cmp) :
        _tree(cmp._tree),
        _begin(nullptr),
        _end(nullptr),
        _size(0),
        _alloc(cmp._alloc),
        _cmp(cmp._cmp)
    {

    }

#if CPP11
    set_base(set_base &&mv) :
        _tree(std::move(mv._tree))
        _begin(mv._begin)
        _end(mv._end)
        _size(mv.size())
        _alloc(std::move(mv._alloc))
        _cmp(std::move(mv._cmp))
    {
        mv._begin = nullptr;
        mv._end = nullptr;
        mv._size = 0;
    }

    set_base(set_base &&mv, const allocator_type &alloc)
        _tree(std::move(mv._tree))
        _begin(mv._begin)
        _end(mv._end)
        _size(mv.size())
        _alloc(alloc)
        _cmp(std::move(mv._cmp))
    {
        mv._begin = nullptr;
        mv._end = nullptr;
        mv._size = 0;
    }
#endif /* CPP11 */

    ~set_base() DEFAULT

// ============================================================================
// -------------------------------
    void insert(const_reference value)
    {
        _tree.insert(value);
    }

    value_type find(const_reference value)
    {
        _tree.find(value);
    }

private:

};

TLU_NAMESPACE_END

#endif /* SET_BASE_HPP */