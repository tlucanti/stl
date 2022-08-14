/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   defs.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tlucanti <tlucanti@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/13 18:39:59 by tlucanti          #+#    #+#             */
/*   Updated: 2022/08/14 12:24:38 by tlucanti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef DEFS_H
# define DEFS_H

// -----------------------------------------------------------------------------
# ifndef WUR
#  define WUR		__attribute__((warn_unused_result))
# endif /* WUR */

// -----------------------------------------------------------------------------
#ifndef UNUSED
# define UNUSED		__attribute__((unused))
# endif

// -----------------------------------------------------------------------------
# ifndef NOEXCEPT
#  define NOEXCEPT	__attribute__((__nothrow__))
# endif /* NOEXCEPT */

// -----------------------------------------------------------------------------
# ifndef LIKELY
#  define LIKELY(__expr)	__builtin_expect((__expr), 1)
# endif /* LIKELY */

// -----------------------------------------------------------------------------
# ifndef UNLIKELY
#  define UNLIKELY(__expr)	__builtin_expect((__expr), 0)
# endif /* UNLIKELY */

// -----------------------------------------------------------------------------
# ifdef __cplusplus
#  define _static_cast(__type, __expr)		static_cast<__type>(__expr)
#  define _reinterpret_cast(__type, __expr)	reinterpret_cast<__type>(__expr)
#  define _const_cast(__type, __expr)		const_cast<__type>(__expr)
// -----------------------------------------------------------------------------
# else
#  define _static_cast(__type, __expr)		(__type)(__expr)
#  define _reinterpret_cast(__type, __expr)	(__type)(__expr)
#  define _const_cast(__type, __expr)		(__type)(__expr)
# endif /* __cplusplus */

#endif /* DEFS_H */
