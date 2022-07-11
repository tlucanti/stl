
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

void constructor_test();
void fn_assign_test();
void fn_get_allocator_test();
void fn_at_test();
void access_operator_test();
void fn_front_test();
void fn_back_test();
void fn_data_test();
void iterator_test();
void reverse_iterator_test();
void fn_empty_tests();
void fn_size_tests();
void fn_max_size_tests();
void fn_reserve_tests();
void fn_capacity_tests();
void fn_shrink_to_fit_tests();
void fn_clear_tests();
void fn_insert_tests();
void fn_emplace_tests();
void fn_erase_tests();
void fn_push_back_tests();
void fn_emplace_back_tests();
void fn_pop_back_tests();
void fn_resize_test();
void swap_tests();

void std_vector_test();
void user_type_constructor_test();

int main()
{
	signal(SIGSEGV, sigsegv_catcher);
    signal(SIGILL, sigill_cathcer);
    signal(SIGABRT, sigabrt_catcher);

	run_test(constructor_test);
    run_test(fn_assign_test);
    run_test(fn_get_allocator_test);
    run_test(fn_at_test);
    run_test(access_operator_test);
    run_test(fn_front_test);
    run_test(fn_back_test);
    run_test(fn_data_test);
    run_test(iterator_test);
    run_test(reverse_iterator_test);
    run_test(fn_empty_tests);
    run_test(fn_empty_tests);
    run_test(fn_size_tests);
    run_test(fn_max_size_tests);
    run_test(fn_reserve_tests);
    run_test(fn_capacity_tests);
    run_test(fn_shrink_to_fit_tests);
    run_test(fn_clear_tests);
    run_test(fn_insert_tests);
    run_test(fn_emplace_tests);
    run_test(fn_erase_tests);
    run_test(fn_push_back_tests);
    run_test(fn_emplace_back_tests);
    run_test(fn_pop_back_tests);
    run_test(fn_resize_test);
    run_test(swap_tests);

    run_test(std_vector_test);
    run_test(user_type_constructor_test);

    final();
}

