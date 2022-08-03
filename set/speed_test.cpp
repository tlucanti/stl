
#include "set.hpp"
#include <set>
#include "benchmark.hpp"

#undef CONTAINER_TYPE
# define CONTAINER_TYPE set
#undef VALUE_TYPE
# define VALUE_TYPE int
#include "speed_test.hpp"

int main()
{
    double std_time;
    double ft_time;

    tlucanti::Benchmark bm("set");

    bm.next_test("set() constructor test");
    SMALL(default_constructor_test);

    RUN_TESTS("set(a.begin(), a.end()) iterator constructor test", iterator_constructor_test);

    RUN_TESTS("set(set) copy constructor test", copy_constructor_test);

    RUN_TESTS("~set() destructor test", destructor_test);

    RUN_TESTS("set = set assign operator test", assign_operator_test);

    bm.next_test(".begin() iterator test");
    SMALL(begin_test);

    bm.next_test(".end() iterator test");
    SMALL(end_test);

    bm.next_test(".rbegin() iterator test");
    SMALL(rbegin_test);

    bm.next_test(".rend() iterator test");
    SMALL(rend_test);

    bm.next_test(".empty() method test");
    run_speed_test(empty_test, 0, E7);
    bm.small(ft_time, std_time);
    run_speed_test(empty_test, 100, E6);
    bm.medium(ft_time, std_time);

    bm.next_test(".size() method test");
    run_speed_test(size_test, 0, E7);
    bm.small(ft_time, std_time);
    run_speed_test(size_test, 100, E6);
    bm.medium(ft_time, std_time);

    bm.next_test(".max_size() method test");
    SMALL(max_size_test);


}