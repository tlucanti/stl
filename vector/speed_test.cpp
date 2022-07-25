
#include "vector.hpp"
#include <vector>
#include <sys/time.h>
#include <unistd.h>
#include "benchmark.hpp"

struct timeval __tv_start_time = {};
struct timeval __tv_end_time = {};

template <class T>
void constructor_test_1(UNUSED(std::size_t _), int times)
{
    std::vector<T *> v(times);
    __test_start();
    for (long long i=0; i < times; ++i)
        v[i] = new T;
    __test_end();
    for (long long i=0; i < times; ++i)
    {
        USED_PTR(v[i]);
        delete v[i];
    }
    USED(v);
}

template <class T>
void constructor_test_2(typename T::size_type size, long long times)
{
    std::vector<T *> v(times);
    __test_start();
    for (long long i=0; i < times; ++i)
        v[i] = new T(size);
    __test_end();
    for (long long i=0; i < times; ++i)
    {
        USED_PTR(v[i]);
        delete v[i];
    }
    USED(v);
}

template <class T>
void constructor_test_3(typename T::size_type size, long long times)
{
    T a(size);
    std::vector<T *> v(times);
    __test_start();
    for (long long i=0; i < times; ++i)
        v[i] = new T(a);
    __test_end();
    for (long long i=0; i < times; ++i)
    {
        USED_PTR(v[i]);
        delete v[i];
    }
    USED(v);
}

template <class T>
void constructor_test_4(typename T::size_type size, long long times)
{
//    std::vector<int> vec(static_cast<std::size_t>(size));
//    for (long long i=0; i < times; ++i)
//    {
//        USED(T a(vec.begin(), vec.end()));
//        LOOP_ITERATION;
//    }
}

#if PRECPP11
template <class T>
void assign_operator_test_1(typename T::size_type size, long long times)
{
    T a(size);
    T b;
    for (long long i=0; i < times; ++i)
    {
        b = a;
    }
}
#endif /* PRECPP11 */

double delta(struct timeval start, struct timeval end)
{
	double start_usec = static_cast<double>(start.tv_usec) / 1e6;
	double end_usec = static_cast<double>(end.tv_usec) / 1e6;
	time_t sec_delta = end.tv_sec - start.tv_sec;
	double usec_delta = end_usec - start_usec;
	return static_cast<double>(sec_delta) + usec_delta;
}

#define run_speed_test(__func, ...) do { \
	__func<ft::vector<int> >(__VA_ARGS__); \
	ft_time = delta(__tv_start_time, __tv_end_time); \
	__func<std::vector<int> >(__VA_ARGS__); \
	std_time = delta(__tv_start_time, __tv_end_time); \
} while (false)

#define SMALL(__func) do { \
    run_speed_test(__func, 5, E7); \
    bm.small(ft_time, std_time); \
} while (false)

#define MEDIUM(__func) do { \
    /*run_speed_test(__func, E3, E5);*/ \
    bm.medium(ft_time, std_time); \
} while (false)

#define LARGE(__func) do { \
    /*run_speed_test(__func, E6, E3);*/ \
    bm.large(ft_time, std_time); \
} while (false)

#define RUN_TESTS(__name, __func) do { \
    bm.next_test(__name); \
    SMALL(__func); \
    MEDIUM(__func); \
    LARGE(__func); \
} while (false)

int main()
{

    double std_time;
    double ft_time;

    tlucanti::Benchmark bm("vector");

    bm.next_test("default constructor test");
    SMALL(constructor_test_1);

    RUN_TESTS("sized constructor test", constructor_test_2);

    RUN_TESTS("copy constructor test", constructor_test_3);

    RUN_TESTS("iterator constructor test", constructor_test_4);

//    RUN_TESTS("assign operator test", constructor_test_3);

}
