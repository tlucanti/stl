
#include <iterator>

#include "defs.h"

TLU_NAMESPACE_BEGIN

# define WRAP_ITERATOR_BOOL_OPERATOR_MACRO(__op) constexpr bool operator __op( \
    const self_type &cmp) const noexcept { return _ptr __op cmp._ptr; }

# define WRAP_ITERATOR_ARITHMETIC_OPERATOR_MACRO(__op) constexpr self_type \
    operator __op(difference_type shift) const noexcept { return self_type(_ptr __op shift); }

# define WRAP_REVERSE_ITERATOR_BOOL_OPERATOR_MACRO(__op, __do) constexpr bool operator __op( \
    const self_type &cmp) const noexcept { return this->_ptr __do cmp._ptr; }

# define WRAP_REVERSE_ITERATOR_ARITHMETIC_OPERATOR_MACRO(__op) constexpr self_type \
    operator __op(difference_type shift) const noexcept { return self_type(this->_ptr __op -shift); }

template <class type_T>
class wrap_iterator_base : public std::iterator<std::random_access_iterator_tag, type_T>
{
// --------------------------------- typedefs ----------------------------------
public:
    typedef std::iterator<std::random_access_iterator_tag, type_T>  iterator_traits;
    typedef typename iterator_traits::iterator_category             iterator_category;
    typedef typename iterator_traits::value_type                    value_type;
    typedef typename iterator_traits::difference_type               difference_type;
    typedef typename iterator_traits::pointer                       pointer;
    typedef typename iterator_traits::reference                     reference;

PRIVATE:
    typedef wrap_iterator_base<value_type>                          self_type;

//    friend vector_base<value_type>;

// ------------------------------ protected fields -------------------------------
PROTECTED:
    pointer    _ptr;

// =============================================================================
// ------------------------------ initialization -------------------------------
PROTECTED:
    explicit constexpr wrap_iterator_base(pointer ptr) : _ptr(ptr) {}
public:
    constexpr wrap_iterator_base() : _ptr(nullptr) {}
    constexpr wrap_iterator_base(const self_type &cpy) : _ptr(cpy._ptr) {}

# if CPP11
    constexpr wrap_iterator_base(self_type &&mv) noexcept
        : _ptr(mv._ptr)
    {
        mv._ptr = nullptr;
    }
# endif /* CPP11 */

// =============================================================================
// --------------------------------- operators ---------------------------------
    WRAP_ITERATOR_BOOL_OPERATOR_MACRO(==)
    WRAP_ITERATOR_BOOL_OPERATOR_MACRO(!=)
    WRAP_ITERATOR_BOOL_OPERATOR_MACRO(> )
    WRAP_ITERATOR_BOOL_OPERATOR_MACRO(>=)
    WRAP_ITERATOR_BOOL_OPERATOR_MACRO(< )
    WRAP_ITERATOR_BOOL_OPERATOR_MACRO(<=)

    WRAP_ITERATOR_ARITHMETIC_OPERATOR_MACRO(+)
    WRAP_ITERATOR_ARITHMETIC_OPERATOR_MACRO(-)

    constexpr value_type operator *() noexcept { return *_ptr; } // *i

    constexpr self_type &operator ++() noexcept // ++i
    {
        ++_ptr;
        return *this;
    }

    constexpr self_type operator ++(int) noexcept // i++
    {
        const self_type ret = self_type(_ptr);
        ++_ptr;
        return ret;
    }

    constexpr self_type &operator --() noexcept // --i
    {
        --_ptr;
        return *this;
    }

    constexpr self_type operator --(int) noexcept // i--
    {
        const self_type ret = self_type(_ptr);
        --_ptr;
        return ret;
    }

    constexpr difference_type operator -(const self_type &diff) const noexcept
    {
        return _ptr - diff._ptr;
    }

// ---------------------------- static members ----------------------------------
    static constexpr difference_type distance(const self_type &begin,
                                              const self_type &end)
    {
        if (end._ptr > begin._ptr)
            return end._ptr - begin._ptr;
        return begin._ptr - end._ptr;
    }

