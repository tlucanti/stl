
#ifndef RB_TREE_ITERATOR_HPP
# define RB_TREE_ITERATOR_HPP

# include "defs.hpp"
# include "type_traits.hpp"

TLU_NAMESPACE_BEGIN

template <class value_T, class cmp_T=std::less<value_T> >
class rb_tree_iterator : public std::iterator<std::bidirectional_iterator_tag, value_T>
{
private:
    typedef typename std::iterator<std::bidirectional_iterator_tag, value_T> base_class;
public:
    typedef typename base_class::iterator_category  iterator_category;
    typedef typename base_class::value_type         value_type;
    typedef typename base_class::difference_type    difference_type;
    typedef typename base_class::pointer            pointer;
    typedef typename base_class::reference          reference;
    typedef const reference                         const_reference;
    typedef const pointer                           const_pointer;

private:
    typedef typename rb_tree<value_type>::rb_node rb_node;

    explicit rb_tree_iterator(rb_node *ptr, bool end) :
        _ptr(ptr), _end(end)
    {}

    rb_node *_ptr;
    bool    _end;

public:
    explicit rb_tree_iterator(const rb_tree_iterator &cpy) :
        _ptr(cpy._ptr), _end(cpy._end)
    {}

    ~rb_tree_iterator() DEFAULT

    const_reference operator *() const noexcept
    {
        return _ptr->get_key();
    }

    rb_tree_iterator &operator ++() // ++i
    {
        if (_end)
        {
            _end = false;
            return *this;
        }
        _ptr = rb_tree<value_type>::prev(_ptr);
        return *this;
    }

    rb_tree_iterator operator ++(int) // i++
    {
        return ++rb_tree_iterator(*this);
    }

    rb_tree_iterator &operator --() {} // --i

    rb_tree_iterator operator --(int) // i--
    {
        return --rb_tree_iterator(*this);
    }
};

template <class value_T, class cmp_T=std::less<value_T> >
class rb_tree_reverse_iterator : public std::iterator<std::bidirectional_iterator_tag, value_T>
{
private:
    typedef typename std::iterator<std::bidirectional_iterator_tag, value_T> base_class;
public:
    typedef typename base_class::iterator_category  iterator_category;
    typedef typename base_class::value_type         value_type;
    typedef typename base_class::difference_type    difference_type;
    typedef typename base_class::pointer            pointer;
    typedef typename base_class::reference          reference;
    typedef const reference                         const_reference;
    typedef const pointer                           const_pointer;

private:
    typedef typename rb_tree<value_type>::rb_node rb_node;

    explicit rb_tree_reverse_iterator(rb_node *ptr, bool end) :
            _ptr(ptr), _end(end)
    {}

    rb_node *_ptr;
    bool    _end;

public:
    explicit rb_tree_reverse_iterator(const rb_tree_reverse_iterator &cpy) :
            _ptr(cpy._ptr), _end(cpy._end)
    {}

    ~rb_tree_reverse_iterator() DEFAULT

    const_reference operator *() const noexcept
    {
        return _ptr->get_key();
    }

    rb_tree_reverse_iterator &operator ++() // ++i
    {
        if (_end)
        {
            _end = false;
            return *this;
        }
        _ptr = rb_tree<value_type>::prev(_ptr);
        return *this;
    }

    rb_tree_reverse_iterator operator ++(int) // i++
    {
        return ++rb_tree_reverse_iterator(*this);
    }

    rb_tree_reverse_iterator &operator --() // --i
    {
        if (_end)
        {
            _end = false;
            return *this;
        }
        _ptr = rb_tree<value_type>::prev(_ptr);
        return *this;
    }

    rb_tree_reverse_iterator operator --(int) // i--
    {
        return --rb_tree_reverse_iterator(*this);
    }
};

TLU_NAMESPACE_END

#endif /* RB_TREE_ITERATOR_HPP */