/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   speed_test.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tlucanti <tlucanti@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/07 15:58:52 by tlucanti          #+#    #+#             */
/*   Updated: 2022/08/14 19:20:31 by tlucanti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "vector.hpp"
#include <vector>
#include "benchmark.hpp"

#undef CONTAINER_TYPE
# define CONTAINER_TYPE vector
#undef VALUE_TYPE
# define VALUE_TYPE int
#include "speed_test.hpp"

int main()
{
    double std_time;
    double ft_time;

    tlucanti::Benchmark bm("vector");

    bm.next_test("vec() constructor test");
    SMALL(default_constructor_test);

    RUN_TESTS("vec(size) constructor test", sized_constructor_test);

    RUN_TESTS("vec(vec) copy constructor test", copy_constructor_test);

    RUN_TESTS("vec(a.begin(), a.end()) iterator constructor test",
        iterator_constructor_test);

    RUN_TESTS("~vec() destructor test", destructor_test);

    RUN_TESTS("vec = vec assign operator test", assign_operator_test);

    RUN_TESTS("vec.swap(vec) swap method test", swap_test);

    bm.next_test(".at() method test");
    SMALL(at_method_test);

    bm.next_test("v[i] access operator test");
    SMALL(access_operator_test);

    bm.next_test(".front() method test");
    SMALL(front_method_test);

    bm.next_test(".back() method test");
    SMALL(back_method_test);

    bm.next_test(".data() method test");
    SMALL(data_method_test);

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

    bm.next_test(".capacity() method test");
    run_speed_test(capacity_test, 0, E7);
    bm.small(ft_time, std_time);
    run_speed_test(capacity_test, 100, E6);
    bm.medium(ft_time, std_time);

    RUN_TESTS(".reserve() method test", reserve_test);

#if CPP11
    RUN_TESTS(".shrink_to_fit() method test", shrink_to_fit_method_test);
#endif /* CPP11 */


}