void constructor_test()
{
    start("constructor tests");
    std::vector<int> vec;

    std::allocator<int> alloc;
    {
        tlucanti::vector_base<int> a;
        vec_ASSERT(a, 0, 0, nullptr, nullptr, alloc, "default constructor test");
    }
    {
        tlucanti::vector_base<int> a(alloc);
        vec_ASSERT(a, 0, 0, nullptr, nullptr, alloc, "default constructor + alloc test");
    }
    {
        tlucanti::vector_base<int> a(3);
        vec_ASSERT(a, 3, 7, a._begin, a._end, alloc, "default size constructor test");
        ASSERT(a._end - a._begin == 3, "size 3 vec iterator test 1");
    }
    {
        tlucanti::vector_base<int> a(3, alloc);
        vec_ASSERT(a, 3, 7, a._begin, a._end, alloc, "default size + alloc constructor test");
        ASSERT(a._end - a._begin == 3, "default size iterator test 1");
    }
    {
        tlucanti::vector_base<int> a(3, 123);
        vec_ASSERT(a, 3, 7, a._begin, a._end, alloc, "constructor default value test");
        ASSERT(a._end - a._begin == 3, "constructor default value iterator test 1");
        ASSERT(a[0] == 123, "constructor default value test 1");
        ASSERT(a[1] == 123, "constructor default value test 2");
        ASSERT(a[2] == 123, "constructor default value test 3");
    }
    {
        tlucanti::vector_base<int> a(3, 123, alloc);
        vec_ASSERT(a, 3, 7, a._begin, a._end, alloc, "constructor default value + alloc test");
        ASSERT(a._end - a._begin == 3, "constructor default value + alloc iterator test 1");
        ASSERT(a[0] == 123, "constructor default value + alloc test 1");
        ASSERT(a[1] == 123, "constructor default value + alloc test 2");
        ASSERT(a[2] == 123, "constructor default value + alloc test 3");
    }
    {
        vec_123(v);
        std_vec_123(stdv);
        {
            tlucanti::vector_base<int> a(v.begin(), v.end());
            vec_ASSERT(a, 3, 7, a._begin, a._end, alloc, "constructor iterator copy test 1");
            ASSERT(a._end - a._begin == 3, "constructor iterator copy iterator test 1");
            ASSERT(a[0] == 1, "constructor iterator copy test 1");
            ASSERT(a[1] == 2, "constructor iterator copy test 2");
            ASSERT(a[2] == 3, "constructor iterator copy test 3");
        } {
            tlucanti::vector_base<int> a(v.data(), v.data() + v.size());
            vec_ASSERT(a, 3, 7, a._begin, a._end, alloc, "constructor iterator copy test 2");
            ASSERT(a._end - a._begin == 3, "constructor iterator copy iterator test 2");
            ASSERT(a[0] == 1, "constructor iterator copy test 4");
            ASSERT(a[1] == 2, "constructor iterator copy test 5");
            ASSERT(a[2] == 3, "constructor iterator copy test 6");
        } {
            tlucanti::vector_base<int> a(stdv.begin(), stdv.end());
            vec_ASSERT(a, 3, 7, a._begin, a._end, alloc, "constructor iterator copy test 3");
            ASSERT(a._end - a._begin == 3, "constructor iterator copy iterator test 3");
            ASSERT(a[0] == 1, "constructor iterator copy test 7");
            ASSERT(a[1] == 2, "constructor iterator copy test 8");
            ASSERT(a[2] == 3, "constructor iterator copy test 9");
        }
    }
    {
        vec_123(v);
        tlucanti::vector_base<int> a(v.begin(), v.end(), alloc);
        vec_ASSERT(a, 3, 7, a._begin, a._end, alloc, "constructor iterator copy + alloc test");
        ASSERT(a._end - a._begin == 3, "constructor iterator copy + alloc iterator test 1");
        ASSERT(a[0] == 1, "constructor iterator copy + alloc test 1");
        ASSERT(a[1] == 2, "constructor iterator copy + alloc test 2");
        ASSERT(a[2] == 3, "constructor iterator copy + alloc test 3");
    }
    {
        vec_123(v);
        tlucanti::vector_base<int> a(v);
        vec_ASSERT(a, 3, 7, a._begin, a._end, alloc, "copy constructor test");
        ASSERT(a._end - a._begin == 3, "copy constructor iterator test 1");
        ASSERT(a[0] == 1, "copy constructor test 1");
        ASSERT(a[1] == 2, "copy constructor test 2");
        ASSERT(a[2] == 3, "copy constructor test 3");
    }
    {
        vec_123(v);
        tlucanti::vector_base<int> a(v, alloc);
        vec_ASSERT(a, 3, 7, a._begin, a._end, alloc, "copy constructor + alloc test");
        ASSERT(a._end - a._begin == 3, "copy constructor + alloc iterator test 1");
        ASSERT(a[0] == 1, "copy constructor + alloc test 1");
        ASSERT(a[1] == 2, "copy constructor + alloc test 2");
        ASSERT(a[2] == 3, "copy constructor + alloc test 3");
    }
#if CPP11
    {
        vec_123(v);
        tlucanti::vector_base<int> a(std::move(v));
        vec_ASSERT(a, 3, 7, a._begin, a._end, alloc, "move constructor test");
        ASSERT(a._end - a._begin == 3, "move constructor iterator test 1");
        ASSERT(a[0] == 1, "move constructor test 1");
        ASSERT(a[1] == 2, "move constructor test 2");
        ASSERT(a[2] == 3, "move constructor test 3");
        vec_ASSERT(v, 0, 0, nullptr, nullptr, alloc, "move constructor empty vector test");
    }
    {
        vec_123(v);
        tlucanti::vector_base<int> a(std::move(v), alloc);
        vec_ASSERT(a, 3, 7, a._begin, a._end, alloc, "move + alloc constructor test");
        ASSERT(a._end - a._begin == 3, "move + alloc constructor iterator test 1");
        ASSERT(a[0] == 1, "move + alloc constructor test 1");
        ASSERT(a[1] == 2, "move + alloc constructor test 2");
        ASSERT(a[2] == 3, "move + alloc constructor test 3");
        vec_ASSERT(v, 0, 0, nullptr, nullptr, alloc, "move + alloc constructor empty vector test");
    }
    {
        tlucanti::vector_base<int> a({1, 2, 3});
        vec_ASSERT(a, 3, 7, a._begin, a._end, alloc, "initializer list constructor test");
        ASSERT(a._end - a._begin == 3, "initializer list constructor iterator test 1");
        ASSERT(a[0] == 1, "initializer list constructor test 1");
        ASSERT(a[1] == 2, "initializer list constructor test 2");
        ASSERT(a[2] == 3, "initializer list constructor test 3");
    }
    {
        tlucanti::vector_base<int> a({1, 2, 3}, alloc);
        vec_ASSERT(a, 3, 7, a._begin, a._end, alloc, "initializer list + alloc constructor test");
        ASSERT(a._end - a._begin == 3, "initializer list + alloc constructor iterator test 1");
        ASSERT(a[0] == 1, "initializer list + alloc constructor test 1");
        ASSERT(a[1] == 2, "initializer list + alloc constructor test 2");
        ASSERT(a[2] == 3, "initializer list + alloc constructor test 3");
    }
#endif /* CPP11 */

    std::vector<int> av;
    std::vector<int> bv(alloc);
    std::vector<int> cv(3);
#if CPP11
    std::vector<int> dv(3, alloc);
#endif /* CPP11 */
    std::vector<int> ev(3, 123);
    std::vector<int> fv(3, 123, alloc);
    std::vector<int> gv(vec.begin(), vec.end());
    std::vector<int> hv(vec.begin(), vec.end(), alloc);
    std::vector<int> iv(av); (void)iv;
#if CPP11
    std::vector<int> jv(av, alloc);
    std::vector<int> kv(std::move(av));
    std::vector<int> lv(std::move(bv), alloc);
    std::vector<int> mv({1, 2, 3});
    std::vector<int> nv({1, 2, 3}, alloc);
#endif /* CPP11 */

    result();
}

void fn_assign_test()
{
    start(".assign() tests");

    {
        vec_123(a);
        a.assign(0, 0);
        ASSERT(a.empty(), "sized assign test 0");
    }
    {
        vec_123(a);
        a.assign(3, 1);
        vec_cmp("sized assign test 1", int, a, 1, 1, 1);
    }

    {
        vec_123(a);
        tlucanti::vector_base<int> b;
        a.assign(b.begin(), b.end());
        ASSERT(a.empty(), "iterator assign test 0");
    }
    {
        std_vec_123(a);
        tlucanti::vector_base<int> b;
        b.assign(a.begin(), a.end());
        vec_cmp("iterator assign test 1", int, b, 1, 2, 3);
    }


    result();
}

void fn_get_allocator_test()
{
    start(".get_allocator() tests");

    {
        std::allocator<int> alloc;
        tlucanti::vector_base<int> a(alloc);
        ASSERT(alloc == a.get_allocator(), "get allocator basic test 0");
    }
    {
        std::allocator<std::string> alloc;
        tlucanti::vector_base<std::string> a(5, alloc);
        ASSERT(alloc == a.get_allocator(), "get allocator basic test 1");
    }

    result();
}

