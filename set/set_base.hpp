
#ifndef SET_BASE_HPP
# define SET_BASE_HPP

# include "defs.h"
# include "rb_tree.h"
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
    rb_tree         _tree;
    rb_node         _begin;
    rb_node         _end;
    size_type       _size;
    allocator_type  _alloc;
    key_compare     _cmp;

public:
    set_base() :
        _tree((rb_tree){nullptr}),
        _begin((rb_node){nullptr}),
        _end((rb_node){nullptr}),
        _size(0),
        _alloc(allocator_type()),
        _cmp(key_compare())
    {}

    explicit set_base(
        const key_type &comp,
        const allocator_type &alloc=allocator_type()
    ) :
        _tree((rb_tree){nullptr}),
        _begin((rb_node){nullptr}),
        _end((rb_node){nullptr}),
        _size(0),
        _alloc(alloc),
        _cmp(comp)
    {}

    template<class iterator_type>
    set_base(
        iterator_type first,
        iterator_type last,
        const key_type &comp=key_type(),
        const allocator_type &alloc=allocator_type()
    ) :
        _tree((rb_tree){nullptr}),
        _begin((rb_node){nullptr}),
        _end((rb_node){nullptr}),
        _size(0),
        _alloc(alloc),
        _cmp(comp)
    {
        while (first != last)
            rb_insert(&_tree, &(*first++), &set_base::_compare_fun);
    }

    void insert(value_type value)
    {
        rb_insert(&_tree, static_cast<void *>(&value),
                  (compare_fun)(&set_base::_compare_fun));
    }

    void *find(value_type value)
    {
        return rb_get_key(rb_find(&_tree, static_cast<void *>(&value), &set_base::_compare_fun));
    }

private:
    int _compare_fun(void *lhs, void *rhs)
    {
        if (_cmp(*static_cast<value_type *>(lhs), *static_cast<value_type *>(rhs)))
            return -1;
        else if (rhs < lhs)
            return 0;
        return 1;
    }
};

TLU_NAMESPACE_END

#endif /* SET_BASE_HPP */