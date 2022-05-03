
#ifndef IS_INTEGRAL_HPP
# define IS_INTEGRAL_HPP

# include "defs.h"

TLU_NAMESPACE_BEGIN

template <class type_T>
struct is_integral : std::false_type {};

template <> struct is_integral<bool>            : std::true_type {};
template <> struct is_integral<char>            : std::true_type {};
#if CPP20
template <> struct is_integral<char8_t>         : std::true_type {};
#endif
template <> struct is_integral<char16_t>        : std::true_type {};
template <> struct is_integral<char32_t>        : std::true_type {};
template <> struct is_integral<wchar_t>         : std::true_type {};
template <> struct is_integral<short int>       : std::true_type {};
template <> struct is_integral<int>             : std::true_type {};
template <> struct is_integral<long int>        : std::true_type {};
template <> struct is_integral<long long int>   : std::true_type {};

template <> struct is_integral<unsigned char>            : std::true_type {};
template <> struct is_integral<unsigned short int>       : std::true_type {};
template <> struct is_integral<unsigned int>             : std::true_type {};
template <> struct is_integral<unsigned long int>        : std::true_type {};
template <> struct is_integral<unsigned long long int>   : std::true_type {};

TLU_NAMESPACE_END

#endif /* IS_INTEGRAL_HPP */