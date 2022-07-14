
#include <iostream>
#include "vector.hpp"
#include <vector>
#include <sys/time.h>
#include "benchmark.hpp"

#define USED(__x) volatile __x
#define LOOP_ITERATION asm("")

template <class T>
void constructor_test_1(int times=10000000)
{
	for (int i=0; i < times; ++i)
	{
		USED(T a);
		LOOP_ITERATION;
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

#define run_test(__func) do { \
	__test_start(); \
	__func<ft::vector<int>>(); \
	__test_end(); \
	ft_time = delta(_start, _end); \
	__test_start(); \
	__func<std::vector<int>>(); \
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

	run_test(constructor_test_1);

    bm.next_test("default constructor test");
    bm.small(ft_time, std_time);
    bm.medium(ft_time, std_time);
    bm.large(ft_time, std_time);

    bm.next_test("default constructor test");
    bm.small(ft_time, std_time);
    bm.medium(ft_time, std_time);
    bm.large(ft_time, std_time);
}
