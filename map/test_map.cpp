
#include "map_base.hpp"
#include "test.hpp"
#include <map>


void constructor_assign_test();
void get_allocator_test();
void at_access_test();
void iterator_test();
void empty_size_test();
void max_size_test();
void clear_test();
void insert_test();
void erase_test();
void swap_test();
void count_test();
void find_test();
void bound_test();

int main()
{
    run_test(constructor_assign_test);
    run_test(get_allocator_test);
    run_test(at_access_test);
    run_test(iterator_test);
    run_test(empty_size_test);
    run_test(max_size_test);
    run_test(clear_test);
    run_test(insert_test);
    run_test(erase_test);
    run_test(swap_test);
    run_test(count_test);
    run_test(find_test);
    run_test(bound_test);

    final();
}


void constructor_assign_test() {
    start("map constructor/assign test");

    {
        ASSERT((tlucanti::is_same<bool, tlucanti::map_base<int, int>::value_compare::result_type>::value), "typedef test 0");
        ASSERT((tlucanti::is_same<int, tlucanti::map_base<int, int>::value_compare::first_argument_type>::value), "typedef test 1");
        ASSERT((tlucanti::is_same<int, tlucanti::map_base<int, int>::value_compare::second_argument_type>::value), "typedef test 2");
    }
    {
        tlucanti::map_base<int, int> a;
        ASSERT(a.empty(), "basic constructor test 0");
    }
    {
        std::allocator<int> alloc;
        std::less<int> cmp;
        tlucanti::TLU_NAMESPACE_HIDDEN::pair_key_compare<std::less<int>, tlucanti::map_base<int, int>::value_type> pair_cmp(cmp);
        (void)pair_cmp;
        tlucanti::map_base<int, int> a(cmp, alloc);
    }
    {
        std::map<int, int> stda;
        std::less<int> cmp;
        std::allocator<int> alloc;
        stda[1] = 2;
        stda[2] = 3;
        stda[3] = 6;
        tlucanti::map_base<int, int> a(stda.begin(), stda.end(), cmp, alloc);
        ASSERT(a[1] == 2, "basic iterator constructor test 0");
        ASSERT(a[2] == 3, "basic iterator constructor test 1");
        ASSERT(a[3] == 6, "basic iterator constructor test 2");
        tlucanti::map_base<int, int> b(a);
        ASSERT(b[1] == 2, "basic copy constructor test 0");
        ASSERT(b[2] == 3, "basic copy constructor test 1");
        ASSERT(b[3] == 6, "basic copy constructor test 2");
        tlucanti::map_base<int, int> c;
        ASSERT(c.empty(), "basic assign operator test 0");
        c = a;
        ASSERT(c[1] == 2, "basic copy constructor test 0");
        ASSERT(c[2] == 3, "basic copy constructor test 1");
        ASSERT(c[3] == 6, "basic copy constructor test 2");

        (void)cmp;
    }

    result();
}

void get_allocator_test()
{
    start("get_allocator test");

    {
        std::allocator<int> alloc;
        std::less<int> cmp;
        tlucanti::map_base<int, int> a(cmp, alloc);
        ASSERT(a.get_allocator() == alloc, "basic get_allocator tes 0");
    }

    result();
}

void at_access_test()
{
    start("access/access test");

    {
        tlucanti::map_base<int, int> a;
        error_test(a.at(0), "at test 0");
        error_test(a.at(123123), "at test 1");
        int q = a[0];
        ASSERT(q == 0, "access tes 0");
        ASSERT(a.at(0) == 0, "at test 2");
        ASSERT(a[0] == 0, "access test 1");
        a[1] = 123;
        ASSERT(a.at(1) == 123, "at test 3");
        ASSERT(a[1] == 123, "access test 2");
        a.at(1) = 321;
        ASSERT(a.at(1) == 321, "at test 4");
        ASSERT(a[1] == 321, "access test 3");
    }

    result();
}

void iterator_test()
{
    start("iterator test");

    {
        typedef tlucanti::map_base<int, int>::value_type P;
        tlucanti::map_base<int, int> a;
        a[1] = 2;
        a[2] = 3;
        a[123] = 321;

        ASSERT(*a.begin() == P(1, 2), "iterator test 0");
        ASSERT(*++a.begin() == P(2, 3), "iterator test 1");
        ASSERT(*++(++a.begin()) == P(123, 321), "iterator test 2");

        ASSERT(*--a.end() == P(123, 321), "iterator test 3");
        ASSERT(*--(--a.end()) == P(2, 3), "iterator test 4");
        ASSERT(*--(--(--a.end())) == P(1, 2), "iterator test 5");

        ASSERT(*a.rbegin() == P(1, 2), "reverse iterator test 0");
        ASSERT(*++a.rbegin() == P(2, 3), "reverse iterator test 1");
        ASSERT(*++(++a.rbegin()) == P(123, 321), "reverse iterator test 2");

        ASSERT(*--a.rend() == P(123, 321), "reverse iterator test 3");
        ASSERT(*--(--a.rend()) == P(2, 3), "reverse iterator test 4");
        ASSERT(*--(--(--a.rend())) == P(1, 2), "reverse iterator test 5");
    }

    result();
}

void empty_size_test()
{
    start("empty/size test");

    {
        tlucanti::map_base<int, int> a;
        ASSERT(a.empty(), "empty test 0");
        ASSERT(a.size() == 0, "size test 0");
        a[0] = 123;
        ASSERT(not a.empty(), "empty test 1");
        ASSERT(a.size() == 1, "size test 1");
    }

    result();
}

