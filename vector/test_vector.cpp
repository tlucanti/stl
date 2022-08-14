/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_vector.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tlucanti <tlucanti@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/14 11:26:42 by tlucanti          #+#    #+#             */
/*   Updated: 2022/08/14 20:24:14 by tlucanti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "vector.hpp"
#include "test.hpp"

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
void user_type_test();
void operator_vector_test();
void std_swap_tests();
void const_iterator_test();
void const_reverse_iterator_test();

void other_tests();

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
    run_test(user_type_test);
    run_test(operator_vector_test);
    run_test(std_swap_tests);
    run_test(const_iterator_test);
    run_test(const_reverse_iterator_test);

    run_test(other_tests);
    final();
}

void constructor_test()
{
    start("constructor tests");
    std::vector<int> vec;

    std::allocator<int> alloc;
    {
        ft::vector<int> a;
        vec_ASSERT(a, 0, 0, nullptr, nullptr, alloc,
            "default constructor test");
    }
    {
        ft::vector<int> a(alloc);
        vec_ASSERT(a, 0, 0, nullptr, nullptr, alloc,
            "default constructor + alloc test");
    }
    {
        ft::vector<int> a(3);
        vec_ASSERT(a, 3, 7, a._begin, a._end, alloc,
            "default size constructor test");
        ASSERT(a.size() == 3, "size 3 vec iterator test 1");
    }
    {
        ft::vector<int> a(3, alloc);
        vec_ASSERT(a, 3, 7, a._begin, a._end, alloc,
            "default size + alloc constructor test");
        ASSERT(a.size() == 3, "default size iterator test 1");
    }
    {
        ft::vector<int> a(3, 123);
        vec_ASSERT(a, 3, 7, a._begin, a._end, alloc,
            "constructor default value test");
        ASSERT(a.size() == 3, "constructor default value iterator test 1");
        ASSERT(a[0] == 123, "constructor default value test 1");
        ASSERT(a[1] == 123, "constructor default value test 2");
        ASSERT(a[2] == 123, "constructor default value test 3");
    }
    {
        ft::vector<int> a(3, 123, alloc);
        vec_ASSERT(a, 3, 7, a._begin, a._end, alloc,
            "constructor default value + alloc test");
        ASSERT(a.size() == 3, "constructor default value + alloc iterator test 1");
        ASSERT(a[0] == 123, "constructor default value + alloc test 1");
        ASSERT(a[1] == 123, "constructor default value + alloc test 2");
        ASSERT(a[2] == 123, "constructor default value + alloc test 3");
    }
    {
        vec_123(v);
        std_vec_123(stdv);
        {
            ft::vector<int> a(v.begin(), v.end());
            vec_ASSERT(a, 3, 7, a._begin, a._end, alloc,
                "constructor iterator copy test 1");
            ASSERT(a.size() == 3, "constructor iterator copy iterator test 1");
            ASSERT(a[0] == 1, "constructor iterator copy test 1");
            ASSERT(a[1] == 2, "constructor iterator copy test 2");
            ASSERT(a[2] == 3, "constructor iterator copy test 3");
        } {
            ft::vector<int> a(v.data(), v.data() + v.size());
            vec_ASSERT(a, 3, 7, a._begin, a._end, alloc,
                "constructor iterator copy test 2");
            ASSERT(a.size() == 3, "constructor iterator copy iterator test 2");
            ASSERT(a[0] == 1, "constructor iterator copy test 4");
            ASSERT(a[1] == 2, "constructor iterator copy test 5");
            ASSERT(a[2] == 3, "constructor iterator copy test 6");
        } {
            ft::vector<int> a(stdv.begin(), stdv.end());
            vec_ASSERT(a, 3, 7, a._begin, a._end, alloc,
                "constructor iterator copy test 3");
            ASSERT(a.size() == 3, "constructor iterator copy iterator test 3");
            ASSERT(a[0] == 1, "constructor iterator copy test 7");
            ASSERT(a[1] == 2, "constructor iterator copy test 8");
            ASSERT(a[2] == 3, "constructor iterator copy test 9");
        }
    }
    {
        vec_123(v);
        ft::vector<int> a(v.begin(), v.end(), alloc);
        vec_ASSERT(a, 3, 7, a._begin, a._end, alloc,
            "constructor iterator copy + alloc test");
        ASSERT(a.size() == 3,
            "constructor iterator copy + alloc iterator test 1");
        ASSERT(a[0] == 1, "constructor iterator copy + alloc test 1");
        ASSERT(a[1] == 2, "constructor iterator copy + alloc test 2");
        ASSERT(a[2] == 3, "constructor iterator copy + alloc test 3");
    }
    {
        vec_123(v);
        ft::vector<int> a(v);
        vec_ASSERT(a, 3, 7, a._begin, a._end, alloc, "copy constructor test");
        ASSERT(a.size() == 3, "copy constructor iterator test 1");
        ASSERT(a[0] == 1, "copy constructor test 1");
        ASSERT(a[1] == 2, "copy constructor test 2");
        ASSERT(a[2] == 3, "copy constructor test 3");
    }
    {
        vec_123(v);
        ft::vector<int> a(v, alloc);
        vec_ASSERT(a, 3, 7, a._begin, a._end, alloc,
            "copy constructor + alloc test");
        ASSERT(a.size() == 3, "copy constructor + alloc iterator test 1");
        ASSERT(a[0] == 1, "copy constructor + alloc test 1");
        ASSERT(a[1] == 2, "copy constructor + alloc test 2");
        ASSERT(a[2] == 3, "copy constructor + alloc test 3");
    }
#if CPP11
    {
        vec_123(v);
        ft::vector<int> a(std::move(v));
        vec_ASSERT(a, 3, 7, a._begin, a._end, alloc,
            "move constructor test");
        ASSERT(a.size() == 3, "move constructor iterator test 1");
        ASSERT(a[0] == 1, "move constructor test 1");
        ASSERT(a[1] == 2, "move constructor test 2");
        ASSERT(a[2] == 3, "move constructor test 3");
        vec_ASSERT(v, 0, 0, nullptr, nullptr, alloc,
            "move constructor empty vector test");
    }
    {
        vec_123(v);
        ft::vector<int> a(std::move(v), alloc);
        vec_ASSERT(a, 3, 7, a._begin, a._end, alloc,
            "move + alloc constructor test");
        ASSERT(a.size() == 3, "move + alloc constructor iterator test 1");
        ASSERT(a[0] == 1, "move + alloc constructor test 1");
        ASSERT(a[1] == 2, "move + alloc constructor test 2");
        ASSERT(a[2] == 3, "move + alloc constructor test 3");
        vec_ASSERT(v, 0, 0, nullptr, nullptr, alloc, "move + alloc constructor empty vector test");
    }
    {
        ft::vector<int> a({1, 2, 3});
        vec_ASSERT(a, 3, 7, a._begin, a._end, alloc,
            "initializer list constructor test");
        ASSERT(a.size() == 3, "initializer list constructor iterator test 1");
        ASSERT(a[0] == 1, "initializer list constructor test 1");
        ASSERT(a[1] == 2, "initializer list constructor test 2");
        ASSERT(a[2] == 3, "initializer list constructor test 3");
    }
    {
        ft::vector<int> a({1, 2, 3}, alloc);
        vec_ASSERT(a, 3, 7, a._begin, a._end, alloc,
            "initializer list + alloc constructor test");
        ASSERT(a.size() == 3,
            "initializer list + alloc constructor iterator test 1");
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
        int _vec_[3] = {1, 1, 1};
        vec_cmp("sized assign test 1", int, a, _vec_);
    }

    {
        vec_123(a);
        ft::vector<int> b;
        a.assign(b.begin(), b.end());
        ASSERT(a.empty(), "iterator assign test 0");
    }
    {
        std_vec_123(a);
        ft::vector<int> b;
        b.assign(a.begin(), a.end());
        int _vec_[3] = {1, 2, 3};
        vec_cmp("iterator assign test 1", int, b, _vec_);
    }
    {
        ft::vector<int> a;
        vec_123(b);
        a.assign(b.begin(), b.end());
        int _vec_[] = {1, 2, 3};
        vec_cmp("iterator assign test 2", int, a, _vec_);
    }

    result();
}

void fn_get_allocator_test()
{
    start(".get_allocator() tests");

    {
        std::allocator<int> alloc;
        ft::vector<int> a(alloc);
        ASSERT(alloc == a.get_allocator(), "get allocator basic test 0");
    }
    {
        std::allocator<std::string> alloc;
        ft::vector<std::string> a(5, alloc);
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

    ft::vector<int> b;
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

    ft::vector<int> b;
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

    ft::vector<int> b;

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

    ft::vector<int> b;

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

    ft::vector<int> b;

    ASSERT(_op(b) == nullptr, "data empty pointer test 1");

    result();
}
#undef _op

template <class myVec>
void F_iterator_test_suite(myVec &a, myVec &b)
{
    start("iterator tests");


    ASSERT(*a.begin() == 1, "basic iterator test 1");
    ASSERT(*(++a.begin()) == 2, "basic iterator test 2");
    ASSERT(*(++(++a.begin())) == 3, "basic iterator test 3");

    ASSERT(*(--a.end()) == 3, "basic iterator test 4");
    ASSERT(*(--(--a.end())) == 2, "basic iterator test 5");
    ASSERT(*(--(--(--a.end()))) == 1, "basic iterator test 6");

    ASSERT(a.begin()++ == a.begin(), "basic increment iterator test 1");
    ASSERT(++a.begin() > a.begin(), "basic increment iterator test 2");
    ASSERT(a.end()-- == a.end(), "basic increment iterator test 3");
    ASSERT(--a.end() < a.end(), "basic increment iterator test 4");

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


    ASSERT(b.begin() == b.end(), "iterator empty test 1");

    const_std_vec_123(_a);
    ASSERT((a.begin() < a.end()) == (_a.begin() < _a.end()),
        "basic iterator test 12");
    ASSERT((a.begin() > a.end()) == (_a.begin() > _a.end()),
        "basic iterator test 13");
    ASSERT((a.begin() <= a.end()) == (_a.begin() <= _a.end()),
        "basic iterator test 14");
    ASSERT((a.begin() >= a.end()) == (_a.begin() >= _a.end()),
        "basic iterator test 15");
    ASSERT((a.begin() <= a.end() - 3) == (_a.begin() <= _a.end() - 3),
        "basic iterator tes 16");
    ASSERT((a.begin() >= a.end() - 3) == (_a.begin() >= _a.end() - 3),
        "basic iterator tes 17");
    ASSERT((a.end() > a.begin()) == (_a.end() > _a.begin()),
        "basic iterator test 18");
    ASSERT((a.end() < a.begin()) == (_a.end() < _a.begin()),
        "basic iterator test 19");
    ASSERT((a.end() >= a.begin()) == (_a.end() >= _a.begin()),
        "basic iterator test 20");
    ASSERT((a.end() <= a.begin()) == (_a.end() <= _a.begin()),
        "basic iterator test 21");
    ASSERT((a.end() >= a.begin() + 3) == (_a.end() >= _a.begin() + 3),
        "basic iterator test 22");
    ASSERT((a.end() <= a.begin() + 3) == (_a.end() <= _a.begin() + 3),
        "basic iterator test 23");

    result();
}

void iterator_test()
{
    vec_123(a);
    ft::vector<int> b;
    F_iterator_test_suite(a, b);
}

void const_iterator_test()
{
    const_vec_123(a);
    const ft::vector<int> b;
    F_iterator_test_suite(a, b);
}

template <class myVec>
void F_reverse_iterator_test_suite(myVec &a, myVec &b)
{
    start("reverse iterator tests");

    ASSERT(*(a.rbegin()) == 3, "basic reverse iterator test 1");
    ASSERT(*(++a.rbegin()) == 2, "basic reverse iterator test 2");
    ASSERT(*(++(++a.rbegin())) == 1, "basic reverse iterator test 3");

    ASSERT(*(--a.rend()) == 1, "basic reverse iterator test 4");
    ASSERT(*(--(--a.rend())) == 2, "basic reverse iterator test 5");
    ASSERT(*(--(--(--a.rend()))) == 3, "basic reverse iterator test 6");

    ASSERT(a.rbegin()++ == a.rbegin(),
        "basic increment reverse iterator test 1");
    ASSERT(a.rbegin() < ++a.rbegin(),
        "basic increment reverse iterator test 2");
    ASSERT(a.rend()-- == a.rend(), "basic increment reverse iterator test 3");
    ASSERT(a.rend() > --a.rend(), "basic increment reverse iterator test 4");

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

    ASSERT(b.begin() == b.end(), "iterator empty test 1");

    const_std_vec_123(_a);
    ASSERT((a.rbegin() < a.rend()) == (_a.rbegin() < _a.rend()),
        "basic iterator test 12");
    ASSERT((a.rbegin() > a.rend()) == (_a.rbegin() > _a.rend()),
        "basic iterator test 13");
    ASSERT((a.rbegin() <= a.rend()) == (_a.rbegin() <= _a.rend()),
        "basic iterator test 14");
    ASSERT((a.rbegin() >= a.rend()) == (_a.rbegin() >= _a.rend()),
        "basic iterator test 15");
    ASSERT((a.rbegin() <= a.rend() - 3) == (_a.rbegin() <= _a.rend() - 3),
        "basic iterator tes 16");
    ASSERT((a.rbegin() >= a.rend() - 3) == (_a.rbegin() >= _a.rend() - 3),
        "basic iterator tes 17");
    ASSERT((a.rend() > a.rbegin()) == (_a.rend() > _a.rbegin()),
        "basic iterator test 18");
    ASSERT((a.rend() < a.rbegin()) == (_a.rend() < _a.rbegin()),
        "basic iterator test 19");
    ASSERT((a.rend() >= a.rbegin()) == (_a.rend() >= _a.rbegin()),
        "basic iterator test 20");
    ASSERT((a.rend() <= a.rbegin()) == (_a.rend() <= _a.rbegin()),
        "basic iterator test 21");
    ASSERT((a.rend() >= a.rbegin() + 3) == (_a.rend() >= _a.rbegin() + 3),
        "basic iterator test 22");
    ASSERT((a.rend() <= a.rbegin() + 3) == (_a.rend() <= _a.rbegin() + 3),
        "basic iterator test 23");

    result();
}

void reverse_iterator_test()
{
    vec_123(a);
    ft::vector<int> b;
    F_reverse_iterator_test_suite(a, b);
}

void const_reverse_iterator_test()
{
    const_vec_123(a);
    const ft::vector<int> b;
    F_reverse_iterator_test_suite(a, b);
}

void fn_empty_tests()
{
    start(".empty() tests");

    {
        vec_123(a);
        ASSERT(not a.empty(), "empty basic test 0");
    }
    {
        ft::vector<int> a;
        ASSERT(a.empty(), "empty basic test 1");
    }

    result();
}

void fn_size_tests()
{
    start(".size() tests");

    {
        ft::vector<int> a;
        ASSERT(a.size() + 1 == 1, "basic size test 2");
        a.push_back(1);
        ASSERT(a.size() == 1, "basic size test 2");
    } {
        ft::vector<int> a(100);
        ASSERT(a.size() == 100, "basic size test 3");
    }

    result();
}

void fn_max_size_tests()
{
    start(".max_size() tests");

    ft::vector<int> a;
    ASSERT(a.max_size() == 9223372036854775807, "max_size test 1");

    result();
}

void fn_reserve_tests()
{
    start(".reserve() tests");

    ft::vector<int> a;
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
        ft::vector<int> v;
        ASSERT(v.capacity() == 0, "capacity empty test 0");
    }
    {
        vec_123(v);
        ASSERT(v.capacity() == 7, "capacity sized test 0");
    }
    {
        ft::vector<int> v;
        v.push_back(1);
        v.push_back(2);
        v.push_back(3);
        v.push_back(4);
        v.push_back(5);
        v.push_back(6);
        ASSERT(v.capacity() == 7, "capacity sized test 1");
        v.push_back(8);
        ASSERT(v.capacity() == 11, "capacity sized test 2");
    }

    result();
}

void fn_shrink_to_fit_tests()
{
    start(".shrink_to_fit() tests");

#if CPP11
    {
        ft::vector<int> a;
        a.shrink_to_fit();
        ASSERT(a.capacity() == 0, "shrink to fit basic test 0");
    }
    {
        vec_123(v);
        v.shrink_to_fit();
        ASSERT(v.capacity() == 7, "shrink to fit basic test 1");
    }
    {
        ft::vector<int> a = {1, 2, 3, 4, 5, 6, 7};
        a.shrink_to_fit();
        ASSERT(a.capacity() == 11, "shrink to fit basic test 2");
    }
#endif /* CPP11 */

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
        ft::vector<int> a;
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
        int _vec_[] = {123, 1, 2, 3};
        vec_cmp("basic insert test 0", int, a, _vec_);
    }
    {
        vec_123(a);
        a.insert(a.end(), 123);
        int _vec_[] = {1, 2, 3, 123};
        vec_cmp("basic insert test 1", int, a, _vec_);
    }
    {
        vec_123(a);
        a.insert(++a.begin(), 111);
        int _vec_[] = {1, 111, 2, 3};
        vec_cmp("basic insert test 2", int, a, _vec_);
    }

    {
        vec_123(a);
        a.insert(a.begin(), 3, 234);
        int _vec_[] = {234, 234, 234, 1, 2, 3};
        vec_cmp("basic insert test 3", int, a, _vec_);
    }
    {
        vec_123(a);
        a.insert(a.end(), 3, 432);
        int _vec_[] = {1, 2, 3, 432, 432, 432};
        vec_cmp("basic insert 4", int, a, _vec_);
    }
    {
        vec_123(a);
        a.insert(++a.begin(), 3, 345);
        int _vec_[] = {1, 345, 345, 345, 2, 3};
        vec_cmp("basic insert test 5", int, a, _vec_);
    }

    {
        vec_123(a);
        vec_111(b);
        a.insert(a.begin(), b.begin(), b.end());
        int _vec_[] = {123, 456, 789, 1, 2, 3};
        vec_cmp("basic insert test 6", int, a, _vec_);
    }
    {
        vec_123(a);
        vec_111(b);
        a.insert(a.end(), b.begin(), b.end());
        int _vec_[] = {1, 2, 3, 123, 456, 789};
        vec_cmp("basic insert test 7", int, a, _vec_);
    }
    {
        vec_123(a);
        vec_111(b);
        a.insert(++a.begin(), b.begin(), b.end());
        int _vec_[] = {1, 123, 456, 789, 2, 3};
        vec_cmp("basic insert test 8", int, a, _vec_);
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
    {
        ft::vector<int> a;
        a.insert(a.end(), 123);
        int _vec_[] = {123};
        vec_cmp("empty insert test 0", int, a, _vec_);
    }
    {
        ft::vector<int> a;
        a.insert(a.end(), 3, 123);
        int _vec_[] = {123, 123, 123};
        vec_cmp("empty insert test 1", int, a, _vec_);
    }
    {
        ft::vector<int> a;
        vec_123(b);
        a.insert(a.end(), b.begin(), b.end());
        int _vec_[] = {1, 2, 3};
        vec_cmp("empty insert test 2", int, a, _vec_);
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
        ft::vector<int> a;
        a.push_back(123);
        int _vec_[] = {123};
        vec_cmp("push back basic test 0", int, a, _vec_);
    }
    {
        vec_123(a);
        a.push_back(321);
        int _vec_[] = {1, 2, 3, 321};
        vec_cmp("push back basic test 1", int, a, _vec_);
    }

    {
        ft::vector<int> a;
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
        int _vec_[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11};
        vec_cmp("push back advanced test 0", int, a, _vec_);
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
        int _vec_[] = {1, 2};
        vec_cmp("pop back basic test 0", int, a, _vec_);
    }
    {
        ft::vector<int> a;
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
        ft::vector<int> a;
        a.resize(3);
        ASSERT(a.size() == 3, "resize basic test 0");
        ASSERT(a.capacity() == 7, "resize basic test 1");
    }
    {
        vec_123(a);
        a.resize(2);
        int _vec_[] = {1, 2};
        vec_cmp("resize basic test 2", int, a, _vec_);
    }
    {
        ft::vector<int> a;
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
        ft::vector<int> a;
        a.resize(4, 111);
        int _vec_[] = {111, 111, 111, 111};
        vec_cmp("resize basic test 8", int, a, _vec_);
    }
    {
        vec_123(a);
        a.resize(2, 888);
        int _vec_[] = {1, 2};
        vec_cmp("resize basic test 9", int, a, _vec_);
    }
    {
        ft::vector<int> a;
        a.resize(6, 111);
        a.resize(1, 222);
        int _vec1_[] = {111};
        vec_cmp("resize basic test 10", int, a, _vec1_);
        a.resize(6, 333);
        int _vec2_[] = {111, 333, 333, 333, 333, 333};
        vec_cmp("resize basic test 11", int, a, _vec2_);
        ASSERT(a.capacity() == 7, "resize basic test 12");
        a.resize(7, 44);
        int _vec3_[] = {111, 333, 333, 333, 333, 333, 44};
        vec_cmp("resize basic test 13", int, a, _vec3_);
        ASSERT(a.capacity() == 11, "resize basic test 14");
        a.resize(0, 999);
        ASSERT(a.empty(), "resize basic test 15");
        a.resize(1, 145145);
        int _vec4_[] = {145145};
        vec_cmp("resize basic test 16", int, a, _vec4_);
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
        int _vec1_[] = {123, 456, 789};
        vec_cmp("basic swap test 0", int, a, _vec1_);
        int _vec2_[] = {1, 2, 3};
        vec_cmp("basic swap test 1", int, b, _vec2_);
    }
    {
        vec_123(a);
        ft::vector<int> b;
        a.swap(b);
        ASSERT(a.empty(), "basic swap test 3");
        int _vec_[] = {1, 2, 3};
        vec_cmp("basic swap test 4", int, b, _vec_);
    }
    {
        vec_123(a);
        ft::vector<int> b;
        b.swap(a);
        ASSERT(a.empty(), "basic swap test 3");
        int _vec_[] = {1, 2, 3};
        vec_cmp("basic swap test 4", int, b, _vec_);
    }

    result();
}

