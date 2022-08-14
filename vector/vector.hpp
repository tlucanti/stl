/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vector.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tlucanti <tlucanti@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/03 18:51:11 by tlucanti          #+#    #+#             */
/*   Updated: 2022/08/14 20:26:39 by tlucanti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef VECTOR_HPP
# define VECTOR_HPP

# include "vector_base.hpp"
# include "defs.hpp"

namespace ft
{
    template <
        class value_type,
        class allocator_type=std::allocator<value_type>
    >
    class vector : public
        TLU_NAMESPACE::vector_base<value_type, allocator_type>
    {
// -----------------------------------------------------------------------------
    private:
        typedef TLU_NAMESPACE::vector_base<
            value_type,
            allocator_type
        >   base_class;

// -----------------------------------------------------------------------------
    public:
        constexpr vector() noexcept : base_class() {}

// -----------------------------------------------------------------------------
        constexpr explicit vector(
            const allocator_type &allocator
        ) noexcept : base_class(allocator) {}

// -----------------------------------------------------------------------------
        constexpr vector(
            typename base_class::difference_type count,
            typename base_class::const_reference value,
            const allocator_type &allocator=allocator_type()
        ) : base_class(count, value, allocator) {}

// -----------------------------------------------------------------------------
        constexpr explicit vector(
            typename base_class::difference_type count,
            const allocator_type &allocator=allocator_type()
        ) : base_class(count, allocator) {}

// -----------------------------------------------------------------------------
        template <class InputIterator>
        constexpr vector(
            InputIterator first,
            InputIterator last,
            const allocator_type &allocator=allocator_type()
        ) : base_class(first, last, allocator) {}

// -----------------------------------------------------------------------------
        constexpr vector(const vector &other,
            const allocator_type &allocator=allocator_type()
        ) : base_class(other, allocator) {}

// -----------------------------------------------------------------------------
#if CPP11
        constexpr vector(
            vector &&other,
            const allocator_type &allocator=allocator_type()
        ) noexcept : base_class(std::move(other), allocator) {}

// -----------------------------------------------------------------------------
        constexpr vector(
            std::initializer_list<value_type> init,
            const allocator_type &allocator=allocator_type()
        ) : base_class(init, allocator) {}
#endif /* CPP11 */

    }; /* vector */

} /* ft */

#endif /* VECTOR_HPP */
