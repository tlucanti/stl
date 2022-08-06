
#include "benchmark.hpp"
#include "defs.hpp"
#include <vector>

struct timeval __tv_start_time = {};
struct timeval __tv_end_time = {};

extern ssize_t write(int fd, const void *buf, size_t size);

template <class T>
void default_constructor_test(UNUSED(std::size_t _), unsigned long long times)
/*
    container();
*/
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
void sized_constructor_test(typename T::size_type size, unsigned long long times)
/*
    container(size);
*/
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
void copy_constructor_test(typename T::size_type size, unsigned long long times)
/*
    container(container);
*/
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
void empty_copy_constructor_test(UNUSED(std::size_t _), unsigned long long times)
/*
    empty_container(empty_container);
*/
{
    T a;
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
void iterator_constructor_test(typename T::size_type size, unsigned long long times)
/*
    container(a.begin(), a.end());
*/
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
void destructor_test(typename T::size_type size, unsigned long long times)
/*
    delete container;
*/
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

template <class T>
void empty_destructor_test(UNUSED(std::size_t _), unsigned long long times)
/*
    delete empty_container;
*/
{
    std::vector<T *> v(times);
    for (unsigned long long i=0; i < times; ++i)
    {
        v.at(i) = new T;
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
void assign_operator_test(typename T::size_type size, unsigned long long times)
/*
    container = container;
*/
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

template <class T>
void empty_assign_operator_test(UNUSED(std::size_t _), unsigned long long times)
/*
    empty_container = empty_container;
*/
{
    T a;
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
void swap_test(typename T::size_type size, std::size_t times)
/*
    container.swap(container);
*/
{
    T a(size);
    std::vector<T *> v(times);
    for (std::size_t i=0; i < times; ++i)
        v.at(i) = new T(size);
    __test_start();
    for (std::size_t i=0; i < times; ++i)
        v[i]->swap(a);
    __test_end();
    for (std::size_t i=0; i < times; ++i)
    {
        USED_PTR(v.at(i));
        delete v.at(i);
    }
    USED(v);
}

template <class T>
void empty_swap_test(UNUSED(std::size_t _), std::size_t times)
/*
    empty_container.swap(empty_container);
*/
{
    T a;
    std::vector<T> v(times);
    __test_start();
    for (typename T::size_type i=0; i < times; ++i)
        v[i].swap(a);
    __test_end();
    USED(v);
}


template <class T>
void at_method_test(UNUSED(typename T::size_type _), typename T::size_type times)
/*
    container.at(i);
*/
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
void access_operator_test(UNUSED(typename T::size_type _), typename T::size_type times)
/*
    container[i];
*/
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
void front_method_test(UNUSED(typename T::size_type _), typename T::size_type times)
/*
    container.front();
*/
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
void back_method_test(UNUSED(typename T::size_type _), typename T::size_type times)
/*
    container.back();
*/
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
void data_method_test(UNUSED(typename T::size_type _), typename T::size_type times)
/*
    container.data();
*/
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
void begin_test(UNUSED(typename T::size_type _), typename T::size_type times)
/*

*/
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
void end_test(UNUSED(typename T::size_type _), typename T::size_type times)
/*
    container.end()
*/
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
void rbegin_test(UNUSED(typename T::size_type _), typename T::size_type times)
/*
    container.rbegin();
*/
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
void rend_test(UNUSED(typename T::size_type _), typename T::size_type times)
/*
    container.rend();
*/
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
void empty_test(unsigned long long size, unsigned long long times)
/*
    container.empty();
*/
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
void size_test(unsigned long long size, unsigned long long times)
/*
    container.size();
*/
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
void max_size_test(unsigned long long size, unsigned long long times)
/*
    container.max_size();
*/
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
void capacity_test(unsigned long long size, unsigned long long times)
/*
    container.capacity();
*/
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
void reserve_test(typename T::size_type size, typename T::size_type times)
/*
    container.reserve();
*/
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
	__func<ft::CONTAINER_TYPE<VALUE_TYPE> >(__VA_ARGS__); \
	ft_time = delta(__tv_start_time, __tv_end_time); \
	__func<std::CONTAINER_TYPE<VALUE_TYPE> >(__VA_ARGS__); \
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