void std_vector_test()
{
    start("std::vector interaction tests");

    {
        vec_123(a);
        std::vector<int> b(a.begin(), a.end());
        int _vec_[] = {1, 2, 3};
        vec_cmp("vector iterator test 0", int, b, _vec_);
    }
    {
        std_vec_123(a);
        ft::vector<int> b(a.begin(), a.end());
        int _vec_[] = {1, 2, 3};
        vec_cmp("vector iterator test 1", int, b, _vec_);
    }
    {
        vec_123(a);
        std::vector<int> b(a.rbegin(), a.rend());
        int _vec_[] = {3, 2, 1};
        vec_cmp("vector iterator test 2", int, b, _vec_);
    }
    {
        std_vec_123(a);
        ft::vector<int> b(a.rbegin(), a.rend());
        int _vec_[] = {3, 2, 1};
        vec_cmp("vector iterator test 3", int, b, _vec_);
    }

    {
        vec_123(a);
        std::vector<int> b;
        b.assign(a.begin(), a.end());
        int _vec_[] = {1, 2, 3};
        vec_cmp("vector iterator test 2", int, b, _vec_);
    }
    {
        std_vec_123(a);
        ft::vector<int> b;
        b.assign(a.begin(), a.end());
        int _vec_[] = {1, 2, 3};
        vec_cmp("vector iterator test 3", int, b, _vec_);
    }

    {
        vec_123(a);
        std::vector<int> b;

        b.assign(++a.begin(), ++(++(a.begin())));
        int _vec1_[] = {2};
        vec_cmp("iterator operators test 0", int, b, _vec1_);

        b.assign(--(--a.end()), --a.end());
        int _vec2_[] = {2};
        vec_cmp("iterator operators test 1", int, b, _vec2_);

        b.assign(++a.rbegin(), ++(++a.rbegin()));
        int _vec3_[] = {2};
        vec_cmp("iterator operators test 2", int, b, _vec3_);

        b.assign(--(--a.rend()), --a.rend());
        int _vec4_[] = {2};
        vec_cmp("iterator operators test 3", int, b, _vec4_);

        b.assign(a.begin() + 1, a.end() + 0);
        int _vec5_[] = {2, 3};
        vec_cmp("iterator operators test 4", int, b, _vec5_);

        b.assign(a.end() - 3, a.end() - 0);
        int _vec6_[] = {1, 2, 3};
        vec_cmp("iterator operators test 5", int, b, _vec6_);

        b.assign(a.rbegin() + 1, a.rend() + 0);
        int _vec7_[] = {2, 1};
        vec_cmp("iterator operators test 6", int, b, _vec7_);

        b.assign(a.rend() - 3, a.rend() - 0);
        int _vec8_[] = {3, 2, 1};
        vec_cmp("iterator operators test 7", int, b, _vec8_);

        b.assign(a.begin() += 1, a.end() += 0);
        int _vec9_[] = {2, 3};
        vec_cmp("iterator operators test 4", int, b, _vec9_);

        b.assign(a.end() -= 3, a.end() -= 0);
        int _vecA_[] = {1, 2, 3};
        vec_cmp("iterator operators test 5", int, b, _vecA_);

        b.assign(a.rbegin() += 1, a.rend() += 0);
        int _vecB_[] = {2, 1};
        vec_cmp("iterator operators test 6", int, b, _vecB_);

        b.assign(a.rend() -= 3, a.rend() -= 0);
        int _vecC_[] = {3, 2, 1};
        vec_cmp("iterator operators test 7", int, b, _vecC_);
    }
    {
        std_vec_123(a);
        ft::vector<int> b;

        b.assign(++a.begin(), ++(++(a.begin())));
        int _vec1_[] = {2};
        vec_cmp("std iterator operators test 0", int, b, _vec1_);

        b.assign(--(--a.end()), --a.end());
        int _vec2_[] = {2};
        vec_cmp("std iterator operators test 1", int, b, _vec2_);

        b.assign(++a.rbegin(), ++(++a.rbegin()));
        int _vec3_[] = {2};
        vec_cmp("std iterator operators test 2", int, b, _vec3_);

        b.assign(--(--a.rend()), --a.rend());
        int _vec4_[] = {2};
        vec_cmp("std iterator operators test 3", int, b, _vec4_);

        b.assign(a.begin() + 1, a.end() + 0);
        int _vec5_[] = {2, 3};
        vec_cmp("std iterator operators test 4", int, b, _vec5_);

        b.assign(a.end() - 3, a.end() - 0);
        int _vec6_[] = {1, 2, 3};
        vec_cmp("std iterator operators test 5", int, b, _vec6_);

        b.assign(a.rbegin() + 1, a.rend() + 0);
        int _vec7_[] = {2, 1};
        vec_cmp("std iterator operators test 6", int, b, _vec7_);

        b.assign(a.rend() - 3, a.rend() - 0);
        int _vec8_[] = {3, 2, 1};
        vec_cmp("std iterator operators test 7", int, b, _vec8_);

        b.assign(a.begin() += 1, a.end() += 0);
        int _vec9_[] = {2, 3};
        vec_cmp("std iterator operators test 4", int, b, _vec9_);

        b.assign(a.end() -= 3, a.end() -= 0);
        int _vecA_[] = {1, 2, 3};
        vec_cmp("std iterator operators test 5", int, b, _vecA_);

        b.assign(a.rbegin() += 1, a.rend() += 0);
        int _vecB_[] = {2, 1};
        vec_cmp("std iterator operators test 6", int, b, _vecB_);

        b.assign(a.rend() -= 3, a.rend() -= 0);
        int _vecC_[] = {3, 2, 1};
        vec_cmp("std iterator operators test 7", int, b, _vecC_);
    }

    result();
}


