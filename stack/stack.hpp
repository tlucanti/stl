
#ifndef STACK_HPP
# define STACK_HPP

# include "defs.hpp"
# include "stack_base.hpp"

namespace ft
{
    template <class value_type, class container_type>
    class stack : public TLU_NAMESPACE::stack_base<value_type, container_type>
    {
    private:
        typedef TLU_NAMESPACE::stack_base<value_type, container_type>   base_class;

    public:
        stack() : base_class() {}

        explicit stack(const container_type &cont) : base_class(cont) {}

        stack(const stack &cpy) : base_class(cpy) {}

#if CPP11
        explicit stack(container_type &&cont) : base_class(std::move(cont)) {}

        stack(stack &&mv) noexcept : base_class(std::move(mv)) {}
#endif /* CPP11 */
    }; /* stack */
} /* ft */

#endif /* STACK_HPP */