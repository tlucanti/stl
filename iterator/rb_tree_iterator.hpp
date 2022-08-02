
#ifndef RB_TREE_ITERATOR_HPP
# define RB_TREE_ITERATOR_HPP

# include "defs.hpp"
# include "type_traits.hpp"

TLU_NAMESPACE_BEGIN

template<class value_T, class size_T, class cmp_T=std::less<value_T> >
class rb_tree_iterator_base : public std::iterator<std::bidirectional_iterator_tag, value_T>
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


protected:
    typedef typename rb_tree<
            typename remove_const<value_type>::type
        >::rb_node rb_node;

private:
    rb_node *_ptr;
    bool    _end;

public:
    virtual reference operator *()
    {
        return _ptr->get_key();
    }

    virtual bool operator ==(const rb_tree_iterator_base &cmp)
    {
        return _ptr == cmp._ptr;
    }

    virtual bool operator !=(const rb_tree_iterator_base &cmp)
    {
        return _ptr != cmp._ptr;
    }

protected:
    rb_tree_iterator_base(rb_node *ptr, bool end) :
        _ptr(ptr), _end(end)
    {}

    void increment() // ++i
    {
        if (_end)
            _end = false;
        else
            _ptr = rb_tree<value_type>::prev(_ptr);
    }

    void decrement()
    {
        if (_end)
            _end = false;
        else
            _ptr = rb_tree<value_type>::prev(_ptr);
    }

    virtual ~rb_tree_iterator_base() DEFAULT
};

template <class value_T, class size_T, class cmp_T=std::less<value_T> >
class rb_tree_iterator : public rb_tree_iterator_base<value_T, size_T, cmp_T>
{
private:
    typedef rb_tree_iterator_base<value_T, size_T, cmp_T> base_class;

public:
    typedef typename base_class::iterator_category  iterator_category;
    typedef typename base_class::value_type         value_type;
    typedef typename base_class::difference_type    difference_type;
    typedef typename base_class::pointer            pointer;
    typedef typename base_class::reference          reference;
    typedef const reference                         const_reference;
    typedef const pointer                           const_pointer;

public:
    typedef typename base_class::rb_node rb_node;


public: // TODO: remove this
    rb_tree_iterator(rb_node *ptr, bool end=false) : base_class(ptr, end)
    {}

public:
    rb_tree_iterator(const rb_tree_iterator &cpy) : base_class(cpy)
    {}

    ~rb_tree_iterator() DEFAULT

    rb_tree_iterator &operator ++() // ++i
    {
        this->increment();
        return *this;
    }

    rb_tree_iterator operator ++(int) // i++
    {
        rb_tree_iterator ret(*this);
        this->increment();
        return ret;
    }

    rb_tree_iterator &operator --() // --i
    {
        this->decrement();
        return *this;
    }

    rb_tree_iterator operator --(int) // i--
    {
       rb_tree_iterator ret(*this);
       this->decrement();
       return ret;
    }

};

template <class value_T, class size_T, class cmp_T=std::less<value_T> >
class rb_tree_reverse_iterator : public rb_tree_iterator_base<value_T, size_T, cmp_T>
{
private:
    typedef rb_tree_iterator_base<value_T, size_T, cmp_T> base_class;

public:
    typedef typename base_class::iterator_category  iterator_category;
    typedef typename base_class::value_type         value_type;
    typedef typename base_class::difference_type    difference_type;
    typedef typename base_class::pointer            pointer;
    typedef typename base_class::reference          reference;
    typedef const reference                         const_reference;
    typedef const pointer                           const_pointer;

protected:
    typedef typename base_class::rb_node rb_node;


public: // TODO: remove this
    rb_tree_reverse_iterator(rb_node *ptr, bool end) : base_class(ptr, end)
    {}

public:
    explicit rb_tree_reverse_iterator(const rb_tree_reverse_iterator &cpy) : base_class(cpy)
    {}

    ~rb_tree_reverse_iterator() DEFAULT

    rb_tree_reverse_iterator &operator ++() // ++i
    {
        this->decrement();
        return *this;
    }

    rb_tree_reverse_iterator operator ++(int) // i++
    {
        rb_tree_reverse_iterator ret(*this);
        this->decrement();
        return ret;
    }

    rb_tree_reverse_iterator &operator --() // --i
    {
        this->increment();
        return *this;
    }

    rb_tree_reverse_iterator operator --(int) // i--
    {
        rb_tree_reverse_iterator ret(*this);
        this->increment();
        return ret;
    }

};

TLU_NAMESPACE_END

#endif /* RB_TREE_ITERATOR_HPP */