void user_type_test()
{
    start("user type tests");

    {
        ft::vector<UserClass> a(3);
        ASSERT(UserClass::total_instances == 3, "user type constructor test 1");
    }
    ASSERT(UserClass::total_instances == 0, "user type constructor test 2");

    {
        ft::vector<UserClass> a(3);
        ASSERT(a.capacity() == 7, "allocator check 1");
        ASSERT(UserClass::total_instances == 3, "user type constructor test 3");
        a.resize(7);
        ASSERT(a.capacity() == 11, "allocator check 2");
        ASSERT(UserClass::total_instances == 7, "user type constructor test 4");
    }
    ASSERT(UserClass::total_instances == 0, "user type constructor test 5");

    {
        ft::vector<UserClass> a(3, UserClass(1, 2));
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
            ft::vector<UserClass> a;
            a.push_back(UserClass());
            ColString _vec_[] = {Def, Cpy, Del};
            vec_cmp_lock("user class modern test 0", ColString, moves, _vec_);
        }
        ColString _v_[] = {Def, Cpy, Del * 2};
        vec_cmp_lock("user class modern test 1", ColString, moves, _v_);
        ASSERT(UserClass::total_instances == 0, "user type test 9");
    }
    {{{
        moves.clear();
        {
            std::vector<UserClass> a;
            a.push_back(UserClass());
            ColString _vec_[] = {Def, Cpy, Del};
            vec_cmp_lock("std -- user class modern test 0",
                ColString, moves, _vec_);
        }
        ColString _v_[] = {Def, Cpy, Del * 2};
        vec_cmp_lock("std -- user class modern test 1", ColString, moves, _v_);
        ASSERT(UserClass::total_instances == 0, "std -- user type test 9");
    }}}
    UserClass::verbose = false;
    {
        moves.clear();
        {
            ft::vector<UserClass> a(6);
            ColString _vec1_[] = {Def * 6};
            make_std_vec_lock(cmp1, ColString, _vec1_);
            std_vec_cmp_lock(cmp1, moves, "user class modern test 2");
            moves.clear();
            a.push_back(UserClass(111, 222));
            ColString _vec2_[] = {Cons, Cpy * 6, Del * 6, Cpy, Del};
            make_std_vec_lock(cmp2, ColString, _vec2_);
            std_vec_cmp_lock(cmp2, moves, "user class modern test 3");
            moves.clear();
        }
        ColString _v_[] = {Del * 7};
        vec_cmp_lock("user class modern test 4", ColString, moves, _v_);
        ASSERT(UserClass::total_instances == 0, "user type test 10");
    }
    {{{
        moves.clear();
        {
            std::vector<UserClass> a(6);
            ColString _vec1_[] = {Def * 6};
            make_std_vec_lock(cmp1, ColString, _vec1_);
            std_vec_cmp_lock(cmp1, moves, "std -- user class modern test 2");
            moves.clear();
            a.push_back(UserClass(111, 222));
            ColString _vec2_[] = {Cons, Cpy * 7, Del * 7};
            make_std_vec_lock(cmp2, ColString, _vec2_);
            std_vec_cmp_lock(cmp2, moves, "std -- user class modern test 3");
            moves.clear();
        }
        ColString _v_[] = {Del * 7};
        vec_cmp_lock("std -- user class modern test 4", ColString, moves, _v_);
        ASSERT(UserClass::total_instances == 0, "std -- user type test 10");
    }}}
    {
        moves.clear();
        {
            ft::vector<UserClass> a(11, UserClass(123, 456));
                // 11 items + 18 allocated
            ColString _vec1_[] = {Cons, Cpy * 11, Del};
            vec_cmp_lock("user class modern test 5", ColString, moves, _vec1_);
            moves.clear();
            a.pop_back(); // 10 left + 1 del
            a.pop_back(); // 9 left + 2 del
            a.pop_back(); // 8 left + 3 del
            a.pop_back(); // 7 left + 4 del
            a.pop_back(); // 6 left + 5 del --> reallocation
            ColString _vec2_[] = {Del * 5, Cpy * 6, Del * 6};
            vec_cmp_lock("user class modern test 6", ColString, moves, _vec2_);
            moves.clear();
        }
        ColString _v_[] = {Del * 6};
        vec_cmp_lock("user class modern test 7", ColString, moves, _v_);
        ASSERT(UserClass::total_instances == 0, "user type test 11");
    }
    {{{
        moves.clear();
        {
            std::vector<UserClass> a(11, UserClass(123, 456));
                // 11 items + 18 allocated
            ColString _vec1_[] = {Cons, Cpy * 11, Del};
            vec_cmp_lock("std -- user class modern test 5",
                ColString, moves, _vec1_);
            moves.clear();
            a.pop_back(); // 10 left + 1 del
            a.pop_back(); // 9 left + 2 del
            a.pop_back(); // 8 left + 3 del
            a.pop_back(); // 7 left + 4 del
            a.pop_back(); // 6 left + 5 del --> reallocation
            ColString _vec2_[] = {Del * 5};
            vec_cmp_lock("std -- user class modern test 6",
                ColString, moves, _vec2_);
            moves.clear();
        }
        ColString _v_[] = {Del * 6};
        vec_cmp_lock("std -- user class modern test 7", ColString, moves, _v_);
        ASSERT(UserClass::total_instances == 0, "std -- user type test 11");
    }}}
    UserClass::verbose = false;
    {
        moves.clear();
        {
            ft::vector<UserClass> a(3);
            ColString _vec1_[] = {Def * 3};
            vec_cmp_lock("user class modern test 8", ColString, moves, _vec1_);
            moves.clear();
            a.erase(++a.begin());
            ColString _vec2_[] = {Icpy, Del};
            vec_cmp_lock("user class modern test 9", ColString, moves, _vec2_);
            moves.clear();
        }
        ColString _v_[] = {Del * 2};
        vec_cmp_lock("user class modern test 10", ColString, moves, _v_);
        ASSERT(UserClass::total_instances == 0, "user type test 12");
    }
    UserClass::verbose = false;
    {{{
        moves.clear();
        {
            std::vector<UserClass> a(3);
            ColString _vec1_[] = {Def * 3};
            vec_cmp_lock("std -- user class modern test 8",
                ColString, moves, _vec1_);
            moves.clear();
            a.erase(++a.begin());
            ColString _vec2_[] = {Icpy, Del};
            vec_cmp_lock("std -- user class modern test 9",
                ColString, moves, _vec2_);
            moves.clear();
        }
        ColString _v_[] = {Del * 2};
        vec_cmp_lock("std -- user class modern test 10", ColString, moves, _v_);
        ASSERT(UserClass::total_instances == 0, "std -- user type test 12");
    }}}
    {
        moves.clear();
        {
            ft::vector<UserClass> a(5);
            ColString _vec1_[] = {Def * 5};
            vec_cmp_lock("user class modern test 11", ColString, moves, _vec1_);
            moves.clear();
            a.erase(++a.begin(), --a.end());
            ASSERT(a.size() == 2, "user class test 13");
            ColString _vec2_[] = {Icpy, Del * 3};
            vec_cmp_lock("user class modern test 12", ColString, moves, _vec2_);
            moves.clear();
        }
        ColString _v_[] = {Del * 2};
        vec_cmp_lock("user class modern test 13", ColString, moves, _v_);
        ASSERT(UserClass::total_instances == 0, "user type test 14");
    }
    {{{
        moves.clear();
        {
            std::vector<UserClass> a(5);
            ColString _vec1_[] = {Def * 5};
            vec_cmp_lock("std -- user class modern test 11",
                ColString, moves, _vec1_);
            moves.clear();
            a.erase(++a.begin(), --a.end());
            ASSERT(a.size() == 2, "user class test 13");
            ColString _vec2_[] = {Icpy, Del * 3};
            vec_cmp_lock("std -- user class modern test 12",
                ColString, moves, _vec2_);
            moves.clear();
        }
        ColString _v_[] = {Del * 2};
        vec_cmp_lock("std -- user class modern test 13", ColString, moves, _v_);
        ASSERT(UserClass::total_instances == 0, "std -- user type test 14");
    }}}
    UserClass::verbose = false;
    {
        moves.clear();
        {
            ft::vector<UserClass> a;
            ASSERT(UserClass::total_instances == 0, "user type test 15");
            a.insert(a.end(), UserClass(1, 2));
            ColString _vec_[] = {Cons, Cpy, Del};
            vec_cmp_lock("user class modern test 14", ColString, moves, _vec_);
            moves.clear();
        }
        ColString _v_[] = {Del};
        vec_cmp_lock("user class modern test 15", ColString, moves, _v_);
        ASSERT(UserClass::total_instances == 0, "user type test 16");
    }
    {{{
        moves.clear();
        {
            std::vector<UserClass> a;
            ASSERT(UserClass::total_instances == 0, "std -- user type test 15");
            a.insert(a.end(), UserClass(1, 2));
            ColString _vec_[] = {Cons, Cpy, Del};
            vec_cmp_lock("std -- user class modern test 14",
                ColString, moves, _vec_);
            moves.clear();
        }
        ColString _v_[] = {Del};
        vec_cmp_lock("std -- user class modern test 15", ColString, moves, _v_);
        ASSERT(UserClass::total_instances == 0, "std -- user type test 16");
    }}}
    UserClass::verbose = false;
    {
        moves.clear();
        {
            ft::vector<UserClass> a(2);
            ColString _vec1_[] = {Def * 2};
            vec_cmp_lock("user class modern test 16", ColString, moves, _vec1_);
            moves.clear();
            {
                ft::vector<UserClass> b(3, UserClass(4, 5));
                ColString _vec2_[] = {Cons, Cpy * 3, Del};
                vec_cmp_lock("user class modern test 17",
                    ColString, moves, _vec2_);
                moves.clear();
                a.insert(++a.begin(), b.begin(), b.end());
                ColString _vec3_[] = {Cpy, Icpy, Cpy * 2};
                vec_cmp_lock("user class modern test 18",
                    ColString, moves, _vec3_);
                moves.clear();
                ASSERT(UserClass::total_instances == 8, "user type test 16.1");
            }
            ColString _v1_[] = {Del * 3};
            vec_cmp_lock("user class modern test 18.1", ColString, moves, _v1_);
            ASSERT(UserClass::total_instances == 5, "user type test 17");
            moves.clear();
        }
        ColString _v2_[] = {Del * 5};
        vec_cmp_lock("user class modern test 19", ColString, moves, _v2_);
        ASSERT(UserClass::total_instances == 0, "user type test 18");
    }
    {{{
        moves.clear();
        {
            std::vector<UserClass> a(2);
            ColString _vec1_[] = {Def * 2};
            vec_cmp_lock("std -- user class modern test 16",
                ColString, moves, _vec1_);
            moves.clear();
            {
                std::vector<UserClass> b(3, UserClass(4, 5));
                ColString _vec2_[] = {Cons, Cpy * 3, Del};
                vec_cmp_lock("std -- user class modern test 17",
                    ColString, moves, _vec2_);
                moves.clear();
                a.insert(++(++a.begin()), b.begin(), b.end());
                ColString _vec3_[] = {Cpy * 5, Del * 2};
                vec_cmp_lock("std -- user class modern test 18",
                    ColString, moves, _vec3_);
                moves.clear();
            }
            ColString _v1_[] = {Del * 3};
            vec_cmp_lock("std -- user class modern test 18.1",
                ColString, moves, _v1_);
            ASSERT(UserClass::total_instances == 5, "std -- user type test 17");
            moves.clear();
        }
        ColString _v2_[] = {Del * 5};
        vec_cmp_lock("std -- user class modern test 19",
            ColString, moves, _v2_);
        ASSERT(UserClass::total_instances == 0, "std -- user type test 18");
    }}}
    {
        moves.clear();
        {
            ft::vector<UserClass> a;
            ASSERT(UserClass::total_instances == 0, "user type test 19");
            {
                ft::vector<UserClass> b(3);
                ColString _vec1_[] = {Def * 3};
                vec_cmp_lock("user class modern test 20",
                    ColString, moves, _vec1_);
                moves.clear();
                a.insert(a.begin(), b.begin(), b.end());
                ColString _vec2_[] = {Cpy * 3};
                vec_cmp_lock("user class modern test 21",
                    ColString, moves, _vec2_);
                ASSERT(UserClass::total_instances == 6, "user type test 20");
                moves.clear();
            }
            ColString _vec3_[] = {Del * 3};
            vec_cmp_lock("user class modern test 22", ColString, moves, _vec3_);
            ASSERT(UserClass::total_instances == 3, "user type test 21");
            moves.clear();
        }
        ColString _v_[] = {Del * 3};
        vec_cmp_lock("user class modern test 23", ColString, moves, _v_);
        ASSERT(UserClass::total_instances == 0, "user type test 22");
    }
    {{{
        moves.clear();
        {
            std::vector<UserClass> a;
            ASSERT(UserClass::total_instances == 0, "std -- user type test 19");
            {
                std::vector<UserClass> b(3);
                ColString _vec1_[] = {Def * 3};
                vec_cmp_lock("std -- user class modern test 20",
                    ColString, moves, _vec1_);
                moves.clear();
                a.insert(a.begin(), b.begin(), b.end());
                ColString _vec2_[] = {Cpy * 3};
                vec_cmp_lock("std -- user class modern test 21",
                    ColString, moves, _vec2_);
                ASSERT(UserClass::total_instances == 6,
                    "std -- user type test 20");
                moves.clear();
            }
            ColString _v1_[] = {Del * 3};
            vec_cmp_lock("std -- user class modern test 22",
                ColString, moves, _v1_);
            ASSERT(UserClass::total_instances == 3, "std -- user type test 21");
            moves.clear();
        }
        ColString _v2_[] = {Del * 3};
        vec_cmp_lock("std -- user class modern test 23",
            ColString, moves, _v2_);
        ASSERT(UserClass::total_instances == 0, "std -- user type test 22");
    }}}
    result();
}

