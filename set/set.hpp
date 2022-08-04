
#ifndef SET_HPP
# define SET_HPP

# include "set_base.hpp"
# include "pair.hpp"
# include "defs.hpp"

namespace ft
{
    template <
            class key_type,
            class key_compare=std::less<key_type>,
            class allocator_type=std::allocator<key_type>
    >
    class set : public TLU_NAMESPACE::set_base<key_type, key_compare, allocator_type, ft::pair<int, int> >
    {
    private:
        typedef TLU_NAMESPACE::set_base<key_type, key_compare, allocator_type, ft::pair<int, int> > base_class;

    public:
        constexpr set() noexcept : base_class() {}

        constexpr explicit set(
            const key_compare &compare,
            const allocator_type &allocator=allocator_type()
        ) noexcept : base_class(compare, allocator) {}

        template <class InputIterator>
        constexpr set(
            InputIterator first,
            InputIterator last,
            const key_compare &compare=key_compare(),
            const allocator_type &allocator=allocator_type()
        ) : base_class(first, last, compare, allocator) {}

        constexpr set(const set &other) : base_class(other) {}
    };
}

#endif /* SET_HPP */
