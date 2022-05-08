
#include <iostream>
#include <vector>
#include <memory>
#include <sstream>

#define __DEBUG
#include "defs.h"
#include "vector.hpp"
#include "color.h"

int all = 0;
int ok = 0;
#undef assert
#define assert(e, msg) do { \
    ++all; \
    try { \
        if (not (e)) \
            std::cout << R "assertion fail: " Y << (msg) << S << std::endl; \
        else \
            ++ok; \
    } catch (std::exception &ex) { \
        std::cout << R "task fall with exception: " Y << ex.what() << S << std::endl; \
    throw;} \
} while (0)

#define vec_assert(__vec, __size, __allocated, __begin, __end, __alloc, __msg) \
    assert((__vec)._size == (__size), __msg " size check"); \
    assert((__vec).size() == (__size), __msg " size() check"); \
    assert((__vec)._allocated == (__allocated), __msg " allocated check"); \
    assert((__vec).capacity() == (__allocated), __msg " .capacity() check"); \
    assert((__vec)._begin == (__begin), __msg " begin iterator check"); \
    assert((__vec)._end == (__end), __msg " end iterator check"); \
    assert((__vec)._allocator == (__alloc), __msg " allocator check"); \
    assert((__vec).get_allocator() == (__alloc), __msg " .get_allocator() check")

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
} while (0)

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
void reverse_iterator_test();

void user_type_constructor_test();

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
    reverse_iterator_test();

    user_type_constructor_test();
}

# define vec_123(__vec) tlucanti::vector_base<int> __vec(3); (__vec)[0] = 1; (__vec)[1] = 2; (__vec)[2] = 3
# define std_vec_123(__vec) std::vector<int> __vec(3); (__vec)[0] = 1; (__vec)[1] = 2; (__vec)[2] = 3