void operator_vector_test()
{
    start("operator vector tests");

    {
        ft::vector<int> a;
        const ft::vector<int> b;
        ASSERT(a == b, "compare vector test 0");
    }
    {
        vec_123(a);
        vec_123(b);
        ASSERT(a == b, "compare vector test 1");
    }
    {
        vec_123(a);
        const_vec_123(b);
        ASSERT(a == b, "compare vector test 2");
    }
    {
        const_vec_123(a);
        const_vec_123(b);
        ASSERT(a == b, "compare vector test 3");
    }

    {
        vec_123(a);
        vec_111(b);
        ASSERT(a != b, "compare vector test 1");
    }
    {
        vec_123(a);
        const_vec_111(b);
        ASSERT(a != b, "compare vector test 2");
    }
    {
        const_vec_123(a);
        const_vec_111(b);
        ASSERT(a != b, "compare vector test 3");
    }

    {
        vec_123(a);
        const_vec_111(b);
        ASSERT(b > a, "compare vector test 4");
        ASSERT(b >= a, "compare vector test 5");
        ASSERT(a < b, "compare vector test 6");
        ASSERT(a <= b, "compare vector test 7");
    }
    {
        const ft::vector<int> a;
        vec_123(b);
        ASSERT(b > a, "compare vector test 8");
        ASSERT(b >= a, "compare vector test 9");
        ASSERT(a < b, "compare vector test 10");
        ASSERT(a <= b, "compare vector test 11");
    }

    result();
}