#undef _op
# define _op(vec, _a) vec.at(_a)
void fn_at_test()
{
    start(".at() tests");

    vec_123(a);
    ASSERT(_op(a, 0) == 1, "basic access test 1");
    ASSERT(_op(a, 1) == 2, "basic access test 2");
    ASSERT(_op(a, 2) == 3, "basic access test 3");
    ASSERT(_op(a, -1) == 3, "reverse access test 1");
    ASSERT(_op(a, -2) == 2, "reverse access test 2");
    ASSERT(_op(a, -3) == 1, "reverse access test 3");

    error_test(_op(a, 3), "out of bound test 1");
    error_test(_op(a, 4), "out of bound test 2");
    error_test(_op(a, 123123), "out of bound test 3");
    error_test(_op(a, -4), "reverse out of bound test 1");
    error_test(_op(a, -5), "reverse out of bound test 2");
    error_test(_op(a, -123123), "reverse out of bound test 3");

    tlucanti::vector_base<int> b;
    error_test(_op(b, 0), "empty access test 1");
    error_test(_op(b, 1), "empty access test 2");
    error_test(_op(b, -1), "empty access test 3");
    error_test(_op(b, -2), "empty access test 4");

    result();
}

#undef _op
# define _op(vec, _a) vec[_a]
void access_operator_test()
{
    start("operator[] tests");

    vec_123(a);
    ASSERT(_op(a, 0) == 1, "basic access test 1");
    ASSERT(_op(a, 1) == 2, "basic access test 2");
    ASSERT(_op(a, 2) == 3, "basic access test 3");
    ASSERT(_op(a, -1) == 3, "reverse access test 1");
    ASSERT(_op(a, -2) == 2, "reverse access test 2");
    ASSERT(_op(a, -3) == 1, "reverse access test 3");

    error_test(_op(a, 3), "out of bound test 1");
    error_test(_op(a, 4), "out of bound test 2");
    error_test(_op(a, 123123), "out of bound test 3");
    error_test(_op(a, -4), "reverse out of bound test 1");
    error_test(_op(a, -5), "reverse out of bound test 2");
    error_test(_op(a, -123123), "reverse out of bound test 3");

    tlucanti::vector_base<int> b;
    error_test(_op(b, 0), "empty access test 1");
    error_test(_op(b, 1), "empty access test 2");
    error_test(_op(b, -1), "empty access test 3");
    error_test(_op(b, -2), "empty access test 4");

    result();
}

#undef _op
#undef fn
# define _op(vec) vec.front()
# define fn "front"
void fn_front_test()
{
    start(".front() tests");

    vec_123(a);
    
    ASSERT(_op(a) == 1, "basic " fn " test 1");
    
    tlucanti::vector_base<int> b;

    error_test(_op(b), "empty" fn "test 1");

    result();
}

#undef _op
#undef fn
# define _op(vec) vec.back()
# define fn "back"
void fn_back_test()
{
    start(".back() tests");

    vec_123(a);
    
    ASSERT(_op(a) == 3, "basic " fn " test 1");
    
    tlucanti::vector_base<int> b;

    error_test(_op(b), "empty" fn "test 1");

    result();
}

#undef _op
# define _op(vec) vec.data()
void fn_data_test()
{
    start(".data() tests");

    vec_123(a);

    ASSERT(_op(a)[0] == 1, "data basic test 1");
    ASSERT(_op(a)[1] == 2, "data basic test 2");
    ASSERT(_op(a)[2] == 3, "data basic test 3");

    ASSERT(_op(a) == &a[0], "data pointer test 1");

    tlucanti::vector_base<int> b;

    ASSERT(_op(b) == nullptr, "data empty pointer test 1");

    result();
}

#undef _op
void iterator_test()
{
    start("iterator tests");

    vec_123(a);

    ASSERT(*a.begin() == 1, "basic iterator test 1");
    ASSERT(*(++a.begin()) == 2, "basic iterator test 2");
    ASSERT(*(++(++a.begin())) == 3, "basic iterator test 3");

    ASSERT(*(--a.end()) == 3, "basic iterator test 4");
    ASSERT(*(--(--a.end())) == 2, "basic iterator test 5");
    ASSERT(*(--(--(--a.end()))) == 1, "basic iterator test 6");

    tlucanti::vector_base<int>::iterator i1 = a.begin();
    ASSERT(i1++ == a.begin(), "basic increment iterator test 1");
    ASSERT(i1 == ++a.begin(), "basic increment iterator test 2");
    tlucanti::vector_base<int>::iterator i2 = a.end();
    ASSERT(i2-- == a.end(), "basic increment iterator test 3");
    ASSERT(i2 == --a.end(), "basic increment iterator test 4");

    ASSERT(*(a.begin() + 1) == 2, "basic iterator test 7");
    ASSERT(*(a.begin() + 2) == 3, "basic iterator test 8");

    ASSERT(*(a.end() - 1) == 3, "basic iterator test 9");
    ASSERT(*(a.end() - 2) == 2, "basic iterator test 10");
    ASSERT(*(a.end() - 3) == 1, "basic iterator test 11");

    ASSERT(a.begin() + 0 == a.end() - 3, "advanced iterator test 1");
    ASSERT(a.begin() + 1 == a.end() - 2, "advanced iterator test 2");
    ASSERT(a.begin() + 2 == a.end() - 1, "advanced iterator test 3");
    ASSERT(a.begin() + 3 == a.end() - 0, "advanced iterator test 4");

    ASSERT(a.end() - 0 == a.begin() + 3, "advanced iterator test 5");
    ASSERT(a.end() - 1 == a.begin() + 2, "advanced iterator test 6");
    ASSERT(a.end() - 2 == a.begin() + 1, "advanced iterator test 7");
    ASSERT(a.end() - 3 == a.begin() + 0, "advanced iterator test 8");

    ASSERT(a.end() - a.begin() == 3, "advanced iterator test 9");
    ASSERT(a.begin() - a.end() == -3, "advanced iterator test 10");

    tlucanti::vector_base<int> b;
    
    ASSERT(b.begin() == b.end(), "iterator empty test 1");

    std_vec_123(_a);
    ASSERT((a.begin() < a.end()) == (_a.begin() < _a.end()), "basic iterator test 12");
    ASSERT((a.begin() > a.end()) == (_a.begin() > _a.end()), "basic iterator test 13");
    ASSERT((a.begin() <= a.end()) == (_a.begin() <= _a.end()), "basic iterator test 14");
    ASSERT((a.begin() >= a.end()) == (_a.begin() >= _a.end()), "basic iterator test 15");
    ASSERT((a.begin() <= a.end() - 3) == (_a.begin() <= _a.end() - 3), "basic iterator tes 16");
    ASSERT((a.begin() >= a.end() - 3) == (_a.begin() >= _a.end() - 3), "basic iterator tes 17");
    ASSERT((a.end() > a.begin()) == (_a.end() > _a.begin()), "basic iterator test 18");
    ASSERT((a.end() < a.begin()) == (_a.end() < _a.begin()), "basic iterator test 19");
    ASSERT((a.end() >= a.begin()) == (_a.end() >= _a.begin()), "basic iterator test 20");
    ASSERT((a.end() <= a.begin()) == (_a.end() <= _a.begin()), "basic iterator test 21");
    ASSERT((a.end() >= a.begin() + 3) == (_a.end() >= _a.begin() + 3), "basic iterator test 22");
    ASSERT((a.end() <= a.begin() + 3) == (_a.end() <= _a.begin() + 3), "basic iterator test 23");

    result();
}

