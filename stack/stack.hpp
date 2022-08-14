/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   stack.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tlucanti <tlucanti@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/14 11:26:42 by tlucanti          #+#    #+#             */
/*   Updated: 2022/08/14 20:10:21 by tlucanti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef STACK_HPP
# define STACK_HPP

# include "defs.hpp"
# include "stack_base.hpp"

namespace ft
{
    template <class value_type, class container_type>
    class stack : public
        TLU_NAMESPACE::stack_base<value_type, container_type>
    {
// -----------------------------------------------------------------------------
    private:
        typedef TLU_NAMESPACE::stack_base<
            value_type,
            container_type
        >   base_class;

// -----------------------------------------------------------------------------
    public:
        constexpr stack() : base_class() {}

// -----------------------------------------------------------------------------
        explicit constexpr stack(const container_type &cont)
            : base_class(cont)
        {}

// -----------------------------------------------------------------------------
        constexpr stack(const stack &cpy) : base_class(cpy) {}

// -----------------------------------------------------------------------------
#if CPP11
        explicit constexpr stack(container_type &&cont) noexcept
            : base_class(std::move(cont))
        {}

// -----------------------------------------------------------------------------
        constexpr stack(stack &&mv) noexcept : base_class(std::move(mv)) {}
#endif /* CPP11 */

    }; /* stack */

} /* ft */

#endif /* STACK_HPP */
