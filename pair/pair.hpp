
#ifndef PAIR_HPP
# define PAIR_HPP

# include "defs.h"
# include "pair_base.hpp"

namespace ft
{
    template <class first_type, class second_type>
    class pair : public TLU_NAMESPACE::pair_base<first_type, second_type>
    {
    private:
        typedef TLU_NAMESPACE::pair_base<first_type, second_type>   base_class;

    public:
        constexpr pair() : base_class() {}

        constexpr pair(
            const first_type &_first, const second_type &_second
        ) : base_class(_first, _second) {}

        template <class type_U1, class type_U2>
        constexpr explicit pair(const pair<type_U1, type_U2> &cpy) : base_class(cpy) {}

        template <class type_U1, class type_U2>
        constexpr pair(const std::pair<type_U1, type_U2> &cpy) : base_class(cpy) {}

        constexpr pair(const pair &cpy) : base_class(cpy) {}

        template <class type_U1, class type_U2>
        constexpr pair &operator =(const pair<type_U1, type_U2> &other)
        {
            base_class::operator=(other);
            return *this;
        }
    }; /* pair */

    template <class type_T, class type_Y>
    constexpr pair<type_T, type_Y> make_pair(type_T first, type_Y second)
    {
        return pair<type_T, type_Y>(first, second);
    }

} /* ft */

TLU_NAMESPACE_BEGIN

template <class T, class Y, class type_T, class type_Y> struct change_pair_type<ft::pair<T, Y>, type_T, type_Y> {
    typedef ft::pair<type_T, type_Y> type;
};

TLU_NAMESPACE_END

#endif /* PAIR_HPP */