void reverse_iterator_test()
{
    start("reverse iterator tests");

    vec_123(a);

    ASSERT(*(a.rbegin()) == 3, "basic reverse iterator test 1");
    ASSERT(*(++a.rbegin()) == 2, "basic reverse iterator test 2");
    ASSERT(*(++(++a.rbegin())) == 1, "basic reverse iterator test 3");

    ASSERT(*(--a.rend()) == 1, "basic reverse iterator test 4");
    ASSERT(*(--(--a.rend())) == 2, "basic reverse iterator test 5");
    ASSERT(*(--(--(--a.rend()))) == 3, "basic reverse iterator test 6");

    tlucanti::vector_base<int>::reverse_iterator i1 = a.rbegin();
    ASSERT(i1++ == a.rbegin(), "basic increment reverse iterator test 1");
    ASSERT(i1 == ++a.rbegin(), "basic increment reverse iterator test 2");
    tlucanti::vector_base<int>::reverse_iterator i2 = a.rend();
    ASSERT(i2-- == a.rend(), "basic increment reverse iterator test 3");
    ASSERT(i2 == --a.rend(), "basic increment reverse iterator test 4");

    ASSERT(*(a.rbegin() + 1) == 2, "basic reverse iterator test 7");
    ASSERT(*(a.rbegin() + 2) == 1, "basic reverse iterator test 8");

    ASSERT(*(a.rend() - 1) == 1, "basic reverse iterator test 9");
    ASSERT(*(a.rend() - 2) == 2, "basic reverse iterator test 10");
    ASSERT(*(a.rend() - 3) == 3, "basic reverse iterator test 11");

    ASSERT(a.rbegin() + 0 == a.rend() - 3, "advanced reverse iterator test 1");
    ASSERT(a.rbegin() + 1 == a.rend() - 2, "advanced reverse iterator test 2");
    ASSERT(a.rbegin() + 2 == a.rend() - 1, "advanced reverse iterator test 3");
    ASSERT(a.rbegin() + 3 == a.rend() - 0, "advanced reverse iterator test 4");

    ASSERT(a.rend() - 0 == a.rbegin() + 3, "advanced reverse iterator test 5");
    ASSERT(a.rend() - 1 == a.rbegin() + 2, "advanced reverse iterator test 6");
    ASSERT(a.rend() - 2 == a.rbegin() + 1, "advanced reverse iterator test 7");
    ASSERT(a.rend() - 3 == a.rbegin() + 0, "advanced reverse iterator test 8");

    ASSERT(a.rend() - a.rbegin() == 3, "advanced reverse iterator test 9");
    ASSERT(a.rbegin() - a.rend() == -3, "advanced reverse iterator test 10");

    tlucanti::vector_base<int> b;

    ASSERT(b.begin() == b.end(), "iterator empty test 1");

    std_vec_123(_a);
    ASSERT((a.rbegin() < a.rend()) == (_a.rbegin() < _a.rend()), "basic iterator test 12");
    ASSERT((a.rbegin() > a.rend()) == (_a.rbegin() > _a.rend()), "basic iterator test 13");
    ASSERT((a.rbegin() <= a.rend()) == (_a.rbegin() <= _a.rend()), "basic iterator test 14");
    ASSERT((a.rbegin() >= a.rend()) == (_a.rbegin() >= _a.rend()), "basic iterator test 15");
    ASSERT((a.rbegin() <= a.rend() - 3) == (_a.rbegin() <= _a.rend() - 3), "basic iterator tes 16");
    ASSERT((a.rbegin() >= a.rend() - 3) == (_a.rbegin() >= _a.rend() - 3), "basic iterator tes 17");
    ASSERT((a.rend() > a.rbegin()) == (_a.rend() > _a.rbegin()), "basic iterator test 18");
    ASSERT((a.rend() < a.rbegin()) == (_a.rend() < _a.rbegin()), "basic iterator test 19");
    ASSERT((a.rend() >= a.rbegin()) == (_a.rend() >= _a.rbegin()), "basic iterator test 20");
    ASSERT((a.rend() <= a.rbegin()) == (_a.rend() <= _a.rbegin()), "basic iterator test 21");
    ASSERT((a.rend() >= a.rbegin() + 3) == (_a.rend() >= _a.rbegin() + 3), "basic iterator test 22");
    ASSERT((a.rend() <= a.rbegin() + 3) == (_a.rend() <= _a.rbegin() + 3), "basic iterator test 23");

    result();
}

