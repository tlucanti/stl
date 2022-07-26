
#include "vector.hpp"
#include <vector>
#include "benchmark.hpp"

struct timeval __tv_start_time = {};
struct timeval __tv_end_time = {};

extern ssize_t write(int fd, const void *buf, size_t size);

template <class T>
void constructor_test_1(UNUSED(std::size_t _), unsigned long long times)
{
    std::vector<T *> v(times);
    __test_start();
    for (unsigned long long i=0; i < times; ++i)
        v[i] = new T;
    __test_end();
    for (unsigned long long i=0; i < times; ++i)
    {
        USED_PTR(v.at(i));
        delete v.at(i);
    }
    USED(v);
}

template <class T>
void constructor_test_2(typename T::size_type size, unsigned long long times)
{
    std::vector<T *> v(times);
    __test_start();
    for (unsigned long long i=0; i < times; ++i)
        v[i] = new T(size);
    __test_end();
    for (unsigned long long i=0; i < times; ++i)
    {
        USED_PTR(v.at(i));
        delete v.at(i);
    }
    USED(v);
}

template <class T>
void constructor_test_3(typename T::size_type size, unsigned long long times)
{
    T a(size);
    std::vector<T *> v(times);
    __test_start();
    for (unsigned long long i=0; i < times; ++i)
        v[i] = new T(a);
    __test_end();
    for (unsigned long long i=0; i < times; ++i)
    {
        USED_PTR(v.at(i));
        delete v.at(i);
    }
    USED(v);
}

template <class T>
void constructor_test_4(typename T::size_type size, unsigned long long times)
{
    T a(size);
    std::vector<T *> v(times);
    __test_start();
    for (unsigned long long i=0; i < times; ++i)
        v[i] = new T(a.begin(), a.end());
    __test_end();
    for (unsigned long long i=0; i < times; ++i)
    {
        USED_PTR(v.at(i));
        delete v.at(i);
    }
    USED(v);
}

template <class T>
void destructor_test_1(typename T::size_type size, unsigned long long times)
{
    std::vector<T *> v(times);
    for (unsigned long long i=0; i < times; ++i)
    {
        v.at(i) = new T(size);
        USED_PTR(v.at(i));
    }
    __test_start();
    for (unsigned long long i=0; i < times; ++i)
        delete v[i];
    __test_end();
    USED(v);
}

#if PRECPP11
template <class T>
void assign_operator_test_1(typename T::size_type size, unsigned long long times)
{
    T a(size);
    std::vector<T> v(times);
    __test_start();
    for (unsigned long long i=0; i < times; ++i)
        v[i] = a;
    __test_end();
    for (unsigned long long i=0; i < times; ++i)
    {
        USED_PTR(v.at(i));
    }
    USED(v);
}
#endif /* PRECPP11 */

template <class T>
void at_method_test_1(UNUSED(typename T::size_type _), typename T::size_type times)
{
    std::vector<typename T::value_type *> dest(static_cast<std::size_t>(times));
    T v(times);
    __test_start();
    for (typename T::size_type i=0; i < times; ++i)
        dest[static_cast<std::size_t>(i)] = &(v.at(i));
    __test_end();
    USED(v);
    USED(dest);
}

template <class T>
void access_operator_test_1(UNUSED(typename T::size_type _), typename T::size_type times)
{
    std::vector<typename T::value_type *> dest(static_cast<std::size_t>(times));
    T v(times);
    __test_start();
    for (typename T::size_type i=0; i < times; ++i)
        dest[static_cast<std::size_t>(i)] = &(v[i]);
    __test_end();
    USED(v);
    USED(dest);
}

template <class T>
void front_method_test_1(UNUSED(typename T::size_type _), typename T::size_type times)
{
    std::vector<typename T::value_type *> dest(static_cast<std::size_t>(times));
    T v(times);
    __test_start();
    for (typename T::size_type i=0; i < times; ++i)
        dest[static_cast<std::size_t>(i)] = &(v.front());
    __test_end();
    USED(v);
    USED(dest);
}

