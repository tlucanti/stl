/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   defs.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tlucanti <tlucanti@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/16 12:09:17 by kostya            #+#    #+#             */
/*   Updated: 2022/03/15 21:30:35 by tlucanti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef DEFS_H
# define DEFS_H

# define __INT_TO_STRING_INTERNAL_MACRO(__x) #__x
# define __INT_TO_STRING_INTERNAL(__x) __INT_TO_STRING_INTERNAL_MACRO(__x)
# define INT_TO_STRING(x) __INT_TO_STRING_INTERNAL(x)
# define STR_LEN(x) ({long __n = 0; for (; (x)[__n]; ++__n); __n;})
# define __ABORT_MESSAGE "[" __PRETTY_FUNCTION__ "::" INT_TO_STRING(__LINE__) \
	"] ABORT : "
# define __ABORT(msg, arg)write(2,__ABORT_MESSAGE,STR_LEN(__ABORT_MESSAGE)+STR_LEN(INT_TO_STRING(__LINE__)))+write(2,msg,STR_LEN(msg))+write(2,": ",2*(STR_LEN(arg)!=0))+write(2,arg,STR_LEN(arg))+write(2,"\n",1);

# if __cplusplus <= 199711L
#  define __WUR __attribute__((warn_unused_result))
#  ifndef noexcept
#   define noexcept throw()
#  endif /* noexcept */
#  ifndef override
#   define override
#  endif /* override */
#  ifndef nullptr
#   define nullptr NULL
#  endif /* nullptr */
#  ifndef constexpr
#   define constexpr
#  endif /* constexpr */
#  define __DEFAULT {}
#  define __UNUSED __attribute__((unused))
#  define __NORET __attribute__((noreturn))
#  define __DELETE ;
#  define __DELETED_MEMBERS private

# else
#  define __WUR [[nodiscard]]
#  define __DEFAULT =default;
#  define __UNUSED [[maybe_unused]]
#  define __NORET [[noreturn]]
#  define __DELETE =delete;
#  define __DELETED_MEMBERS public

# endif /* 199711L */
#endif /* DEFS_H */
