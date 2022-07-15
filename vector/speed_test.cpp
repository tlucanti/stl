
#include <iostream>
#include "vector.hpp"
#include <vector>
#include <sys/time.h>
#include "benchmark.hpp"

#define USED(__x) volatile __x
#define LOOP_ITERATION asm("")

template <class T>
void constructor_test_1(int times)
{
	for (int i=0; i < times; ++i)
	{
		USED(T a);
		LOOP_ITERATION;
	}
}

template <class T>
void constructor_test_2(typename T::size_type size, int times)
{
    for (int i=0; i < times; ++i)
    {
        USED(T a(size));
        LOOP_ITERATION;
    }
}

template <class T>
void constructor_test_3(typename T::size_type size, int times) {
    USED(T a(size));
    for (int i = 0; i < times; ++i) {
        USED(T b(a));
        LOOP_ITERATION;
    }
}

template <class T>
void constructor_test_3(int size, int times)
{
    std::vector<int> vec(static_cast<std::size_t>(size));
    for (int i=0; i < times; ++i)
    {
        USED(T a(vec.begin(), vec.end()));
        LOOP_ITERATION;
    }
}

template <class T>
void assign_operator_test_1(int size, int times)
{
    USED(T a(size));
    USED(T b);
    for (int i=0; i < times; ++i)
    {
        b = a;
    }
}


#define __test_start() gettimeofday(&_start, NULL)
#define __test_end()   gettimeofday(&_end, NULL)

double delta(struct timeval start, struct timeval end)
{
	double start_usec = static_cast<double>(start.tv_usec) / 1e6;
	double end_usec = static_cast<double>(end.tv_usec) / 1e6;
	time_t sec_delta = end.tv_sec - start.tv_sec;
	double usec_delta = end_usec - start_usec;
	return static_cast<double>(sec_delta) + usec_delta;
}

#define run_speed_test(__func, ...) do { \
	__test_start(); \
	__func<ft::vector<int>>(__VA_ARGS__); \
	__test_end(); \
	ft_time = delta(_start, _end); \
	__test_start(); \
	__func<std::vector<int>>(__VA_ARGS__); \
	__test_end(); \
	std_time = delta(_start, _end); \
} while (false)
	

int main()
{
	struct timeval _start {};
	struct timeval _end {};
    double std_time;
    double ft_time;

    tlucanti::Benchmark bm("vector");

    bm.next_test("test");
    bm.small(0.02, 1);
    bm.small(0.3, 1);
    bm.small(0.5, 1);
    bm.small(0.7, 1);
    bm.small(0.9, 1);
    bm.small(1.1, 1);
    bm.small(1.5, 1);
    bm.small(2, 1);
    bm.small(5, 1);
    bm.small(10, 1);
    bm.small(18, 1);

// ----------------------------------------------------------------------------
    bm.next_test("default constructor test");

    run_speed_test(constructor_test_1, 100000000);
    bm.large(ft_time, std_time);

// ----------------------------------------------------------------------------
    bm.next_test("sized constructor test");

    run_speed_test(constructor_test_2, 5, 1000000);
    bm.small(ft_time, std_time);

    run_speed_test(constructor_test_2, 1000, 1000);
    bm.medium(ft_time, std_time);

    run_speed_test(constructor_test_2, 1000000, 100);
    bm.large(ft_time, std_time);

// ----------------------------------------------------------------------------
    bm.next_test("iterator constructor test");

    run_speed_test(constructor_test_3, 5, 1000000);
    bm.small(ft_time, std_time);

    run_speed_test(constructor_test_2, 1000, 1000);
    bm.medium(ft_time, std_time);

    run_speed_test(constructor_test_2, 1000000, 100);
    bm.large(ft_time, std_time);

// ----------------------------------------------------------------------------
    bm.next_test("copy constructor test");

    run_speed_test(constructor_test_3, 5, 1000000);
    bm.small(ft_time, std_time);

    run_speed_test(constructor_test_2, 1000, 1000);
    bm.medium(ft_time, std_time);

    run_speed_test(constructor_test_2, 1000000, 100);
    bm.large(ft_time, std_time);

}
