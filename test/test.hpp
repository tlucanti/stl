
#include <iostream>
#include <vector>
#include <memory>
#include <sstream>
#include <cstdio>
#include <csignal>

#define __DEBUG
#define __SAFE_TEST 0

#include "defs.h"
#include "vector.hpp"
#include "color.h"

int all = 0;
int grand_total = 0;
int ok = 0;
int grand_total_ok = 0;
std::string current_test = "null";

template <typename func_T>
void run_test(func_T func)
{
#if __SAFE_TEST
    try {
        func();
    } catch (std::exception &e) {
        std::cout << R "test fall with exception: " Y << e.what() << S << std::endl;
    }
#else
    func();
#endif
}

#undef ASSERT
#define ASSERT(e, msg) do { \
    current_test = msg; \
    ++all; \
    ++grand_total; \
    try { \
        if (not (e)) \
            std::cout << R "assertion fail: " Y << (msg) << S << std::endl; \
        else { \
            ++ok; \
            ++grand_total_ok; \
        } \
    } catch (std::exception &ex) { \
        std::cout << R "task fall with exception: " Y << ex.what() << S << std::endl; \
    } \
} while (0)

#define vec_ASSERT(__vec, __size, __allocated, __begin, __end, __alloc, __msg) \
    ASSERT((__vec).size() == (__size), (__msg) + std::string(" size() check")); \
    ASSERT((__vec).empty() == !(__size), (__msg) + std::string(" empty() check")); \
    ASSERT((__vec)._allocated == (__allocated), (__msg)  + std::string(" allocated check")); \
    ASSERT((__vec).capacity() == (__allocated), (__msg) + std::string(" .capacity() check")); \
    ASSERT((__vec)._begin == (__begin), (__msg) + std::string(" begin iterator check")); \
    ASSERT((__vec)._end == (__end), (__msg) + std::string(" end iterator check")); \
    ASSERT((__vec)._allocator == (__alloc), (__msg) + std::string(" allocator check")); \
    ASSERT((__vec).get_allocator() == (__alloc), (__msg) + std::string(" .get_allocator() check"))

# define vec_cmp(__msg, __type, __vec, ...) do { \
    __type __cmp[] = {__VA_ARGS__}; \
    std::ptrdiff_t __size = sizeof(__cmp) / sizeof(__type); \
    std::stringstream __ssvc, __sscm, __ss; \
    std::ptrdiff_t __vec_size = static_cast<std::ptrdiff_t>((__vec).size()); \
	bool __ok = true; \
    __ok = (__vec_size == __size); \
    __ssvc << "["; \
    __sscm << "["; \
    for (std::ptrdiff_t __i=0; __i < __size; ++__i) \
    { \
        if (__i == 0) \
        { \
            __sscm << __cmp[0]; \
        } \
        else \
        { \
            __sscm << ", " << __cmp[__i]; \
        } \
        if (__ok && (__vec.data())[__i] != __cmp[__i]) \
        { \
            __ok = false; \
        } \
    } \
	for (std::ptrdiff_t __i=0; __i < __vec_size; ++__i) \
    { \
        if (__i == 0) \
        { \
            __ssvc << (__vec)[0]; \
        } \
        else \
        { \
            __ssvc << ", " << (__vec.data())[__i]; \
        } \
        if ((__vec.data())[__i] != __cmp[__i]) \
        { \
            __ok = false; \
        } \
    } \
	__ssvc << "]"; \
    __sscm << "]"; \
    if (not __ok) \
    { \
        __ss << "vector compare: (expected) " << __sscm.str() << " != (got) " << __ssvc.str() << ": "; \
    } \
    ASSERT(__ok, __ss.str() + (__msg)); \
} while (false)

# define make_vec(__name, __type, ...) \
    __type __cmp[] = {__VA_ARGS__}; \
    std::ptrdiff_t __size = sizeof(__cmp) / sizeof(__type); \
    tlucanti::vector_base<__type> __name(__cmp, __cmp + __size)