void std_swap_tests()
{
    start("std::swap() tests");

    {
        vec_123(a);
        vec_111(b);
        std::swap(a, b);
        int _vec1_[] = {123, 456, 789};
        vec_cmp("basic swap test 0", int, a, _vec1_);
        int _vec2_[] = {1, 2, 3};
        vec_cmp("basic swap test 1", int, b, _vec2_);
    }
    {
        vec_123(a);
        ft::vector<int> b;
        std::swap(a, b);
        ASSERT(a.empty(), "basic swap test 3");
        int _vec_[] = {1, 2, 3};
        vec_cmp("basic swap test 4", int, b, _vec_);
    }
    {
        vec_123(a);
        ft::vector<int> b;
        std::swap(b, a);
        ASSERT(a.empty(), "basic swap test 3");
        int _vec_[] = {1, 2, 3};
        vec_cmp("basic swap test 4", int, b, _vec_);
    }

    result();
}

void other_tests()
{
    start("other tests");

#if CPP11
    {
        vec_123(a);
        ASSERT(*a.begin() == *a.cbegin(), "other test 0");
        ASSERT(*--a.cend() == *--a.cend(), "other test 1");
        ASSERT(*++a.rbegin() == *++a.crbegin(), "other test 2");
        ASSERT(*a.rend() == *a.crend(), "other test 3");
    }
    {
        ft::vector<int> a;
        a.insert(a.begin(), {1, 2, 3});
        vec_cmp("other test 4", int, a, 1, 2, 3);
    }
#endif

    result();
}
