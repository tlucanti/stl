
#include "pair.hpp"
#include <utility>
#include "benchmark.hpp"

#undef CONTAINER_TYPE
# define CONTAINER_TYPE pair
#undef VALUE_TYPE
# define VALUE_TYPE int, int
#include "speed_test.hpp"

int main()
{
    double std_time;
    double ft_time;

    tlucanti::Benchmark bm("pair");

    bm.next_test("pair() constructor test");
    SMALL(default_constructor_test);

    RUN_TESTS("pair(pair) copy constructor test", empty_copy_constructor_test);

    RUN_TESTS("~pair() destructor test", empty_destructor_test);

    RUN_TESTS("pair = pair assign operator test", empty_assign_operator_test);

}

