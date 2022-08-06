
#ifndef DEFS_H
# define DEFS_H

# ifndef WUR
#  define WUR __attribute__((warn_unused_result))
# endif /* WUR */

# ifndef NOEXCEPT
#  define NOEXCEPT __attribute__((__nothrow__))
# endif /* NOEXCEPT */

# ifndef LIKELY
#  define LIKELY(__expr) __builtin_expect((__expr), 1)
# endif /* LIKELY */

# ifndef UNLIKELY
#  define UNLIKELY(__expr) __builtin_expect((__expr), 0)
# endif /* UNLIKELY */

#endif /* DEFS_H */