void max_size_test()
{
    start("max_size test");

    {
        tlucanti::map_base<int, int> a;
        ASSERT(a.max_size() == std::numeric_limits<std::size_t>::max(), "max_size() test 0");
    }

    result();
}

void clear_test()
{
    start("clear test");

    {
        tlucanti::map_base<int, int> a;
        a[0] = 1;
        a[1] = 2;
        a[2] = 3;
        ASSERT(a.size() == 3, "clear test 0");
        a.clear();
        ASSERT(a.empty(), "clear test 1");
    }

    result();
}

void insert_test()
{
    start("insert test");

    typedef tlucanti::map_base<int, int>::iterator IT;
    typedef tlucanti::pair_base<IT, bool> pIT;
    typedef tlucanti::map_base<int, int>::value_type P;
    {
        tlucanti::map_base<int, int> a;

        pIT i = a.insert(P(123, 321));
        ASSERT(a.size() == 1, "insert test 0");
        ASSERT(i.second == true, "insert test 1");
        ASSERT(*i.first == P(123, 321), "insert test 2");

        pIT j = a.insert(P(123, 321));
        ASSERT(a.size() == 1, "insert test 3");
        ASSERT(j.second == false, "insert test 4");
        ASSERT(*j.first == P(123, 321), "insert test 5");


        IT q = a.insert(a.begin(), P(11, 22));
        ASSERT(a.size() == 2, "hint-insert test 0");
        ASSERT(*q == P(11, 22), "hint-insert 2");

        IT w = a.insert(a.begin(), P(11, 22));
        ASSERT(a.size() == 2, "hint-inset test 3");
        ASSERT(*w == P(11, 22), "hint-insert test 5");
    }

    {
        std::map<int, int> stda;
        stda[0] = 123;
        stda[1] = 234;
        stda[2] = 345;

        tlucanti::map_base<int, int> a;
        a.insert(stda.begin(), stda.end());
        ASSERT(a.size() == 3, "iterator-insert test 0");
        ASSERT(*a.begin() == P(0, 123), "iterator-insert test 1");
    }

    result();
}

void erase_test()
{
    start("erase test");

    {
         tlucanti::map_base<int, int> a;
         a[0] = 123;
         ASSERT(a.size() == 1, "erase test 0");
         ASSERT(a.erase(a.begin()) == a.end(), "erase test 1");
         ASSERT(a.empty(), "erase test 2");
    }
    {
        tlucanti::map_base<int, int> a;
        a[0] = 123;
        a[1] = 321;
        a[3] = 111;
        ASSERT(a.size() == 3, "range-erase test 0");
        a.erase(++a.begin(), a.end());
        ASSERT(a.size() == 1, "range-erase test 1");
    }

    result();
}

void swap_test()
{
    start("swap test");

    {
        tlucanti::map_base<int, int> a;
        tlucanti::map_base<int, int> b;
        a[0] = 123;
        a[2] = 321;
        ASSERT(a.size() == 2, "swap test 0");
        ASSERT(b.empty(), "swap test 1");
        a.swap(b);
        ASSERT(a.empty(), "swap test 2");
        ASSERT(b.size() == 2, "swap test 3");
        a.swap(b);
        ASSERT(a.size() == 2, "swap test 4");
        ASSERT(b.empty(), "swap test 5");
    }

    result();
}

void count_test()
{
    start("count test");

    {
        tlucanti::map_base<int, int> a;
        ASSERT(a.count(0) == 0, "count test 0");
        ASSERT(a.count(11) == 0, "count test 1");
        a[0] = 123;
        a.insert(tlucanti::map_base<int, int>::value_type(11, 22));
        ASSERT(a.count(0) == 1, "count test 2");
        ASSERT(a.count(11) == 1, "count test 3");
        a[11] = 0;
        ASSERT(a.count(11) == 1, "count test 4");
    }

    result();
}

void find_test()
{
    start("find test");

    typedef tlucanti::map_base<int, int>::value_type P;
    {
        tlucanti::map_base<int, int> a;
        ASSERT(a.find(0) == a.end(), "find test 0");
        ASSERT(a.find(123) == a.end(), "find test 1");

        a[0] = 1;
        a[123] = 321;
        ASSERT(*a.find(1) == P(0, 1), "find test 2");
        ASSERT(*a.find(123) == P(123, 321), "find test 3");
    }

    result();
}

void bound_test()
{
    start("equal_range/lower/upper-bound test");

    typedef tlucanti::map_base<int, int>::iterator IT;
    typedef tlucanti::pair_base<IT, IT> P;
    {
        tlucanti::map_base<int, int> a;
        a[0] = 1;
        a[4] = 2;
        a[10] = 3;

        ASSERT(a.equal_range(0) == P(a.begin(), ++a.begin()), "equal_range test 0");
        ASSERT(a.equal_range(1) == P(++a.begin(), ++a.begin()), "equal_range test 1");
        ASSERT(a.equal_range(4) == P(++(++a.begin()), --a.end()), "equal_range test 2");
        ASSERT(a.equal_range(9) == P(--a.end(), --a.end()), "equal_range test 3");
        ASSERT(a.equal_range(10) == P(--a.end(), a.end()), "equal_range test 4");
        ASSERT(a.equal_range(123) == P(a.end(), a.end()), "equal_range test 5");
    }

    result();
}