template <class T>
void back_method_test_1(UNUSED(typename T::size_type _), typename T::size_type times)
{
    std::vector<typename T::value_type *> dest(static_cast<std::size_t>(times));
    T v(times);
    __test_start();
    for (typename T::size_type i=0; i < times; ++i)
        dest[static_cast<std::size_t>(i)] = &(v.back());
    __test_end();
    USED(v);
    USED(dest);
}

template <class T>
void data_method_test_1(UNUSED(typename T::size_type _), typename T::size_type times)
{
    std::vector<typename T::value_type *> dest(static_cast<std::size_t>(times));
    T v(times);
    __test_start();
    for (typename T::size_type i=0; i < times; ++i)
        dest[static_cast<std::size_t>(i)] = v.data();
    __test_end();
    USED(v);
    USED(dest);
}

template <class T>
void iterator_test_1(UNUSED(typename T::size_type _), typename T::size_type times)
{
    std::vector<typename T::iterator> dest(static_cast<std::size_t>(times));
    T v(times);
    __test_start();
    for (std::size_t i=0; i < static_cast<std::size_t>(times); ++i)
        dest[i] = v.begin();
    __test_end();
    USED(v);
    USED(dest);
}

template <class T>
void iterator_test_2(UNUSED(typename T::size_type _), typename T::size_type times)
{
    std::vector<typename T::iterator> dest(static_cast<std::size_t>(times));
    T v(times);
    __test_start();
    for (std::size_t i=0; i < static_cast<std::size_t>(times); ++i)
        dest[i] = v.end();
    __test_end();
    USED(v);
    USED(dest);
}

template <class T>
void iterator_test_3(UNUSED(typename T::size_type _), typename T::size_type times)
{
    std::vector<typename T::reverse_iterator> dest(static_cast<std::size_t>(times));
    T v(times);
    __test_start();
    for (std::size_t i=0; i < static_cast<std::size_t>(times); ++i)
        dest[i] = v.rbegin();
    __test_end();
    USED(v);
    USED(dest);
}

template <class T>
void iterator_test_4(UNUSED(typename T::size_type _), typename T::size_type times)
{
    std::vector<typename T::reverse_iterator> dest(static_cast<std::size_t>(times));
    T v(times);
    __test_start();
    for (std::size_t i=0; i < static_cast<std::size_t>(times); ++i)
        dest[i] = v.rend();
    __test_end();
    USED(v);
    USED(dest);
}

template <class T>
void capacity_test_1(unsigned long long size, unsigned long long times)
{
    std::vector<T *> v(times);
    std::vector<int> dest(times);
    for (unsigned long long i=0; i < times; ++i)
        v.at(i) = new T(static_cast<typename T::size_type>(size));
    __test_start();
    for (unsigned long long i=0; i < times; ++i)
        dest[i] = v[i]->empty();
    __test_end();
    for (unsigned long long i=0; i < times; ++i)
    {
        USED_PTR(v.at(i));
        delete v.at(i);
    }
    USED(v);
    USED(dest);
}

template <class T>
void capacity_test_2(unsigned long long size, unsigned long long times)
{
    std::vector<T *> v(times);
    std::vector<typename T::size_type> dest(times);
    for (unsigned long long i=0; i < times; ++i)
        v.at(i) = new T(static_cast<typename T::size_type>(size));
    __test_start();
    for (unsigned long long i=0; i < times; ++i)
        dest[i] = v[i]->size();
    __test_end();
    for (unsigned long long i=0; i < times; ++i)
    {
        USED_PTR(v.at(i));
        delete v.at(i);
    }
    USED(v);
    USED(dest);
}

template <class T>
void capacity_test_3(unsigned long long size, unsigned long long times)
{
    std::vector<T *> v(times);
    std::vector<typename T::size_type> dest(times);
    for (unsigned long long i=0; i < times; ++i)
        v.at(i) = new T(static_cast<typename T::size_type>(size));
    __test_start();
    for (unsigned long long i=0; i < times; ++i)
        dest[i] = v[i]->max_size();
    __test_end();
    for (unsigned long long i=0; i < times; ++i)
    {
        USED_PTR(v.at(i));
        delete v.at(i);
    }
    USED(v);
    USED(dest);
}

