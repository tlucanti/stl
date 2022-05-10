
#ifndef TYPE_TRAITS_HPP
# define TYPE_TRAITS_HPP

# include "defs.h"

TLU_NAMESPACE_BEGIN

// ------------------------------- is_same -------------------------------
    template <class type_T, class type_Y>   struct is_same                  : public std::false_type {};
    template <class type_T>                 struct is_same<type_T, type_T>  : public std::true_type {};

// ------------------------------- enable_if -------------------------------
    template <bool term, class type_T = void> struct enable_if {};
    template <class type_T> struct enable_if<true, type_T>
    {
        typedef type_T  type;
    };

// ------------------------------- is_integral -------------------------------
    template <class type_T> struct is_integral : public std::false_type {};

    template <> struct is_integral<bool>            : public std::true_type {};
    template <> struct is_integral<char>            : public std::true_type {};
#if CPP20
    template <> struct is_integral<char8_t>         : public std::true_type {};
#endif
    template <> struct is_integral<char16_t>        : public std::true_type {};
    template <> struct is_integral<char32_t>        : public std::true_type {};
    template <> struct is_integral<wchar_t>         : public std::true_type {};
    template <> struct is_integral<short int>       : public std::true_type {};
    template <> struct is_integral<int>             : public std::true_type {};
    template <> struct is_integral<long int>        : public std::true_type {};
#if CPP11
    template <> struct is_integral<long long int>   : public std::true_type {};
#endif /* CPP11 */

// ------------------------------- is_integral -------------------------------
    template <> struct is_integral<unsigned char>            : public std::true_type {};
    template <> struct is_integral<unsigned short int>       : public std::true_type {};
    template <> struct is_integral<unsigned int>             : public std::true_type {};
    template <> struct is_integral<unsigned long int>        : public std::true_type {};
#if CPP11
    template <> struct is_integral<unsigned long long int>   : public std::true_type {};
#endif /* CPP11 */

// ------------------------------- is_iterator -------------------------------
TLU_NAMESPACE_HIDDEN_BEGIN
    template<typename type_T, typename = void> struct _is_iterator_base : public std::false_type {};

    template<typename type_T>
    struct _is_iterator_base<type_T,
            typename enable_if<!is_same<
                typename std::iterator_traits<type_T>::value_type,
                void>::value>::type> : public std::true_type {};
TLU_NAMESPACE_HIDDEN_END

    template <typename type_T> struct is_iterator : public TLU_NAMESPACE_HIDDEN::_is_iterator_base<type_T> {};

TLU_NAMESPACE_END

#endif /* TYPE_TRAITS_HPP */