# define make_std_vec(__name, __type, ...) \
    std::vector<__type> __name; \
    { \
        __type __cmp[] = {__VA_ARGS__}; \
        std::ptrdiff_t __size = sizeof(__cmp) / sizeof(__type); \
        __name.assign(__cmp, __cmp + __size); \
    }

template <typename T>
void __to_trash(T &_) {(void)_;}

# define error_test(expr, message) do { \
    ++all; \
    bool __err = true; \
    try { \
        __to_trash(expr); \
        __err = false; \
    } catch (std::exception &) {} \
    if (not __err) \
        std::cout << message ": didnt raise exception\n"; \
    else ++ok; \
} while (false)

void start(const char *msg) {
    all = 0;
    ok = 0;
    std::cout << P "** results for " << msg << "**" S "\n";
}
void result() {
    if (ok == all)
        std::cout << G "[ OK ] all " C << all << G " tests passed " S "\n\n";
    else
        std::cout << R "[FAIL] " C << ok << " / " << all << R " tests passed\n\n";
}

void final() {
    const char *status;
    const char *c;
    if (grand_total_ok == grand_total) {
        c = G;
        status = "       SUCCESS      ";
    } else if (grand_total_ok >= grand_total * 0.8) {
        c = Y;
        status = "        FAIL        ";
    } else {
        c = R;
        status = "      YOU SUCK      ";
    }
    std::cout << c << "╭────────────────────╮" S "\n";
    std::cout << c << "│" << status << "│" S "\n";
    std::cout << c << "│ " << "TOTAL TESTS PASSED " << c << "│" S "\n";

    printf("%s│     " C "%3d / %3d %s     │" C "\n", c, grand_total_ok, grand_total, c);
    std::cout << c << "╰────────────────────╯" S "\n";
}

void sigsegv_catcher(UNUSED(int sig))
{
    throw std::runtime_error("test fall with SIGSEGV");
}

void sigill_cathcer(UNUSED(int sig))
{
    throw std::runtime_error("test fall with SIGILL");
}

void sigabrt_catcher(UNUSED(int sig))
{
    throw std::runtime_error("test fall with SIGABRT");
}

# define vec_123(__vec) tlucanti::vector_base<int> __vec(3); (__vec)[0] = 1; (__vec)[1] = 2; (__vec)[2] = 3
# define vec_111(__vec) tlucanti::vector_base<int> __vec(3); (__vec)[0] = 123; (__vec)[1] = 456; (__vec)[2] = 789
# define std_vec_123(__vec) std::vector<int> __vec(3); (__vec)[0] = 1; (__vec)[1] = 2; (__vec)[2] = 3
# define std_vec_111(__vec) std::vector<int> __vec(3); (__vec)[0] = 123; (__vec)[1] = 456; (__vec)[2] = 789
# define const_vec_123(__vec) vec_123(GLUE2(__vec_non_const_macro__, __vec)); \
    const tlucanti::vector_base<int> __vec = GLUE2(__vec_non_const_macro__, __vec)
# define const_vec_111(__vec) vec_111(GLUE2(__vec_non_const_macro__, __vec)); \
    const tlucanti::vector_base<int> __vec = GLUE2(__vec_non_const_macro__, __vec)

template<class V>
std::string _vec_to_str(const V &v)
{
    std::stringstream ss;
    for (std::ptrdiff_t i=0; i < static_cast<std::ptrdiff_t>(v.size()); ++i)
    {
        if (i == 0)
        {
            ss << '[' << v.data()[0];
        }
        else
        {
            ss << ", " << v.data()[i];
        }
    }
    ss << ']';
    return ss.str();
}