void constructor_test()
{
    start("constructor tests");
    std::vector<int> vec;

    std::allocator<int> alloc;
    {
        tlucanti::vector_base<int> a;
        vec_assert(a, 0, 0, nullptr, nullptr, alloc, "default constructor test");
    }
    {
        tlucanti::vector_base<int> a(alloc);
        vec_assert(a, 0, 0, nullptr, nullptr, alloc, "default constructor + alloc test");
    }
    {
        tlucanti::vector_base<int> a(3);
        vec_assert(a, 3, 7, a._begin, a._end, alloc, "default size constructor test");
        assert(a._end - a._begin == 3, "size 3 vec iterator test 1");
    }
    {
        tlucanti::vector_base<int> a(3, alloc);
        vec_assert(a, 3, 7, a._begin, a._end, alloc, "default size + alloc constructor test");
        assert(a._end - a._begin == 3, "default size iterator test 1");
    }
    {
        tlucanti::vector_base<int> a(3, 123);
        vec_assert(a, 3, 7, a._begin, a._end, alloc, "constructor default value test");
        assert(a._end - a._begin == 3, "constructor default value iterator test 1");
        assert(a[0] == 123, "constructor default value test 1");
        assert(a[1] == 123, "constructor default value test 2");
        assert(a[2] == 123, "constructor default value test 3");
    }
    {
        tlucanti::vector_base<int> a(3, 123, alloc);
        vec_assert(a, 3, 7, a._begin, a._end, alloc, "constructor default value + alloc test");
        assert(a._end - a._begin == 3, "constructor default value + alloc iterator test 1");
        assert(a[0] == 123, "constructor default value + alloc test 1");
        assert(a[1] == 123, "constructor default value + alloc test 2");
        assert(a[2] == 123, "constructor default value + alloc test 3");
    }
    {
        vec_123(v);
        tlucanti::vector_base<int> a(v.begin(), v.end());
        vec_assert(a, 3, 7, a._begin, a._end, alloc, "constructor iterator copy test");
        assert(a._end - a._begin == 3, "constructor iterator copy iterator test 1");
        assert(a[0] == 1, "constructor iterator copy test 1");
        assert(a[1] == 2, "constructor iterator copy test 2");
        assert(a[2] == 3, "constructor iterator copy test 3");
    }
    {
        vec_123(v);
        tlucanti::vector_base<int> a(v.begin(), v.end(), alloc);
        vec_assert(a, 3, 7, a._begin, a._end, alloc, "constructor iterator copy + alloc test");
        assert(a._end - a._begin == 3, "constructor iterator copy + alloc iterator test 1");
        assert(a[0] == 1, "constructor iterator copy + alloc test 1");
        assert(a[1] == 2, "constructor iterator copy + alloc test 2");
        assert(a[2] == 3, "constructor iterator copy + alloc test 3");
    }
    {
        vec_123(v);
        tlucanti::vector_base<int> a(v);
        vec_assert(a, 3, 7, a._begin, a._end, alloc, "copy constructor test");
        assert(a._end - a._begin == 3, "copy constructor iterator test 1");
        assert(a[0] == 1, "copy constructor test 1");
        assert(a[1] == 2, "copy constructor test 2");
        assert(a[2] == 3, "copy constructor test 3");
    }
    {
        vec_123(v);
        tlucanti::vector_base<int> a(v, alloc);
        vec_assert(a, 3, 7, a._begin, a._end, alloc, "copy constructor + alloc test");
        assert(a._end - a._begin == 3, "copy constructor + alloc iterator test 1");
        assert(a[0] == 1, "copy constructor + alloc test 1");
        assert(a[1] == 2, "copy constructor + alloc test 2");
        assert(a[2] == 3, "copy constructor + alloc test 3");
    }
#if CPP11
    {
        vec_123(v);
        tlucanti::vector_base<int> a(std::move(v));
        vec_assert(a, 3, 7, a._begin, a._end, alloc, "move constructor test");
        assert(a._end - a._begin == 3, "move constructor iterator test 1");
        assert(a[0] == 1, "move constructor test 1");
        assert(a[1] == 2, "move constructor test 2");
        assert(a[2] == 3, "move constructor test 3");
        vec_assert(v, 0, 0, nullptr, nullptr, alloc, "move constructor empty vector test");
    }
    {
        vec_123(v);
        tlucanti::vector_base<int> a(std::move(v), alloc);
        vec_assert(a, 3, 7, a._begin, a._end, alloc, "move + alloc constructor test");
        assert(a._end - a._begin == 3, "move + alloc constructor iterator test 1");
        assert(a[0] == 1, "move + alloc constructor test 1");
        assert(a[1] == 2, "move + alloc constructor test 2");
        assert(a[2] == 3, "move + alloc constructor test 3");
        vec_assert(v, 0, 0, nullptr, nullptr, alloc, "move + alloc constructor empty vector test");
    }
    {
        tlucanti::vector_base<int> a({1, 2, 3});
        vec_assert(a, 3, 7, a._begin, a._end, alloc, "initializer list constructor test");
        assert(a._end - a._begin == 3, "initializer list constructor iterator test 1");
        assert(a[0] == 1, "initializer list constructor test 1");
        assert(a[1] == 2, "initializer list constructor test 2");
        assert(a[2] == 3, "initializer list constructor test 3");
    }
    {
        tlucanti::vector_base<int> a({1, 2, 3}, alloc);
        vec_assert(a, 3, 7, a._begin, a._end, alloc, "initializer list + alloc constructor test");
        assert(a._end - a._begin == 3, "initializer list + alloc constructor iterator test 1");
        assert(a[0] == 1, "initializer list + alloc constructor test 1");
        assert(a[1] == 2, "initializer list + alloc constructor test 2");
        assert(a[2] == 3, "initializer list + alloc constructor test 3");
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
    std::vector<int> jv(av, alloc);
#if CPP11
    std::vector<int> kv(std::move(av));
    std::vector<int> lv(std::move(bv), alloc);
    std::vector<int> mv({1, 2, 3});
    std::vector<int> nv({1, 2, 3}, alloc);
#endif /* CPP11 */

    result();
}

void assign_test()
{
    start("assign tests");

    result();
}

void fn_assign_test()
{
    start(".assign() tests");

    result();
}

void fn_get_allocator_test()
{
    start(".get_allocator() tests");

    result();
}

#undef _op
# define _op(vec, _a) vec.at(_a)
void fn_at_test()
{
    start(".at() tests");

    vec_123(a);
    assert(_op(a, 0) == 1, "basic access test 1");
    assert(_op(a, 1) == 2, "basic access test 2");
    assert(_op(a, 2) == 3, "basic access test 3");
    assert(_op(a, -1) == 3, "reverse access test 1");
    assert(_op(a, -2) == 2, "reverse access test 2");
    assert(_op(a, -3) == 1, "reverse access test 3");

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
    assert(_op(a, 0) == 1, "basic access test 1");
    assert(_op(a, 1) == 2, "basic access test 2");
    assert(_op(a, 2) == 3, "basic access test 3");
    assert(_op(a, -1) == 3, "reverse access test 1");
    assert(_op(a, -2) == 2, "reverse access test 2");
    assert(_op(a, -3) == 1, "reverse access test 3");

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
    start(".back() tests");

    vec_123(a);
    
    assert(_op(a) == 3, "basic " fn " test 1");
    
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
    start("iterator tests");

    vec_123(a);

    assert(*a.begin() == 1, "basic iterator test 1");
    assert(*(++a.begin()) == 2, "basic iterator test 2");
    assert(*(++(++a.begin())) == 3, "basic iterator test 3");

    assert(*(--a.end()) == 3, "basic iterator test 4");
    assert(*(--(--a.end())) == 2, "basic iterator test 5");
    assert(*(--(--(--a.end()))) == 1, "basic iterator test 6");

    tlucanti::vector_base<int>::iterator i1 = a.begin();
    assert(i1++ == a.begin(), "basic increment iterator test 1");
    assert(i1 == ++a.begin(), "basic increment iterator test 2");
    tlucanti::vector_base<int>::iterator i2 = a.end();
    assert(i2-- == a.end(), "basic increment iterator test 3");
    assert(i2 == --a.end(), "basic increment iterator test 4");

    assert(*(a.begin() + 1) == 2, "basic iterator test 7");
    assert(*(a.begin() + 2) == 3, "basic iterator test 8");

    assert(*(a.end() - 1) == 3, "basic iterator test 9");
    assert(*(a.end() - 2) == 2, "basic iterator test 10");
    assert(*(a.end() - 3) == 1, "basic iterator test 11");

    assert(a.begin() + 0 == a.end() - 3, "advanced iterator test 1");
    assert(a.begin() + 1 == a.end() - 2, "advanced iterator test 2");
    assert(a.begin() + 2 == a.end() - 1, "advanced iterator test 3");
    assert(a.begin() + 3 == a.end() - 0, "advanced iterator test 4");

    assert(a.end() - 0 == a.begin() + 3, "advanced iterator test 5");
    assert(a.end() - 1 == a.begin() + 2, "advanced iterator test 6");
    assert(a.end() - 2 == a.begin() + 1, "advanced iterator test 7");
    assert(a.end() - 3 == a.begin() + 0, "advanced iterator test 8");

    assert(a.end() - a.begin() == 3, "advanced iterator test 9");
    assert(a.begin() - a.end() == -3, "advanced iterator test 10");

    tlucanti::vector_base<int> b;
    
    assert(b.begin() == b.end(), "iterator empty test 1");

    assert(a.begin() < a.end(), "basic iterator test 12");
    assert(a.begin() <= a.end(), "basic iterator test 13");
    assert(a.begin() <= a.end() - 3, "basic iterator tes 14");
    assert(a.end() > a.begin(), "basic iterator test 15");
    assert(a.end() >= a.begin(), "basic iterator test 16");
    assert(a.end() >= a.begin() + 3, "basic iterator test 17");

    result();
}

void reverse_iterator_test()
{
    start("reverse iterator tests");

    vec_123(a);

    assert(*a.rbegin() == 3, "basic reverse iterator test 1");
    assert(*(++a.rbegin()) == 2, "basic reverse iterator test 2");
    assert(*(++(++a.rbegin())) == 1, "basic reverse iterator test 3");

    assert(*(--a.rend()) == 1, "basic reverse iterator test 4");
    assert(*(--(--a.rend())) == 2, "basic reverse iterator test 5");
    assert(*(--(--(--a.rend()))) == 3, "basic reverse iterator test 6");

    tlucanti::vector_base<int>::reverse_iterator i1 = a.rbegin();
    assert(i1++ == a.rbegin(), "basic increment reverse iterator test 1");
    assert(i1 == ++a.rbegin(), "basic increment reverse iterator test 2");
    tlucanti::vector_base<int>::reverse_iterator i2 = a.rend();
    assert(i2-- == a.rend(), "basic increment reverse iterator test 3");
    assert(i2 == --a.rend(), "basic increment reverse iterator test 4");

    assert(*(a.rbegin() + 1) == 2, "basic reverse iterator test 7");
    assert(*(a.rbegin() + 2) == 1, "basic reverse iterator test 8");

    assert(*(a.rend() - 1) == 1, "basic reverse iterator test 9");
    assert(*(a.rend() - 2) == 2, "basic reverse iterator test 10");
    assert(*(a.rend() - 3) == 3, "basic reverse iterator test 11");

    assert(a.rbegin() + 0 == a.rend() - 3, "advanced reverse iterator test 1");
    assert(a.rbegin() + 1 == a.rend() - 2, "advanced reverse iterator test 2");
    assert(a.rbegin() + 2 == a.rend() - 1, "advanced reverse iterator test 3");
    assert(a.rbegin() + 3 == a.rend() - 0, "advanced reverse iterator test 4");

    assert(a.rend() - 0 == a.rbegin() + 3, "advanced reverse iterator test 5");
    assert(a.rend() - 1 == a.rbegin() + 2, "advanced reverse iterator test 6");
    assert(a.rend() - 2 == a.rbegin() + 1, "advanced reverse iterator test 7");
    assert(a.rend() - 3 == a.rbegin() + 0, "advanced reverse iterator test 8");

    assert(a.rend() - a.rbegin() == 3, "advanced reverse iterator test 9");
    assert(a.rbegin() - a.rend() == -3, "advanced reverse iterator test 10");

    tlucanti::vector_base<int> b;

    assert(b.begin() == b.end(), "iterator empty test 1");

    std_vec_123(_a);
    assert(a.rbegin() < a.rend() == _a.rbegin() < _a.rend(), "basic iterator test 12");
    assert(a.rbegin() > a.rend() == _a.rbegin() > _a.rend(), "basic iterator test 13");
    assert(a.rbegin() <= a.rend() == _a.rbegin() <= _a.rend(), "basic iterator test 14");
    assert(a.rbegin() >= a.rend() == _a.rbegin() >= _a.rend(), "basic iterator test 15");
    assert(a.rbegin() <= a.rend() - 3 == _a.rbegin() <= _a.rend() - 3, "basic iterator tes 16");
    assert(a.rbegin() >= a.rend() - 3 == _a.rbegin() >= _a.rend() - 3, "basic iterator tes 17");
    assert(a.rend() > a.rbegin() == _a.rend() > _a.rbegin(), "basic iterator test 18");
    assert(a.rend() < a.rbegin() == _a.rend() < _a.rbegin(), "basic iterator test 19");
    assert(a.rend() >= a.rbegin() == _a.rend() >= _a.rbegin(), "basic iterator test 20");
    assert(a.rend() <= a.rbegin() == _a.rend() <= _a.rbegin(), "basic iterator test 21");
    assert(a.rend() >= a.rbegin() + 3 == _a.rend() >= _a.rbegin() + 3, "basic iterator test 22");
    assert(a.rend() <= a.rbegin() + 3 == _a.rend() <= _a.rbegin() + 3, "basic iterator test 23");

    _a.begin() >= a.end();
    result();
}

struct UserClass
{
    int a;
    int b;

    static int c;
    static int total_instances;
    int my_c;
    UserClass() :a(111), b(222) {
        my_c = ++c;
        ++total_instances;
        std::cout << cl() << "[" << this << "]:" G " default" S "\n";
    }
    UserClass(int _a, int _b) : a(_a), b(_b) {
        my_c = ++c;
        ++total_instances;
       std::cout << cl() << "[" << this << "]:" C " constructor" S "\n";
    }
    ~UserClass() {
        --total_instances;
        std::cout << cl() << "[" << this << "]:" R " destructor" S "\n";
    }
    __WUR const char *cl() const {
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
__DELETED_MEMBERS:
    UserClass(const UserClass &) __DELETE
    UserClass &operator =(const UserClass &) __DELETE
#if CPP11
    UserClass(UserClass &&) __DELETE
    UserClass &operator =(UserClass &&) __DELETE
#endif /* CPP11 */
};
int UserClass::c = 3;
int UserClass::total_instances = 0;

void user_type_constructor_test()
{
    start("user type tests");

    {
        tlucanti::vector_base<UserClass> a(3);
        assert(UserClass::total_instances == 3, "user type constructor test 1");
    }
    assert(UserClass::total_instances == 0, "user type constructor test 2");

//    {
//        tlucanti::vector_base<UserClass> a(6);
//        assert(UserClass::total_instances == 6, "user type constructor test 3");
//        a.resize(7);
//    }

    result();
}
