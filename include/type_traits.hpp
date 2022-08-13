/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   type_traits.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tlucanti <tlucanti@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/13 15:56:23 by tlucanti          #+#    #+#             */
/*   Updated: 2022/08/13 19:00:39 by tlucanti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TYPE_TRAITS_HPP
# define TYPE_TRAITS_HPP

# include "defs.hpp"

# define IS_INTEGRAL(__type) typename TLU_NAMESPACE::enable_if< \
    TLU_NAMESPACE::is_integral<__type>::value, __type>::type
# define NOT_INTEGRAL(__type) typename TLU_NAMESPACE::enable_if< \
    not TLU_NAMESPACE::is_integral<__type>::value, __type>::type

# define IS_ITERATOR(__type) typename enable_if< \
    TLU_NAMESPACE::is_iterator<__type>::value, __type>::type
# define NOT_ITERATOR(__type) typename enable_if< \
    not TLU_NAMESPACE::is_iterator<__type>::value, __type>::type

# define IS_POINTER(__type) typename TLU_NAMESPACE::enable_if< \
    TLU_NAMESPACE::is_pointer<__type>::value, __type>::type
# define NOT_POINTER(__type) typename TLU_NAMESPACE::enable_if< \
    not TLU_NAMESPACE::is_pointer<__type>::value, __type>::type

# define HAS_SIZE_TYPE(__type) typename TLU_NAMESPACE::enable_if < \
    TLU_NAMESPACE::has_size_type<__type>::type, __type>::type
# define  HAS_NOT_SIZE_TYPE(__type) typename TLU_NAMESPACE::enable_if < \
    not TLU_NAMESPACE::has_size_type<__type>::type, __type>::type

TLU_NAMESPACE_BEGIN

// ----------------------------- integral constant -----------------------------

    template<typename type_T, type_T type_value>
    struct integral_constant
    {
        const static constexpr type_T       value = type_value;
    };

    typedef integral_constant<bool, true>   true_type;
    typedef integral_constant<bool, false>  false_type;

// ---------------------------------- is_same ----------------------------------
    template <class type_T, class type_Y> struct is_same    : public false_type
    {};
    template <class type_T> struct is_same<type_T, type_T>  : public true_type
    {};

// --------------------------------- enable_if ---------------------------------
    template <bool term, class type_T = void>
    struct enable_if
    {};

    template <class type_T>
    struct enable_if<true, type_T>
    {
        typedef type_T  type;
    };

// -------------------------------- is_integral --------------------------------
    template <class type_T> struct is_integral          : public false_type {};

    template <> struct is_integral<bool>                : public true_type {};
    template <> struct is_integral<char>                : public true_type {};

    template <> struct is_integral<wchar_t>             : public true_type {};
    template <> struct is_integral<short int>           : public true_type {};
    template <> struct is_integral<int>                 : public true_type {};
    template <> struct is_integral<long int>            : public true_type {};
    template <> struct is_integral<unsigned char>       : public true_type {};
    template <> struct is_integral<unsigned short int>  : public true_type {};
    template <> struct is_integral<unsigned int>        : public true_type {};
    template <> struct is_integral<unsigned long int>   : public true_type {};
#if CPP11
    template <> struct is_integral<char16_t>            : public true_type {};
    template <> struct is_integral<char32_t>            : public true_type {};
    template <> struct is_integral<long long>           : public true_type {};
    template <> struct is_integral<unsigned long long>  : public true_type {};
#endif /* CPP11 */
#if CPP20
    template <> struct is_integral<char8_t>             : public true_type {};
#endif /* CPP20 */

// -------------------------------- is_pointer ---------------------------------
    template <class type_T> struct is_pointer           : public false_type {};
    template <class type_T> struct is_pointer<type_T *> : public true_type {}

// -------------------------------- is_iterator --------------------------------
TLU_NAMESPACE_HIDDEN_BEGIN
    template<typename type_T, typename = void>
    struct _is_iterator_base : public false_type
    {};

    template<typename type_T>
    struct _is_iterator_base<
            type_T,
            typename enable_if<
                !is_same<
                    typename type_T::value_type,
                    void
                >::value
            >::type
        > : public true_type {};
TLU_NAMESPACE_HIDDEN_END

    template <typename type_T>
    struct is_iterator : public TLU_NAMESPACE_HIDDEN::_is_iterator_base<type_T>
    {};

// ------------------------------- remove_const --------------------------------
    template <class type_T>
    struct remove_const
    {
        typedef type_T type;
    };

    template <class type_T>
    struct remove_const<const type_T>
    {
        typedef type_T type;
    };

// ----------------------------- change_pair_type ------------------------------
    template <class T, class type_T, class type_Y> struct change_pair_type {};

// --------------------------------- make_void ---------------------------------
    template<typename>
    struct make_void
    {
        typedef void type;
    };

// ------------------------------- has_size_type -------------------------------
    template <class type_T, typename = void>
    struct has_size_type : false_type
    {};

    template <class type_T>
    struct has_size_type<
            type_T,
            typename make_void<
                typename type_T::size_type
            >::type
        > : true_type
    {};

// ---------------------------- complete_size_type -----------------------------
TLU_NAMESPACE_HIDDEN_BEGIN
    template <class type_T, typename = void>
    struct complete_size_type_base
    {
        typedef bool size_type;
    };

    template <class type_T>
    struct complete_size_type_base<
            type_T,
            typename make_void<
                typename type_T::size_type
            >::type
        >
    {
        typedef typename type_T::size_type size_type;
    };
TLU_NAMESPACE_HIDDEN_END

    template <class type_T>
    struct complete_size_type
        : public TLU_NAMESPACE_HIDDEN::complete_size_type_base<type_T>
    {};

TLU_NAMESPACE_END

#endif /* TYPE_TRAITS_HPP */