void fn_empty_tests()
{
    start(".empty() tests");

    {
        vec_123(a);
        ASSERT(not a.empty(), "empty basic test 0");
    }
    {
        tlucanti::vector_base<int> a;
        ASSERT(a.empty(), "empty basic test 1");
    }

    result();
}

void fn_size_tests()
{
    start(".size() tests");

    {
        tlucanti::vector_base<int> a;
        ASSERT(a.size() == 0, "basic size test 2");
        a.push_back(1);
        ASSERT(a.size() == 1, "basic size test 2");
    } {
        tlucanti::vector_base<int> a(100);
        ASSERT(a.size() == 100, "basic size test 3");
    }

    result();
}

void fn_max_size_tests()
{
    start(".max_size() tests");

    tlucanti::vector_base<int> a;
    ASSERT(a.max_size() == 9223372036854775807, "max_size test 1");

    result();
}

void fn_reserve_tests()
{
    start(".reserve() tests");

    tlucanti::vector_base<int> a;
    a.reserve(0);
    ASSERT(a.capacity() == 0, "reserve test 1");
    a.reserve(1);
    ASSERT(a.capacity() == 7, "reserve test 2");
    a.reserve(6);
    ASSERT(a.capacity() == 7, "reserve test 3");
    a.reserve(7);
    ASSERT(a.capacity() == 11, "reserve test 4");

    result();
}

void fn_capacity_tests()
{
    start(".capacity() tests");

    {
        tlucanti::vector_base<int> v;
        ASSERT(v.capacity() == 0, "capacity empty test 0");
    }
    {
        vec_123(v);
        ASSERT(v.capacity() == 7, "capacity sized test 0");
    }
    {
        tlucanti::vector_base<int> v = {1, 2, 3, 4, 5, 6};
        ASSERT(v.capacity() == 7, "capacity sized test 1");
        v.push_back(8);
        ASSERT(v.capacity() == 11, "capacity sized test 2");
    }

    result();
}

void fn_shrink_to_fit_tests()
{
    start(".shrink_to_fit() tests");

    {
        tlucanti::vector_base<int> a;
        a.shrink_to_fit();
        ASSERT(a.capacity() == 0, "shrink to fit basic test 0");
    }
    {
        vec_123(v);
        v.shrink_to_fit();
        ASSERT(v.capacity() == 7, "shrink to fit basic test 1");
    }
    {
        tlucanti::vector_base<int> a = {1, 2, 3, 4, 5, 6, 7};
        a.shrink_to_fit();
        ASSERT(a.capacity() == 11, "shrink to fit basic test 2");
    }

    result();
}

void fn_clear_tests()
{
    start(".clear() tests");

    std::allocator<int> alloc;
    {
        vec_123(a);
        a.clear();
        ASSERT(a.empty(), "clear basic test 0");
        ASSERT(a.capacity() != 0, "clear basic test 1");
    }
    {
        tlucanti::vector_base<int> a;
        a.clear();
        vec_ASSERT(a, 0, 0, nullptr, nullptr, alloc, "clear empty test 1");
    }

    result();
}

void fn_insert_tests()
{
    start(".insert() tests");

    {
        vec_123(a);
        a.insert(a.begin(), 123);
        vec_cmp("basic insert test 0", int, a, 123, 1, 2, 3);
    }
    {
        vec_123(a);
        a.insert(a.end(), 123);
        vec_cmp("basic insert test 1", int, a, 1, 2, 3, 123);
    }
    {
        vec_123(a);
        a.insert(++a.begin(), 111);
        vec_cmp("basic insert test 2", int, a, 1, 111, 2, 3);
    }

    {
        vec_123(a);
        a.insert(a.begin(), 3, 234);
        vec_cmp("basic insert test 3", int, a, 234, 234, 234, 1, 2, 3);
    }
    {
        vec_123(a);
        a.insert(a.end(), 3, 432);
        vec_cmp("basic insert 4", int, a, 1, 2, 3, 432, 432, 432);
    }
    {
        vec_123(a);
        a.insert(++a.begin(), 3, 345);
        vec_cmp("basic insert test 5", int, a, 1, 345, 345, 345, 2, 3);
    }

    {
        vec_123(a);
        vec_111(b);
        a.insert(a.begin(), b.begin(), b.end());
        vec_cmp("basic insert test 6", int, a, 123, 456, 789, 1, 2, 3);
    }
    {
        vec_123(a);
        vec_111(b);
        a.insert(a.end(), b.begin(), b.end());
        vec_cmp("basic insert test 7", int, a, 1, 2, 3, 123, 456, 789);
    }
    {
        vec_123(a);
        vec_111(b);
        a.insert(++a.begin(), b.begin(), b.end());
        vec_cmp("basic insert test 8", int, a, 1, 123, 456, 789, 2, 3);
    }
    {
        vec_123(a);
        vec_111(b);
        std_vec_123(sa);
        std_vec_111(sb);
        a.insert(++a.begin(), b.rbegin(), b.rend());
        sa.insert(++sa.begin(), sb.rbegin(), sb.rend());
        std_vec_cmp(sa, a, "basic insert test 9");
    }

    result();

}

void fn_emplace_tests()
{
    start(".emplace() tests");

    result();
}