template<class v1T, class v2T>
void std_vec_cmp(const v1T &expected, const v2T &got, const std::string &msg)
{

    bool _ok = true;
    std::string sexp, sgot;
    sexp = _vec_to_str(expected);
    sgot = _vec_to_str(got);
    std::string outmsg;
    if (static_cast<std::ptrdiff_t>(expected.size()) != static_cast<std::ptrdiff_t>(got.size()))
    {
        _ok = false;
        outmsg = "vector size mismatch: ";
        goto _ASSERT_LABEL;
    }
    for (std::ptrdiff_t i=0; i < static_cast<std::ptrdiff_t>(expected.size()); ++i)
    {
        if (expected.data()[i] != got.data()[i])
        {
            _ok = false;
            outmsg = "vector compare: ";
            goto _ASSERT_LABEL;
        }
    }
    _ASSERT_LABEL:
    ASSERT(_ok, outmsg + "(expected) " + sexp + " != (got) " + sgot + ": " + msg);
}


typedef enum e_colors
{
    Green,
    Cyan,
    Red,
    Blue,
    Purple,
    Black
} colors;

struct ColString
{
    std::string _str;
    std::string _col;
    colors _col_e;
    int _cnt;
    ColString(const std::string &str, colors col, int cnt=1)
    {
        _cnt = cnt;
        _str = str;
        _col_e = col;
        switch (col)
        {
            case Green: _col = G; break;
            case Cyan: _col = C; break;
            case Red: _col = R; break;
            case Blue: _col = B; break;
            case Purple: _col = P; break;
            case Black: _col = B; break;
        }
    }

    ColString operator *(int i)
    {
        return ColString(_str, _col_e, i);
    }

    std::string str() const
    {
        std::stringstream ss;
        ss << _col << _str;
        if (_cnt > 1)
            ss << "(x" << _cnt << ')';
        ss << Y;
        return ss.str();
    }

    bool operator ==(const ColString &cmp) const
    {
        return _col_e == cmp._col_e && _cnt == cmp._cnt;
    }

    bool operator !=(const ColString &cmp) const
    {
        return not (*this == cmp);
    }

    bool cmp(const ColString &cmp) const
    {
        return _str == cmp._str;
    }
};

std::ostream &operator <<(std::ostream &out, const ColString &s)
{
    out << s.str();
    return out;
}

ColString Def("def", Green); // = G + std::string("def") + Y;
ColString Cons("cons", Cyan); // = C + std::string("cons") + Y;
ColString Del("del", Red); // = R + std::string("del") + Y;
ColString Cpy("cpy", Blue); // = C + std::string("cpy") + Y;
ColString Icpy("icpy", Blue); // = B + std::string("icpy") + Y;
ColString Mv("mv", Purple); // = P + std::string("mv") + Y;
ColString Imv("imv", Black); // = K + std::string("imv") + Y;

std::vector<ColString> moves;
std::vector<ColString> moves_std;
void convolve(std::vector<ColString> &v)
{
    if (v.empty())
        return ;
    ColString last = v[0];
    int cnt = 0;
    std::vector<ColString> out;
    for (std::size_t i=0; i < v.size(); ++i)
    {
        if (v[i].cmp(last))
            cnt += v[i]._cnt;
        else
        {
            out.push_back(last * cnt);
            cnt = v[i]._cnt;
            last = v[i];
        }
    }
    out.push_back(last * cnt);
    v.swap(out);
}

struct UserClass
{
    int a;
    int b;
    bool *valid;

    static int c;
    static int total_instances;
    static bool verbose;
    static bool monitoring;

