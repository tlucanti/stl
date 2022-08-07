
#ifndef RB_TREE_ITERATOR_HPP
# define RB_TREE_ITERATOR_HPP

# include "defs.hpp"
# include "type_traits.hpp"

TLU_NAMESPACE_BEGIN

template<class value_T, class cmp_T, class alloc_T>
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
    typedef rb_tree<
            typename remove_const<value_type>::type,
            cmp_T,
            alloc_T
        > tree_type;
    typedef typename tree_type::rb_node rb_node;

private:
    rb_node *_ptr;
    bool    _end;

public:
    virtual reference operator *() const
    {
        return _ptr->get_key();
    }

    virtual bool operator ==(const rb_tree_iterator_base &cmp) const
    {
        bool x = _end;
        bool y = cmp._end;
        bool z = _ptr == cmp._ptr;
        return (x and y) or ((not x) and (not y) and z);
    }

    virtual bool operator !=(const rb_tree_iterator_base &cmp) const
    {
        return not operator ==(cmp);
    }

    virtual rb_node *get_node() const
    {
        return _ptr;
    }

    virtual bool is_end() const
    {
        return _end;
    }

protected:
    rb_tree_iterator_base() :
        _ptr(nullptr), _end(false)
    {}

    rb_tree_iterator_base(rb_node *ptr, bool end) :
        _ptr(ptr), _end(end)
    {}

    virtual ~rb_tree_iterator_base() DEFAULT

    void increment() // ++i
    {
        if (_end)
            _end = false;
        else
            _ptr = tree_type::next(_ptr);
        if (_ptr == nullptr)
            _end = true;
    }

    void decrement()
    {
        if (_end)
            _end = false;
        else
            _ptr = tree_type::prev(_ptr);
    }
};

template<class value_T, class cmp_T, class alloc_T>
class rb_tree_iterator : public rb_tree_iterator_base<value_T, cmp_T, alloc_T>
{
private:
    typedef rb_tree_iterator_base<value_T, cmp_T, alloc_T> base_class;

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
    rb_tree_iterator() DEFAULT

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

template<class value_T, class key_T, class cmp_T>
class rb_tree_reverse_iterator : public rb_tree_iterator_base<value_T, key_T, cmp_T>
{
private:
    typedef rb_tree_iterator_base<value_T, key_T, cmp_T> base_class;

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

    rb_tree_reverse_iterator() DEFAULT

    rb_tree_reverse_iterator(const rb_tree_reverse_iterator &cpy) : base_class(cpy)
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