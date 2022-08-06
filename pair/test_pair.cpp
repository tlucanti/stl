
#include "test.hpp"
#include "pair.hpp"
#include <string>

void constructor_test();
void assignment_operator_test();
void make_pair_test();
void operators_test();

int main()
{
    signal(SIGSEGV, sigsegv_catcher);
    signal(SIGILL, sigill_cathcer);
    signal(SIGABRT, sigabrt_catcher);

    run_test(constructor_test);
    run_test(assignment_operator_test);
    run_test(make_pair_test);
    run_test(operators_test);

    final();
}

void constructor_test()
{
    start("constructor tests");

    {
        ft::pair<int, int> a;
        ft::pair<float, int> b;
        ft::pair<std::string, int> c;
        ft::pair<std::string, std::string> d;

        MAKE_USED(a);
        MAKE_USED(b);
        MAKE_USED(c);
        MAKE_USED(d);
    }
    {
        PAIR_SET_ASSERT(a, int, int, 123, 456, "constructor test 0");
        PAIR_SET_ASSERT(b, unsigned long long, int, 123, 5345, "constructor test 1");
        PAIR_SET_ASSERT(c, std::string, int, "lols", 234234, "constructor test 2");
        PAIR_SET_ASSERT(d, std::string, std::string, ")&RT#$*", "", "constructor test 3");

        MAKE_USED(a);
        MAKE_USED(b);
        MAKE_USED(c);
        MAKE_USED(d);
    }
    {
        PAIR_SET_ASSERT(a, int, int, 423, 124, "test 4");
        ft::pair<int, int> b(a);
        PAIR_ASSERT(a, 423, 124, "test 5");

        ft::pair<int, const char *> c(1432, "eee");
        ft::pair<int, std::string> d(c);
        PAIR_ASSERT(d, 1432, "eee", "test 6");

        MAKE_USED(a);
        MAKE_USED(b);
        MAKE_USED(c);
        MAKE_USED(d);
    }

    result();
}

void assignment_operator_test()
{
    start("assignment tests");

    {
        ft::pair<int, int> a(123, 346);
        ft::pair<int, int> b;
        b = a;
        PAIR_ASSERT(b, 123, 346, "assignment test 0");
    }
    {
        tlucanti::pair_base<float, float> a(123.345f, 432.666f);
        tlucanti::pair_base<double, double> b;

        b.operator=(a);
    }
    {
        ft::pair<const char *, const char *> a("123", "234");
        ft::pair<std::string, std::string> b("qwe", "rew");

        b.operator=(a);

        PAIR_ASSERT(b, "123", "234" ,"assignment test 2");
    }
    {
        ft::pair<int, int> a(123, 234);
        ft::pair<long, long> b(432, 432);

        b.operator=(a);
        PAIR_ASSERT(b, 123, 234, "assignment test 1");
    }

    result();
}

void make_pair_test()
{
    start("make pair tests");

    {
        PAIR_ASSERT(ft::make_pair(1, 2), 1, 2, "make_pair test 0");
    }
    {
        PAIR_ASSERT(ft::make_pair(1231431, std::string("lols")), 1231431, std::string("lols"), "make_pair test 1");
    }
    {
        ft::pair<int, int> a = ft::make_pair(1, 2);
        PAIR_ASSERT(a, 1, 2, "make_pair test 2");
    }
    {
        ft::pair<int, std::string> a = ft::make_pair(1, std::string("twsts"));
        PAIR_ASSERT(a, 1, "twsts", "make_pair test 3");
    }

    result();
}

void operators_test()
{
    start("lexicographical operators tests");

    {
        ft::pair<int, int> a(123, 567);
        ft::pair<int, int> b(234, 456);
        ASSERT(!(a == b), "lexicographical test 0");
        ASSERT(a != b, "lexicographical test 1");
        ASSERT(a < b, "lexicographical test 2");
        ASSERT(a <= b, "lexicographical test 3");
        ASSERT(!(a > b), "lexicographical test 4");
        ASSERT(!(a >= b), "lexicographical test 5");
    }
    {
        ft::pair<int, int> a(123, 234);
        ft::pair<int, int> b(123, 654);
        ASSERT(!(a == b), "lexicographical test 6");
        ASSERT(a != b, "lexicographical test 7");
        ASSERT(a < b, "lexicographical test 8");
        ASSERT(a <= b, "lexicographical test 9");
        ASSERT(!(a > b), "lexicographical test 10");
        ASSERT(!(a >= b), "lexicographical test 11");
    }
    {
        ft::pair<int, int> a(123, 543);
        ft::pair<int, int> b(123, 543);
        ASSERT(a == b, "lexicographical test 6");
        ASSERT(!(a != b), "lexicographical test 7");
        ASSERT(!(a < b), "lexicographical test 8");
        ASSERT(a <= b, "lexicographical test 9");
        ASSERT(!(a > b), "lexicographical test 10");
        ASSERT(a >= b, "lexicographical test 11");
    }

    result();
}
