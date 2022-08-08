
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
    SET_SMALL(default_constructor_test);

    RUN_SET_TESTS("set(a.begin(), a.end()) iterator constructor test", iterator_constructor_test);

    RUN_SET_TESTS("set(set) copy constructor test", copy_constructor_test);

    RUN_SET_TESTS("~set() destructor test", destructor_test);

    RUN_SET_TESTS("set = set assign operator test", assign_operator_test);

    bm.next_test(".begin() iterator test");
    SET_SMALL(begin_test);

    bm.next_test(".end() iterator test");
    SET_SMALL(end_test);

    bm.next_test(".rbegin() iterator test");
    SET_SMALL(rbegin_test);

    bm.next_test(".rend() iterator test");
    SET_SMALL(rend_test);

    bm.next_test(".empty() method test");
    run_set_speed_test(empty_test, 0, E6);
    bm.small(ft_time, std_time);
    run_set_speed_test(empty_test, 100, E5);
    bm.medium(ft_time, std_time);

    bm.next_test(".size() method test");
    run_set_speed_test(size_test, 0, E6);
    bm.small(ft_time, std_time);
    run_set_speed_test(size_test, 100, E5);
    bm.medium(ft_time, std_time);

    bm.next_test(".max_size() method test");
    SET_SMALL(max_size_test);

    RUN_SET_TESTS(".clear() method test", clear_test);

    RUN_SET_TESTS(".insert(value) method test", insert_val_test);

    RUN_SET_TESTS(".insert(hint, value) method test", insert_iterator_test);

    RUN_SET_TESTS(".insert(first, last) ranged insert test", ranged_insert_test);

    RUN_SET_TESTS(".erase(pos) method test", erase_iterator_test);

    RUN_SET_TESTS(".erase(first, last) ranged erase test", ranged_erase_test);

    RUN_SET_TESTS(".erase(value) method test", erase_val_test);

    RUN_SET_TESTS(".swap() method test", swap_test);

    RUN_SET_TESTS(".count() method test", count_test);

    RUN_SET_TESTS(".find() method test", find_test);

    RUN_SET_TESTS(".equal_range() method test", equal_range_test);

    RUN_SET_TESTS(".lower_bound() method test", lower_bound_test);

    RUN_SET_TESTS(".upper_bound() method test", upper_bound_test);

    RUN_SET_TESTS("++iterator test", next_iterator_test);

    RUN_SET_TESTS("--iterator test", prev_iterator_test);
}

