
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
	for (long long i=0; i < times; ++i)
	{
		USED(T a);
		LOOP_ITERATION;
	}
}

template <class T>
void constructor_test_2(typename T::size_type size, long long times)
{
    for (long long i=0; i < times; ++i)
    {
        USED(T a(size));
        LOOP_ITERATION;
    }
}

template <class T>
void constructor_test_3(typename T::size_type size, long long times) {
    USED(T a(size));
    for (long long i = 0; i < times; ++i) {
        USED(T b(a));
        LOOP_ITERATION;
    }
}

template <class T>
void constructor_test_3(long long size, long long times)
{
    std::vector<int> vec(static_cast<std::size_t>(size));
    for (long long i=0; i < times; ++i)
    {
        USED(T a(vec.begin(), vec.end()));
        LOOP_ITERATION;
    }
}

template <class T>
void assign_operator_test_1(long long size, long long times)
{
    T a(size);
    T b;
    for (long long i=0; i < times; ++i)
    {
//        b = a;
    }
}

#define E1 10LL
#define E2 100LL
#define E3 1000LL
#define E4 10000LL
#define E5 100000LL
#define E6 1000000LL
#define E7 10000000LL
#define E8 100000000LL
#define E9 1000000000LL
#define E10 10000000000LL
#define E11 100000000000LL
#define E12 1000000000000LL
#define E13 10000000000000LL
#define E14 100000000000000LL
#define E15 1000000000000000LL

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

#define SMALL(__func) do { \
    run_speed_test(assign_operator_test_1, 5, E10); \
    bm.small(ft_time, std_time); \
} while (false)

#define MEDIUM(__func) do { \
    run_speed_test(constructor_test_2, E3, E5); \
    bm.medium(ft_time, std_time); \
} while (false)

#define LARGE(__func) do { \
    run_speed_test(constructor_test_2, E6, E3); \
    bm.large(ft_time, std_time); \
} while (false)

#define RUN_TESTS(__name, __func) do { \
    bm.next_test("default constructor test"); \
    SMALL(__func); \
    MEDIUM(__func); \
    LARGE(__func); \
} while (false)

int main()
{
	struct timeval _start {};
	struct timeval _end {};
    double std_time;
    double ft_time;

    tlucanti::Benchmark bm("vector");

    bm.next_test("default constructor test");
    SMALL(constructor_test_1);

    RUN_TESTS("sized constructor test", constructor_test_2);

    RUN_TESTS("iterator constructor test", constructor_test_3);

//    RUN_TESTS("assign operator test", constructor_test_3);

}
