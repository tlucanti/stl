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

# define CPP	1
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

/* internal */# define ___INTERNAL___INT_TO_STRING_INTERNAL_MACRO(__x)#__x
/* internal */# define ___INTERNAL___INT_TO_STRING_INTERNAL(__x)___INTERNAL___INT_TO_STRING_INTERNAL_MACRO(__x)
# define INT_TO_STRING(x)___INTERNAL___INT_TO_STRING_INTERNAL(x)
# define STR_LEN(x)({long __n=0;for(;(x)[__n];++__n);__n;})
/* internal */# define ___INTERNAL___ABORT_MESSAGE"["__PRETTY_FUNCTION__"::"INT_TO_STRING(__LINE__)"] ABORT : "
# define __ABORT(msg, arg)write(2,___INTERNAL___ABORT_MESSAGE,STR_LEN(___INTERNAL___ABORT_MESSAGE)+STR_LEN(INT_TO_STRING(__LINE__)))+write(2,msg,STR_LEN(msg))+write(2,": ",2*(STR_LEN(arg)!=0))+write(2,arg,STR_LEN(arg))+write(2,"\n",1);

# ifdef CPP11
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
# endif /* PRECPP11 */

# ifdef PRECPP14
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
# endif /* CPP17 */

#endif /* DEFS_H */