    int my_c;
    UserClass() : a(111), b(222) {
        valid = new(bool);
        *valid = true;
        my_c = ++c;
        ++total_instances;
        if (monitoring) {
            moves.push_back(Def);
            convolve(moves);
        }
        if (verbose)
            std::cout << str() << G " default" S "\n";
    }
    UserClass(int _a, int _b) : a(_a), b(_b) {
        valid = new(bool);
        *valid = true;
        my_c = ++c;
        ++total_instances;
        if (monitoring) {
            moves.push_back(Cons);
            convolve(moves);
        }
        if (verbose)
            std::cout << str() << C " constructor" S "\n";
    }
    ~UserClass() noexcept(false) {
        if (!valid)
            throw std::runtime_error("nullptr in `valid` value");
        if (not *valid)
            throw std::runtime_error("invalid class");
        delete valid;
        --total_instances;
        if (monitoring) {
            moves.push_back(Del);
            convolve(moves);
        }
        if (verbose)
            std::cout << str() << R " destructor" S "\n";
    }
    WUR const char *cl() const {
        switch (my_c % 7) {
            case 0: return K;
            case 1: return R;
            case 2: return G;
            case 3: return Y;
            case 4: return B;
            case 5: return P;
            case 6: return C;
            default: return W;
        }
    }
    UserClass(const UserClass &cpy) : a(cpy.a), b(cpy.b) {
        if (!cpy.valid)
            throw std::runtime_error("nullptr in copy `valid` value");
        if (not *cpy.valid)
            throw std::runtime_error("invalid copy class");
        valid = new(bool);
        *valid = true;
        my_c = ++c;
        ++total_instances;
        if (monitoring) {
            moves.push_back(Cpy);
            convolve(moves);
        }
        if (verbose)
            std::cout << str() << Y " copy" S "\n";
    }
    UserClass &operator =(const UserClass &cpy) {
        if (!cpy.valid)
            throw std::runtime_error("nullptr in copy `valid` value");
        if (not *cpy.valid)
            throw std::runtime_error("invalid copy class");
        if (!valid)
            throw std::runtime_error("nullptr in `valid` value");
        if (not *valid)
            throw std::runtime_error("invalid class");
        a = cpy.a;
        b = cpy.b;
        valid = new(bool);
        *valid = true;
        my_c = cpy.my_c;
        if (monitoring) {
            moves.push_back(Icpy);
            convolve(moves);
        }
        if (verbose)
            std::cout << str() << C " copy assign" S "\n";
        return *this;
    }

    std::string str()
    {
        std::stringstream ss;
        ss << cl() << '[' << this << '(' << my_c << ")]:";
        return ss.str();
    }
//    UserClass(UserClass &&mv) noexcept : a(mv.a), b(mv.b) {
//        my_c = mv.my_c;
//        valid = mv.valid;
//        mv.valid = nullptr;
//        ++total_instances;
//        if (monitoring) {
//            moves.push_back(Mv);
//            convolve(moves);
//        }
//        if (verbose)
//            std::cout << cl() << "[" << this << "]:" P " move" S "\n";
//    }
//    UserClass &operator =(UserClass &&mv) noexcept {
//        a = mv.a;
//        b = mv.b;
//        valid = mv.valid;
//        mv.valid = nullptr;
//        my_c = mv.my_c;
//        ++total_instances;
//        if (monitoring) {
//            moves.push_back(Imv);
//            convolve(moves);
//        }
//        if (verbose)
//            std::cout << cl() << "[" << this << "]:" K " move assign" S "\n";
//        return *this;
//    }
DELETED_MEMBERS:
//    UserClass(const UserClass &) __DELETE
//    UserClass &operator =(const UserClass &) __DELETE
#if CPP11
//    UserClass(UserClass &&) DELETE
//    UserClass &operator =(UserClass &&) DELETE
#endif /* CPP11 */
};
int UserClass::c = 3;
int UserClass::total_instances = 0;
bool UserClass::verbose = false;
bool UserClass::monitoring = false;

# define vec_cmp_lock(__msg, __type, __vec, ...) \
    UserClass::monitoring = false; \
    vec_cmp(__msg, __type, __vec, __VA_ARGS__); \
    UserClass::monitoring = true

# define make_std_vec_lock(__name, __type, ...) \
    UserClass::monitoring = false; \
    make_std_vec(__name, __type, __VA_ARGS__); \
    UserClass::monitoring = true

#define std_vec_cmp_lock(__v1, __v2, __msg) \
    UserClass::monitoring = false; \
    std_vec_cmp(__v1, __v2, __msg); \
    UserClass::monitoring = true
