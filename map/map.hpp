/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tlucanti <tlucanti@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/06 16:24:53 by tlucanti          #+#    #+#             */
/*   Updated: 2022/08/14 19:40:01 by tlucanti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MAP_HPP
# define MAP_HPP

# include "map_base.hpp"
# include "pair.hpp"
# include "defs.hpp"

namespace ft
{
    template <
        class key_type,
        class value_type,
        class key_compare=std::less<key_type>,
        class allocator_type=std::allocator<value_type>
    >
    class map : public
        TLU_NAMESPACE::map_base<
            key_type,
            value_type,
            key_compare,
            allocator_type,
            ft::pair<int, int>
        >
    {
// -----------------------------------------------------------------------------
    private:
        typedef TLU_NAMESPACE::map_base<
            key_type,
            value_type,
            key_compare,
            allocator_type,
            ft::pair<int, int>
        >   base_class;

// -----------------------------------------------------------------------------
    public:
        constexpr map() noexcept : base_class() {}

// -----------------------------------------------------------------------------
        constexpr explicit map(
            const key_compare &compare,
            const allocator_type &allocator=allocator_type()
        ) noexcept : base_class(compare, allocator) {}

// -----------------------------------------------------------------------------
        template <class InputIterator>
        constexpr map(
            InputIterator first,
            InputIterator last,
            const key_compare &compare=key_compare(),
            const allocator_type &allocator=allocator_type()
        ) : base_class(first, last, compare, allocator) {}

// -----------------------------------------------------------------------------
        constexpr map(const map &other) : base_class(other) {}

    }; /* map */

} /* ft */

#endif /* MAP_HPP */
