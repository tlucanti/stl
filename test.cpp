
#include <iostream>
#include <vector>
#include "vector.hpp"

template <typename T>
[[maybe_unused]]void print([[maybe_unused]] const T & t)
{
    for ([[maybe_unused]] const auto o : t)
        std::cout << o << ' ';
    std::cout << std::endl;
}

int all = 0;
int ok = 0;
#undef assert
#define assert(e, msg) { \
    ++all; \
    try { \
        if (not (e)) \
            std::cout << "assertion fail: " << (msg) << std::endl; \
        else \
            ++ok; \
    } catch (std::exception &ex) { \
        std::cout << "task fall with exception: " << ex.what() << std::endl; \
    throw;} \
}

# define error_test(expr, message) { \
    ++all; \
    bool __err = true; \
    try { \
        auto _ = expr;\
        __err = false; \
    } catch (std::exception &) {} \
    if (not __err) \
        std::cout << message ": didnt raise exception\n"; \
    else ++ok; \
}

void start(const char *msg) {
    all = 0;
    ok = 0;
    std::cout << "** results for " << msg << "**\n";
}
void result() {
    if (ok == all)
        std::cout << "[ OK ] all " << all << " tests passed\n\n";
    else
        std::cout << "[FAIL] " << ok << " / " << all << " tests passed\n\n";
}

void constructor_test();
void assign_test();
void fn_assign_test();
void fn_get_allocator_test();
void fn_at_test();
void access_operator_test();

int main()
{
    constructor_test();
    assign_test();
    fn_assign_test();
    fn_get_allocator_test();
    fn_at_test();
    access_operator_test();
}

void constructor_test()
{
    start("constructor test");
    std::vector<int> vec;

    std::allocator<int> alloc;
    tlucanti::vector_base<int> a;
    tlucanti::vector_base<int> b(alloc);
    tlucanti::vector_base<int> c(3);
    tlucanti::vector_base<int> d(3, alloc);
    tlucanti::vector_base<int> e(3, 123);
    tlucanti::vector_base<int> f(3, 123, alloc);
    tlucanti::vector_base<int> g(a.begin(), a.end());
    tlucanti::vector_base<int> h(a.begin(), a.end(), alloc);
    tlucanti::vector_base<int> i(a);
    tlucanti::vector_base<int> j(a, alloc);
    tlucanti::vector_base<int> k(std::move(a));
    tlucanti::vector_base<int> l(std::move(b), alloc);
    tlucanti::vector_base<int> m({1, 2, 3});
    tlucanti::vector_base<int> n({1, 2, 3}, alloc);

    std::vector<int> av;
    std::vector<int> bv(alloc);
    std::vector<int> cv(3);
    std::vector<int> dv(3, alloc);
    std::vector<int> ev(3, 123);
    std::vector<int> fv(3, 123, alloc);
    std::vector<int> gv(vec.begin(), vec.end());
    std::vector<int> hv(vec.begin(), vec.end(), alloc);
    std::vector<int> iv(av);
    std::vector<int> jv(av, alloc);
    std::vector<int> kv(std::move(av));
    std::vector<int> lv(std::move(bv), alloc);
    std::vector<int> mv({1, 2, 3});
    std::vector<int> nv({1, 2, 3}, alloc);

    result();
}

void assign_test()
{
    start("assign test");

    result();
}

void fn_assign_test()
{
    start(".assign() test");

    result();
}

void fn_get_allocator_test()
{
    start(".get_allocator() test");

    result();
}

#undef _op
# define _op(vec, _a) vec.at(_a)
void fn_at_test()
{
    start(".at() test");

    tlucanti::vector_base<int> a = {1, 2, 3};
    assert(_op(a, 0) == 1, "basic access test 1")
    assert(_op(a, 1) == 2, "basic access test 2")
    assert(_op(a, 2) == 3, "basic access test 3")
    assert(_op(a, -1) == 3, "reverse access test 1")
    assert(_op(a, -2) == 2, "reverse access test 2")
    assert(_op(a, -3) == 1, "reverse access test 3")

    error_test(_op(a, 3), "out of bound test 1")
    error_test(_op(a, 4), "out of bound test 2")
    error_test(_op(a, 123123), "out of bound test 3")
    error_test(_op(a, -4), "reverse out of bound test 1")
    error_test(_op(a, -5), "reverse out of bound test 2")
    error_test(_op(a, -123123), "reverse out of bound test 3")

    tlucanti::vector_base<int> b;
    error_test(_op(b, 0), "empty access test 1")
    error_test(_op(b, 1), "empty access test 2")
    error_test(_op(b, -1), "empty access test 3")
    error_test(_op(b, -2), "empty access test 4")

    result();
}

#undef _op
# define _op(vec, _a) vec[_a]
void access_operator_test()
{
    start("operator[] test");

    tlucanti::vector_base<int> a = {1, 2, 3};
    assert(_op(a, 0) == 1, "basic access test 1")
    assert(_op(a, 1) == 2, "basic access test 2")
    assert(_op(a, 2) == 3, "basic access test 3")
    assert(_op(a, -1) == 3, "reverse access test 1")
    assert(_op(a, -2) == 2, "reverse access test 2")
    assert(_op(a, -3) == 1, "reverse access test 3")

    error_test(_op(a, 3), "out of bound test 1")
    error_test(_op(a, 4), "out of bound test 2")
    error_test(_op(a, 123123), "out of bound test 3")
    error_test(_op(a, -4), "reverse out of bound test 1")
    error_test(_op(a, -5), "reverse out of bound test 2")
    error_test(_op(a, -123123), "reverse out of bound test 3")

    tlucanti::vector_base<int> b;
    error_test(_op(b, 0), "empty access test 1")
    error_test(_op(b, 1), "empty access test 2")
    error_test(_op(b, -1), "empty access test 3")
    error_test(_op(b, -2), "empty access test 4")

    result();
}