    template <class input_it>
    static constexpr typename std::iterator_traits<input_it>::difference_type
        distance(input_it first, const input_it &last)
    {
        return _distance(first, last, typename std::iterator_traits<input_it>::iterator_category());
    }

    template <class input_it>
    static constexpr typename std::iterator_traits<input_it>::difference_type
        _distance(input_it first, const input_it &last, std::input_iterator_tag)
    {
        typename std::iterator_traits<input_it>::difference_type n = 0;
        while (first++ != last)
            ++n;
        return n;
    }

    template <class rand_it>
    static constexpr typename std::iterator_traits<rand_it>::difference_type
        _distance(rand_it first, const rand_it &last, std::random_access_iterator_tag)
    {
        return last - first;
    }
};

template <class type_T>
class wrap_iterator : public wrap_iterator_base<type_T>
{
PRIVATE:
    typedef wrap_iterator_base<type_T>                  base_class;
public:
    typedef typename base_class::iterator_traits        iterator_traits;
    typedef typename base_class::iterator_category      iterator_category;
    typedef typename base_class::value_type             value_type;
    typedef typename base_class::difference_type        difference_type;
    typedef typename base_class::pointer                pointer;
    typedef typename base_class::reference              reference;
PRIVATE:
    typedef wrap_iterator<value_type>                   self_type;

PROTECTED:
    explicit constexpr wrap_iterator(pointer ptr) : base_class(ptr) {}
public:
    constexpr wrap_iterator() : base_class() {}
    constexpr wrap_iterator(const self_type &cpy) : base_class(cpy) {}
# if CPP11
    constexpr wrap_iterator(self_type &&mv) noexcept : base_class(mv) {}
# endif /* CPP11 */
};

template <class type_T>
class wrap_reverse_iterator : public wrap_iterator_base<type_T>
{
// --------------------------------- typedefs ----------------------------------

PRIVATE:
    typedef wrap_iterator_base<type_T>                  base_class;
public:
    typedef typename base_class::iterator_traits        iterator_traits;
    typedef typename base_class::iterator_category      iterator_category;
    typedef typename base_class::value_type             value_type;
    typedef typename base_class::difference_type        difference_type;
    typedef typename base_class::pointer                pointer;
    typedef typename base_class::reference              reference;
PRIVATE:
    typedef wrap_reverse_iterator<value_type>           self_type;

// =============================================================================
// ------------------------------ initialization -------------------------------
PROTECTED:
    explicit constexpr wrap_reverse_iterator(pointer ptr) : base_class(ptr) {}
public:
    constexpr wrap_reverse_iterator() : base_class() {}
    constexpr wrap_reverse_iterator(const self_type &cpy) : base_class(cpy) {}
# if CPP11
    constexpr wrap_reverse_iterator(self_type &&mv) noexcept : base_class(mv) {}
# endif /* CPP11 */

// =============================================================================
// --------------------------------- operators ---------------------------------
public:
    WRAP_REVERSE_ITERATOR_BOOL_OPERATOR_MACRO(> , < )
    WRAP_REVERSE_ITERATOR_BOOL_OPERATOR_MACRO(>=, <=)
    WRAP_REVERSE_ITERATOR_BOOL_OPERATOR_MACRO(< , > )
    WRAP_REVERSE_ITERATOR_BOOL_OPERATOR_MACRO(<=, >=)

    WRAP_REVERSE_ITERATOR_ARITHMETIC_OPERATOR_MACRO(+)
    WRAP_REVERSE_ITERATOR_ARITHMETIC_OPERATOR_MACRO(-)

    constexpr self_type &operator ++() noexcept // ++i
    {
        --this->_ptr;
        return *this;
    }

    constexpr self_type operator ++(int) noexcept // i++
    {
        const self_type ret = self_type(this->_ptr);
        --this->_ptr;
        return ret;
    }

    constexpr self_type &operator --() noexcept // --i
    {
        ++this->_ptr;
        return *this;
    }

    constexpr self_type operator --(int) noexcept // i--
    {
        const self_type ret = self_type(this->_ptr);
        ++this->_ptr;
        return ret;
    }

    constexpr difference_type operator -(const self_type &diff) const noexcept
    {
        return diff._ptr - this->_ptr;
    }
};

TLU_NAMESPACE_END
