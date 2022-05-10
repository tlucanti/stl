/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   defs.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tlucanti <tlucanti@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/16 12:09:17 by kostya            #+#    #+#             */
/*   Updated: 2022/03/16 12:58:39 by tlucanti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef DEFS_H
# define DEFS_H

# include <unistd.h>
# include <string.h>

# ifdef __DEBUG
#  define PRIVATE public
#  define PROTECTED public
# else
#  define PRIVATE private
#  define PROTECTED protected
# endif

# define CPP	    1
# define CPP98		__cplusplus >= 199711L
# define CPP11		__cplusplus >= 201103L
# define CPP14		__cplusplus >= 201402L
# define CPP17		__cplusplus >= 201703L
# define CPP20		__cplusplus >= 202002L
# define CPP23		__cplusplus >= 202003L
# define PRECPP98	__cplusplus <  199711L
# define PRECPP11	__cplusplus <  201103L
# define PRECPP14	__cplusplus <  201402L
# define PRECPP17	__cplusplus <  201703L
# define PRECPP20	__cplusplus <  202002L
# define PRECPP23	1

# define TLU_NAMESPACE tlucanti
# define TLU_NAMESPACE_BEGIN namespace TLU_NAMESPACE {
# define TLU_NAMESPACE_END }
# define TLU_NAMESPACE_HIDDEN __hidden_tlucanti_namespace__
# define TLU_NAMESPACE_HIDDEN_BEGIN namespace TLU_NAMESPACE_HIDDEN {
# define TLU_NAMESPACE_HIDDEN_END }

# define GLUE2(__a, __b) __a##__b
# define GLUE3(__a, __b, __c) GLUE2(__a, __b)##__c
# define GLUE4(__a, __b, __c, __d) GLUE3(__a, __b, __c)##__d
# define GLUE5(__a, __b, __c, __d, __e) GLUE4(__a, __b, __c, __d)##__e

# /* internal */ define ___INTERNAL___INT_TO_STRING_MACRO(__x)  #__x
# /* internal */ define ___INTERNAL___INT_TO_STRING(__x)        ___INTERNAL___INT_TO_STRING_MACRO(__x)
# define INT_TO_STRING(x)   ___INTERNAL___INT_TO_STRING(x)
# define STR_LEN(x) strlen(x)

# define __ABORT(msg, arg) __do_ABORT(msg, arg, __PRETTY_FUNCTION__, INT_TO_STRING(__LINE__))

inline void __do_ABORT(const char *msg, const char *arg, const char *func, const char *lineno)
{
    write(2, "[", 1);
//    write(2, func, STR_LEN(func));
    while (not isspace(*func))
        ++func;
    while (isspace(*func))
        ++func;
    while (isalnum(*func) or *func == '_' or *func == ':')
        write(2, func++, 1);
    write(2, "::", 2);
    write(2, lineno, STR_LEN(lineno));
    write(2, "] ABORT : ", 10);
    write(2, msg, STR_LEN(msg));
    if (STR_LEN(arg) > 0)
    {
        write(2, ": ", 2);
        write(2, arg, STR_LEN(arg));
    }
    write(2, "\n", 1);
    abort();
}

# if CPP11
#  ifndef __DEFAULT
#   define __DEFAULT =default;
#  endif /* __DEFAULT */
#  ifndef __DELETE
#   define __DELETE =delete;
#  endif /* __DELETE */
#  ifndef __DELETED_MEMBERS
#   define __DELETED_MEMBERS public
#  endif /* __DELETED_MEMBERS */
# else /* PRECPP11 */
#  ifndef __DEFAULT
#   define __DEFAULT {}
#  endif /* __DEFAULT */
#  ifndef __DELETE
#   define __DELETE ;
#  endif /* __DELETE */
#  ifndef __DELETED_MEMBERS
#   define __DELETED_MEMBERS private
#  endif /* __DELETED_MEMBERS */
#  ifndef noexcept
#   define noexcept throw()
#  endif /* noexcept */
#  ifndef override
#   define override
#  endif /* override */
#  ifndef nullptr
#   define nullptr NULL
#  endif /* nullptr */
# endif /* CPP11 */

# if PRECPP14
#  ifndef constexpr
#   define constexpr
#  endif /* constexpr */
# endif /* PRECPP14 */

# if CPP17
#  ifndef __WUR
#   define __WUR [[nodiscard]]
#  endif /* __WUR */
#  ifndef __UNUSED
#   define __UNUSED [[maybe_unused]]
#  endif /* __UNUSED */
#  ifndef __NORET
#   define __NORET [[noreturn]]
#  endif /* __NORET */
# else /* PRECPP17 */
#  ifndef __WUR
#   define __WUR __attribute__((warn_unused_result))
#  endif /* __WUR */
#  ifndef __UNUSED
#   define __UNUSED __attribute__((unused)))
#  endif /* __UNUSED */
#  ifndef __NORET
#   define __NORET __attribute__((noreturn))
#  endif /* __NORET */
# endif /* CPP17 */

#endif /* DEFS_H */