template <class T>
void capacity_test_4(unsigned long long size, unsigned long long times)
{
    std::vector<T *> v(times);
    std::vector<typename T::size_type> dest(times);
    for (unsigned long long i=0; i < times; ++i)
        v.at(i) = new T(static_cast<typename T::size_type>(size));
    __test_start();
    for (unsigned long long i=0; i < times; ++i)
        dest[i] = v[i]->capacity();
    __test_end();
    for (unsigned long long i=0; i < times; ++i)
    {
        USED_PTR(v.at(i));
        delete v.at(i);
    }
    USED(v);
    USED(dest);
}

template <class T>
void reserve_method_test_1(typename T::size_type size, typename T::size_type times)
{
    std::vector<T> v(static_cast<std::size_t>(times));
    __test_start();
    for (std::size_t i=0; i < static_cast<std::size_t>(times); ++i)
        v[i].resize(size);
    __test_end();
    USED(v);
}

#if CPP11
template <class T>
void shrink_to_fit_method_test_1(typename T::size_typen size, unsigned long long times)
{
    std::vector<T *> v(static_cast<std::size_t>(times));
    for (unsigned long long i=0; i < times; ++i)
        v.at(i) = new T(size);
    __test_start();
    for (unsigned long long i=0; i < times; ++i)
        v[i]->shrink_to_fit();
    __test_end();
    for (unsigned long long i=0; i < timse; ++i)
    {
        USED_PTR(v.at(i));
        delete v.at(i);
    }
    USED(v);
}
#endif /* CPP11 */

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
    run_speed_test(__func, E3, E6); \
    bm.medium(ft_time, std_time); \
} while (false)

#define LARGE(__func) do { \
    run_speed_test(__func, E6, E3); \
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

    bm.next_test("vec() constructor test");
    SMALL(constructor_test_1);

    RUN_TESTS("vec(size) constructor test", constructor_test_2);

    RUN_TESTS("vec(vec) copy constructor test", constructor_test_3);

    RUN_TESTS("vec(a.begin(), a.end()) iterator constructor test", constructor_test_4);

    RUN_TESTS("~vec() destructor test", destructor_test_1);

    RUN_TESTS("vec = a assign operator test", assign_operator_test_1);

    bm.next_test(".at() method test");
    SMALL(at_method_test_1);

    bm.next_test("v[i] access operator test");
    SMALL(access_operator_test_1);

    bm.next_test(".front() method test");
    SMALL(front_method_test_1);

    bm.next_test(".back() method test");
    SMALL(back_method_test_1);

    bm.next_test(".data() method test");
    SMALL(data_method_test_1);

    bm.next_test(".begin() iterator test");
    SMALL(iterator_test_1);

    bm.next_test(".end() iterator test");
    SMALL(iterator_test_2);

    bm.next_test(".rbegin() iterator test");
    SMALL(iterator_test_3);

    bm.next_test(".rend() iterator test");
    SMALL(iterator_test_4);

    bm.next_test(".empty() method test");
    run_speed_test(capacity_test_1, 0, E7);
    bm.small(ft_time, std_time);
    run_speed_test(capacity_test_1, 100, E6);
    bm.medium(ft_time, std_time);

    bm.next_test(".size() method test");
    run_speed_test(capacity_test_2, 0, E7);
    bm.small(ft_time, std_time);
    run_speed_test(capacity_test_2, 100, E6);
    bm.medium(ft_time, std_time);

    bm.next_test(".max_size() method test");
    SMALL(capacity_test_3);

    bm.next_test(".capacity() method test");
    run_speed_test(capacity_test_4, 0, E7);
    bm.small(ft_time, std_time);
    run_speed_test(capacity_test_4, 100, E6);
    bm.medium(ft_time, std_time);

    RUN_TESTS(".reserve() method test", reserve_method_test_1);

#if CPP11
    RUN_TESTS(".shrink_to_fit() method test", shrink_to_fit_method_test);
#endif /* CPP11 */


}
