
#ifndef VECTOR_HPP
# define VECTOR_HPP

# include "vector_base.hpp"
# include "defs.h"

namespace ft
{
    template <class value_type, class allocator_type=std::allocator<value_type> >
    class vector : public TLU_NAMESPACE::vector_base<value_type, allocator_type>
    {
    private:
        typedef TLU_NAMESPACE::vector_base<value_type, allocator_type> base_class;

    public:
        constexpr vector() noexcept : base_class() {}

        constexpr explicit vector(
            const allocator_type &allocator
        ) noexcept : base_class(allocator) {}

        constexpr vector(
            typename base_class::size_type count,
            typename base_class::const_reference value,
            const allocator_type &allocator=allocator_type()
        ) : base_class(count, value, allocator) {}

        constexpr explicit vector(
            typename base_class::size_type count,
            const allocator_type &allocator=allocator_type()
        ) : base_class(count, allocator) {}

        template <class InputIt>
        constexpr vector(
            InputIt first, InputIt last,
            const allocator_type &allocator=allocator_type()
        ) : base_class(first, last, allocator) {}

        constexpr vector(const vector &other,
            const allocator_type &allocator=allocator_type()
        ) : base_class(other, allocator) {}

#if CPP11
        constexpr vector(
            vector &&other,
            const allocator_type &allocator=allocator_type()
        ) noexcept : base_class(std::move(other), allocator) {}

        constexpr vector(
            std::initializer_list<value_type> init,
            const allocator_type &allocator=allocator_type()
        ) : base_class(init, allocator) {}
#endif /* CPP11 */
    };
} /* ft */

#endif /* VECTOR_HPP */