void fn_erase_tests()
{
    start(".fn_erase_tests() tests");

    {
        vec_123(a);
        std_vec_123(b);
        a.erase(a.begin());
        b.erase(b.begin());
        std_vec_cmp(b, a, "basic erase test 0");
    }
    {
        vec_123(a);
        std_vec_123(b);
        a.erase(--a.end());
        b.erase(--b.end());
        std_vec_cmp(b, a, "basic erase test 1");
    }
    {
        vec_123(a);
        std_vec_123(b);
        a.erase(++a.begin());
        b.erase(++b.begin());
        std_vec_cmp(b, a, "basic erase test 2");
    }

    {
        vec_123(a);
        std_vec_123(b);
        a.erase(a.begin(), a.end());
        b.erase(b.begin(), b.end());
        std_vec_cmp(b, a, "basic erase test 3");
    }
    {
        vec_123(a);
        std_vec_123(b);
        a.erase(a.begin(), ++a.begin());
        b.erase(b.begin(), ++b.begin());
        std_vec_cmp(b, a, "basic erase test 4");
    }
    {
        vec_123(a);
        std_vec_123(b);
        a.erase(--a.end(), a.end());
        b.erase(--b.end(), b.end());
        std_vec_cmp(b, a, "basic erase test 5");
    }
    {
        vec_123(a);
        std_vec_123(b);
        a.erase(++a.begin(), ++(++a.begin()));
        b.erase(++b.begin(), ++(++b.begin()));
        std_vec_cmp(b, a, "basic erase test 6");
    }

    result();
}

void fn_push_back_tests()
{
    start(".push_back() tests");

    {
        tlucanti::vector_base<int> a;
        a.push_back(123);
        vec_cmp("push back basic test 0", int, a, 123);
    }
    {
        vec_123(a);
        a.push_back(321);
        vec_cmp("push back basic test 1", int, a, 1, 2, 3, 321);
    }

    {
        tlucanti::vector_base<int> a;
        a.push_back(1);
        a.push_back(2);
        a.push_back(3);
        a.push_back(4);
        a.push_back(5);
        a.push_back(6);
        a.push_back(7);
        a.push_back(8);
        a.push_back(9);
        a.push_back(10);
        a.push_back(11);
        vec_cmp("push back advanced test 0", int, a, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11);
    }

    result();
}

void fn_emplace_back_tests()
{
    start(".fn_emplace_back_tests() tests");

    result();
}

void fn_pop_back_tests()
{
    start(".fn_pop_back_tests() tests");

    {
        vec_123(a);
        a.pop_back();
        vec_cmp("pop back basic test 0", int, a, 1, 2);
    }
    {
        tlucanti::vector_base<int> a;
        a.push_back(1);
        a.pop_back();
        ASSERT(a.empty(), "pop back basic test 1");
    }

    result();
}

void fn_resize_test()
{
    start(".fn_resize_test() tests");

    {
        tlucanti::vector_base<int> a;
        a.resize(3);
        ASSERT(a.size() == 3, "resize basic test 0");
        ASSERT(a.capacity() == 7, "resize basic test 1");
    }
    {
        vec_123(a);
        a.resize(2);
        vec_cmp("resize basic test 2", int, a, 1, 2);
    }
    {
        tlucanti::vector_base<int> a;
        a.resize(6);
        a.resize(0);
        ASSERT(a.empty(), "resize basic test 3");
        a.resize(6);
        ASSERT(a.size() == 6, "resize basic test 4");
        ASSERT(a.capacity() == 7, "resize basic test 5");
        a.resize(7);
        ASSERT(a.size() == 7, "resize basic test 6");
        ASSERT(a.capacity() == 11, "resize basic test 7");
    }

    {
        tlucanti::vector_base<int> a;
        a.resize(4, 111);
        vec_cmp("resize basic test 8", int, a, 111, 111, 111, 111);
    }
    {
        vec_123(a);
        a.resize(2, 888);
        vec_cmp("resize basic test 9", int, a, 1, 2);
    }
    {
        tlucanti::vector_base<int> a;
        a.resize(6, 111);
        a.resize(1, 222);
        vec_cmp("resize basic test 10", int, a, 111);
        a.resize(6, 333);
        vec_cmp("resize basic test 11", int, a, 111, 333, 333, 333, 333, 333);
        ASSERT(a.capacity() == 7, "resize basic test 12");
        a.resize(7, 44);
        vec_cmp("resize basic test 13", int, a, 111, 333, 333, 333, 333, 333, 44);
        ASSERT(a.capacity() == 11, "resize basic test 14");
        a.resize(0, 999);
        ASSERT(a.empty(), "resize basic test 15");
        a.resize(1, 145145);
        vec_cmp("resize basic test 16", int, a, 145145);
    }

    result();
}

void swap_tests()
{
    start(".swap_tests() tests");

    {
        vec_123(a);
        vec_111(b);
        a.swap(b);
        vec_cmp("basic swap test 0", int, a, 123, 456, 789);
        vec_cmp("basic swap test 1", int, b, 1, 2, 3);
    }
    {
        vec_123(a);
        tlucanti::vector_base<int> b;
        a.swap(b);
        ASSERT(a.empty(), "basic swap test 3");
        vec_cmp("basic swap test 4", int, b, 1, 2, 3);
    }
    {
        vec_123(a);
        tlucanti::vector_base<int> b;
        b.swap(a);
        ASSERT(a.empty(), "basic swap test 3");
        vec_cmp("basic swap test 4", int, b, 1, 2, 3);
    }

    result();
}

