
#ifndef ENABLE_IF_HPP
# define ENABLE_IF_HPP

# include "defs.h"

TLU_NAMESPACE_BEGIN

template <bool term, class type_T>
struct enable_if {};

template <class type_T>
struct enable_if<true, type_T>
{
    typedef type_T  type;
};

TLU_NAMESPACE_END

#endif /* ENABLE_IF_HPP */
