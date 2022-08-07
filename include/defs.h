
#ifndef DEFS_H
# define DEFS_H

# ifndef WUR
#  define WUR __attribute__((warn_unused_result))
# endif /* WUR */

#ifndef UNUSED
# define UNUSED(__x) __attribute__((unused)) __x
# endif

# ifndef NOEXCEPT
#  define NOEXCEPT __attribute__((__nothrow__))
# endif /* NOEXCEPT */

# ifndef LIKELY
#  define LIKELY(__expr) __builtin_expect((__expr), 1)
# endif /* LIKELY */

# ifndef UNLIKELY
#  define UNLIKELY(__expr) __builtin_expect((__expr), 0)
# endif /* UNLIKELY */

# ifdef __cplusplus
#  define static_cast(__type, __expr) static_cast<__type>(__expr)
#  define reinterpret_cast(__type, __expr) reinterpret_cast<__type>(__expr)
#  define const_cast(__type, __expr) const_cast<__type>(__expr)
# else
#  define static_cast(__type, __expr) (__type)(__expr)
#  define reinterpret_cast(__type, __expr) (__type)(__expr)
#  define const_cast(__type, __expr) (__type)(__expr)
# endif /* __cplusplus */

#endif /* DEFS_H */