void std_vector_test()
{
    start("std::vector interaction tests");

    {
        vec_123(a);
        std::vector<int> b(a.begin(), a.end());
        vec_cmp("vector iterator test 0", int, b, 1, 2, 3);
    }
    {
        std_vec_123(a);
        tlucanti::vector_base<int> b(a.begin(), a.end());
        vec_cmp("vector iterator test 1", int, b, 1, 2, 3);
    }
    {
        vec_123(a);
        std::vector<int> b(a.rbegin(), a.rend());
        vec_cmp("vector iterator test 2", int, b, 3, 2, 1);
    }
    {
        std_vec_123(a);
        tlucanti::vector_base<int> b(a.rbegin(), a.rend());
        vec_cmp("vector iterator test 3", int, b, 3, 2, 1);
    }

    {
        vec_123(a);
        std::vector<int> b;
        b.assign(a.begin(), a.end());
        vec_cmp("vector iterator test 2", int, b, 1, 2, 3);
    }
    {
        std_vec_123(a);
        tlucanti::vector_base<int> b;
        b.assign(a.begin(), a.end());
        vec_cmp("vector iterator test 3", int, b, 1, 2, 3);
    }

    {
        vec_123(a);
        std::vector<int> b;

        b.assign(++a.begin(), ++(++(a.begin())));
        vec_cmp("iterator operators test 0", int, b, 2);

        b.assign(--(--a.end()), --a.end());
        vec_cmp("iterator operators test 1", int, b, 2);

        b.assign(++a.rbegin(), ++(++a.rbegin()));
        vec_cmp("iterator operators test 2", int, b, 2);

        b.assign(--(--a.rend()), --a.rend());
        vec_cmp("iterator operators test 3", int, b, 2);

        b.assign(a.begin() + 1, a.end() + 0);
        vec_cmp("iterator operators test 4", int, b, 2, 3);

        b.assign(a.end() - 3, a.end() - 0);
        vec_cmp("iterator operators test 5", int, b, 1, 2, 3);

        b.assign(a.rbegin() + 1, a.rend() + 0);
        vec_cmp("iterator operators test 6", int, b, 2, 1);

        b.assign(a.rend() - 3, a.rend() - 0);
        vec_cmp("iterator operators test 7", int, b, 3, 2, 1);

        b.assign(a.begin() += 1, a.end() += 0);
        vec_cmp("iterator operators test 4", int, b, 2, 3);

        b.assign(a.end() -= 3, a.end() -= 0);
        vec_cmp("iterator operators test 5", int, b, 1, 2, 3);

        b.assign(a.rbegin() += 1, a.rend() += 0);
        vec_cmp("iterator operators test 6", int, b, 2, 1);

        b.assign(a.rend() -= 3, a.rend() -= 0);
        vec_cmp("iterator operators test 7", int, b, 3, 2, 1);
    }
    {
        std_vec_123(a);
        tlucanti::vector_base<int> b;

        b.assign(++a.begin(), ++(++(a.begin())));
        vec_cmp("std iterator operators test 0", int, b, 2);

        b.assign(--(--a.end()), --a.end());
        vec_cmp("std iterator operators test 1", int, b, 2);

        b.assign(++a.rbegin(), ++(++a.rbegin()));
        vec_cmp("std iterator operators test 2", int, b, 2);

        b.assign(--(--a.rend()), --a.rend());
        vec_cmp("std iterator operators test 3", int, b, 2);

        b.assign(a.begin() + 1, a.end() + 0);
        vec_cmp("std iterator operators test 4", int, b, 2, 3);

        b.assign(a.end() - 3, a.end() - 0);
        vec_cmp("std iterator operators test 5", int, b, 1, 2, 3);

        b.assign(a.rbegin() + 1, a.rend() + 0);
        vec_cmp("std iterator operators test 6", int, b, 2, 1);

        b.assign(a.rend() - 3, a.rend() - 0);
        vec_cmp("std iterator operators test 7", int, b, 3, 2, 1);

        b.assign(a.begin() += 1, a.end() += 0);
        vec_cmp("std iterator operators test 4", int, b, 2, 3);

        b.assign(a.end() -= 3, a.end() -= 0);
        vec_cmp("std iterator operators test 5", int, b, 1, 2, 3);

        b.assign(a.rbegin() += 1, a.rend() += 0);
        vec_cmp("std iterator operators test 6", int, b, 2, 1);

        b.assign(a.rend() -= 3, a.rend() -= 0);
        vec_cmp("std iterator operators test 7", int, b, 3, 2, 1);
    }

    result();
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
        return _col_e == cmp._col_e;
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

void user_type_constructor_test()
{
    start("user type tests");

    {
        tlucanti::vector_base<UserClass> a(3);
        ASSERT(UserClass::total_instances == 3, "user type constructor test 1");
    }
    ASSERT(UserClass::total_instances == 0, "user type constructor test 2");

    {
        tlucanti::vector_base<UserClass> a(3);
        ASSERT(a.capacity() == 7, "allocator check 1");
        ASSERT(UserClass::total_instances == 3, "user type constructor test 3");
        a.resize(7);
        ASSERT(a.capacity() == 11, "allocator check 2");
        ASSERT(UserClass::total_instances == 7, "user type constructor test 4");
    }
    ASSERT(UserClass::total_instances == 0, "user type constructor test 5");

    {
        tlucanti::vector_base<UserClass> a(3, UserClass(1, 2));
        ASSERT(UserClass::total_instances == 3, "user type test 6");
        a.resize(11);
        ASSERT(UserClass::total_instances == 11, "user type test 7");
    }
    ASSERT(UserClass::total_instances == 0, "user type test 8");

    // --------------------------------
    UserClass::total_instances = 0;
    UserClass::verbose = false;
    UserClass::monitoring = true;
    {
        moves.clear();
        {
            tlucanti::vector_base<UserClass> a;
            a.push_back(UserClass());
            vec_cmp_lock("user class modern test 0", ColString, moves, Def, Cpy, Del);
        }
        vec_cmp_lock("user class modern test 1", ColString, moves, Def, Cpy, Del * 2);
        ASSERT(UserClass::total_instances == 0, "user type test 9");
    }
    {{{
        moves.clear();
        {
            std::vector<UserClass> a;
            a.push_back(UserClass());
            vec_cmp_lock("std -- user class modern test 0", ColString, moves, Def, Cpy, Del);
        }
        vec_cmp_lock("std -- user class modern test 1", ColString, moves, Def, Cpy, Del * 2);
        ASSERT(UserClass::total_instances == 0, "std -- user type test 9");
    }}}
    UserClass::verbose = false;
    {
        moves.clear();
        {
            tlucanti::vector_base<UserClass> a(6);
            make_std_vec_lock(cmp1, ColString, Def * 6);
            std_vec_cmp_lock(cmp1, moves, "user class modern test 2");
            moves.clear();
            a.push_back(UserClass(111, 222));
            make_std_vec_lock(cmp2, ColString, Cons, Cpy * 6, Del * 6, Cpy, Del);
            std_vec_cmp_lock(cmp2, moves, "user class modern test 3");
            moves.clear();
        }
        vec_cmp_lock("user class modern test 4", ColString, moves, Del * 7);
        ASSERT(UserClass::total_instances == 0, "user type test 10");
    }
    {{{
        moves.clear();
        {
            std::vector<UserClass> a(6);
            make_std_vec_lock(cmp1, ColString, Def * 6);
            std_vec_cmp_lock(cmp1, moves, "std -- user class modern test 2");
            moves.clear();
            a.push_back(UserClass(111, 222));
            make_std_vec_lock(cmp2, ColString, Cons, Cpy * 7, Del * 7);
            std_vec_cmp_lock(cmp2, moves, "std -- user class modern test 3");
            moves.clear();
        }
        vec_cmp_lock("std -- user class modern test 4", ColString, moves, Del * 7);
        ASSERT(UserClass::total_instances == 0, "std -- user type test 10");
    }}}
    {
        moves.clear();
        {
            tlucanti::vector_base<UserClass> a(11, UserClass(123, 456)); // 11 items + 18 allocated
            vec_cmp_lock("user class modern test 5", ColString, moves, Cons, Cpy * 11, Del);
            moves.clear();
            a.pop_back(); // 10 left + 1 del
            a.pop_back(); // 9 left + 2 del
            a.pop_back(); // 8 left + 3 del
            a.pop_back(); // 7 left + 4 del
            a.pop_back(); // 6 left + 5 del --> reallocation
            vec_cmp_lock("user class modern test 6", ColString, moves, Del * 5, Cpy * 6, Del * 6);
            moves.clear();
        }
        vec_cmp_lock("user class modern test 7", ColString, moves, Del * 6);
        ASSERT(UserClass::total_instances == 0, "user type test 11");
    }
    {{{
        moves.clear();
        {
            std::vector<UserClass> a(11, UserClass(123, 456)); // 11 items + 18 allocated
            vec_cmp_lock("std -- user class modern test 5", ColString, moves, Cons, Cpy * 11, Del);
            moves.clear();
            a.pop_back(); // 10 left + 1 del
            a.pop_back(); // 9 left + 2 del
            a.pop_back(); // 8 left + 3 del
            a.pop_back(); // 7 left + 4 del
            a.pop_back(); // 6 left + 5 del --> reallocation
            vec_cmp_lock("std -- user class modern test 6", ColString, moves, Del * 5);
            moves.clear();
        }
        vec_cmp_lock("std -- user class modern test 7", ColString, moves, Del * 6);
        ASSERT(UserClass::total_instances == 0, "std -- user type test 11");
    }}}
    UserClass::verbose = false;
    {
        moves.clear();
        {
            tlucanti::vector_base<UserClass> a(3);
            vec_cmp_lock("user class modern test 8", ColString, moves, Def * 3);
            moves.clear();
            a.erase(++a.begin());
            vec_cmp_lock("user class modern test 9", ColString, moves, Del, Cpy, Del);
            moves.clear();
        }
        vec_cmp_lock("user class modern test 10", ColString, moves, Del * 2);
        ASSERT(UserClass::total_instances == 0, "user type test 12");
    }
    UserClass::verbose = true;
    {{{
        moves.clear();
        {
            std::vector<UserClass> a(10);
            vec_cmp_lock("std -- user class modern test 8", ColString, moves, Def * 3);
            moves.clear();
            a.erase(++a.begin(), --a.end());
            vec_cmp_lock("std -- user class modern test 9", ColString, moves, Del);
            moves.clear();
        }
        vec_cmp_lock("std -- user class modern test 10", ColString, moves, Del * 2);
        ASSERT(UserClass::total_instances == 0, "std -- user type test 12");
    }}}
    return;
    {
        moves.clear();
        {
            tlucanti::vector_base<UserClass> a(5);
            vec_cmp_lock("user class modern test 11", ColString, moves, Def * 5);
            moves.clear();
            a.erase(++a.begin(), --a.end());
            ASSERT(a.size() == 2, "user class test 13");
            vec_cmp_lock("user class modern test 12", ColString, moves, Del * 3);
            moves.clear();
        }
        vec_cmp_lock("user class modern test 13", ColString, moves, Del * 2);
        ASSERT(UserClass::total_instances == 0, "user type test 14");
    }
    {
        moves.clear();
        {
            tlucanti::vector_base<UserClass> a;
            ASSERT(UserClass::total_instances == 0, "user type test 15");
            a.insert(a.end(), UserClass(1, 2));
            vec_cmp_lock("user class modern test 14", ColString, moves, Cons, Cpy, Del);
            moves.clear();
        }
        vec_cmp_lock("user class modern test 15", ColString, moves, Del * 2);
        ASSERT(UserClass::total_instances == 0, "user type test 16");
    }
    {
        moves.clear();
        {
            tlucanti::vector_base<UserClass> a(2);
            vec_cmp_lock("user class modern test 16", ColString, moves, Def * 2);
            moves.clear();
            {
                tlucanti::vector_base<UserClass> b(3, UserClass(4, 5));
                vec_cmp_lock("user class modern test 17", ColString, moves, Cons, Cpy * 3, Del);
                moves.clear();
                a.insert(++(++a.begin()), b.begin(), b.end());
                vec_cmp_lock("user class modern test 18", ColString, moves, Cpy, Del, Cpy * 3);
                moves.clear();
            }
            vec_cmp_lock("user class modern test 18", ColString, moves, Del * 3);
            ASSERT(UserClass::total_instances == 5, "user type test 17");
            moves.clear();
        }
        vec_cmp_lock("user class modern test 19", ColString, moves, Del * 5);
        ASSERT(UserClass::total_instances == 0, "user type test 18");
    }

    result();
}
