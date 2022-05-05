
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
void fn_front_test();
void fn_back_test();
void fn_data_test();
void iterator_test();

int main()
{
    constructor_test();
    assign_test();
    fn_assign_test();
    fn_get_allocator_test();
    fn_at_test();
    access_operator_test();
    fn_front_test();
    fn_back_test();
    fn_data_test();
    iterator_test();
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

#undef _op
#undef fn
# define _op(vec) vec.front()
# define fn "front"
void fn_front_test()
{
    start(".front() test");

    tlucanti::vector_base<int> a = {1, 2, 3};
    
    assert(_op(a) == 1, "basic " fn " test 1");
    
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
    start(".back() test");

    tlucanti::vector_base<int> a = {1, 2, 3};
    
    assert(_op(a) == 3, "basic " fn " test 1");
    
    tlucanti::vector_base<int> b;

    error_test(_op(b), "empty" fn "test 1");

    result();
}

#undef _op
# define _op(vec) vec.data()
void fn_data_test()
{
    start(".data() test");

    tlucanti::vector_base<int> a = {1, 2, 3};

    assert(_op(a)[0] == 1, "data basic test 1");
    assert(_op(a)[1] == 2, "data basic test 2");
    assert(_op(a)[2] == 3, "data basic test 3");

    assert(_op(a) == &a[0], "data pointer test 1");

    tlucanti::vector_base<int> b;

    assert(_op(b) == nullptr, "data empty pointer test 1");

    result();
}

#undef _op
void iterator_test()
{
    start("iterator test");

    tlucanti::vector_base<int> a = {1, 2, 3};

    assert(*a.begin() == 1, "basic iterator test 1");
    assert(*(++a.begin()) == 2, "basic iterator test 2");
    assert(*(++(++a.begin())) == 3, "basic iterator test 3");

    assert(*(--a.end()) == 3, "basic iterator test 4");
    assert(*(--(--a.end())) == 2, "basic iterator test 5");
    assert(*(--(--(--a.end()))) == 1, "basic iterator test 6");

    auto i1 = a.begin();
    assert(i1++ == a.begin(), "basic increment test 1");
    assert(i1 == ++a.begin(), "basic increment test 2");
    auto i2 = a.end();
    assert(i2-- == a.end(), "basic increment test 3");
    assert(i2 == --a.end(), "basic increment test 4");

    assert(*(a.begin() + 1) == 2, "basic iterator test 7");
    assert(*(a.begin() + 2) == 3, "basic iterator test 8");
   
    assert(*(a.end() - 1) == 3, "basic iteratir test 9");
    assert(*(a.end() - 2) == 2, "basic iteratir test 10");
    assert(*(a.end() - 3) == 1, "basic iteratir test 11");

    assert(a.begin() + 0 == a.end() - 3, "advanced iterator test 1");
    assert(a.begin() + 1 == a.end() - 2, "advanced iterator test 2");
    assert(a.begin() + 2 == a.end() - 1, "advanced iterator test 3");
    assert(a.begin() + 3 == a.end() - 0, "advanced iterator test 4");

    assert(a.end() - 0 == a.begin() + 3, "advanced iterator test 5");
    assert(a.end() - 1 == a.begin() + 2, "advanced iterator test 6");
    assert(a.end() - 2 == a.begin() + 1, "advanced iterator test 7");
    assert(a.end() - 3 == a.begin() + 0, "advanced iterator test 8");

    tlucanti::vector_base<int> b;
    
    assert(a.begin() == a.end(), "iterator empty test 1");

    result();
}

