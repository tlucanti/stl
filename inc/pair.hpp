
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

        constexpr pair(const pair &cpy) : base_class(cpy) {}

#if CPP11
        template <class type_U1, class type_U2>
        constexpr pair(
            type_U1 &&_first, type_U2 &&_second
        ) : base_class(std::move(_first), std::move(_second)) {}

        template<class U1, class U2>
        constexpr explicit pair(pair<U1, U2> &&mv) : base_class(std::move(mv.first), std::move(mv.second)) {}
#endif /* CPP11 */
    }; /* pair */
} /* ft */

#endif /* PAIR_HPP */