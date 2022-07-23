
#ifndef MAP_BASE_HPP
# define MAP_BASE_HPP

# include "defs.h"
# include "rb_tree.h"
# include "pair_base.hpp"
# include <functional>

TLU_NAMESPACE_BEGIN

template <
        class key_T,
        class val_T,
        class cmp_T=std::less<key_T>,
        class allocator_T=std::allocator<val_T>
    >
class map_base
{

};

TLU_NAMESPACE_END

#endif /* MAP_BASE_HPP */