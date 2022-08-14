/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   speed_test.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tlucanti <tlucanti@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/14 11:26:42 by tlucanti          #+#    #+#             */
/*   Updated: 2022/08/14 19:20:12 by tlucanti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SPEED_TEST_HPP
# define SPEED_TEST_HPP

# include "benchmark.hpp"
# include "defs.hpp"
# include "type_traits.hpp"
# include <vector>

struct timeval __tv_start_time = {};
struct timeval __tv_end_time = {};

extern ssize_t write(int fd, const void *buf, size_t size);

template <class T>
void sized_container_generate(
        typename tlucanti::complete_size_type<T>::size_type size,
        typename tlucanti::enable_if<tlucanti::has_size_type<T>::value,
        T>::type &container
    )
{
    container.resize(size);
    for (typename T::size_type i=0; i < size; ++i)
        container[i] = static_cast<int>(i);
}

template <class T>
void sized_container_generate(
        typename tlucanti::enable_if<
            not tlucanti::has_size_type<T>::value,
            int
        >::type
    )
{}

template <class T>
void insert_container_generate(typename T::size_type size, T &container)
{
    for (typename T::size_type i=0; i < size; ++i)
        container.insert(static_cast<int>(i));
}

template <class T, typename gen_fn_t>
void default_constructor_test(
        UNUSED std::size_t _,
        unsigned long times,
        gen_fn_t
    )
/*
    container();
*/
{
    std::vector<T *> v(times);
    __test_start();
    for (unsigned long i=0; i < times; ++i)
        v[i] = new T;
    __test_end();
    for (unsigned long i=0; i < times; ++i)
    {
        USED_PTR(v.at(i));
        delete v.at(i);
    }
    USED(v);
}

template <class T, class gen_fn_t>
void sized_constructor_test(
        typename T::size_type size,
        unsigned long times,
        gen_fn_t
    )
/*
    container(size);
*/
{
    std::vector<T *> v(times);
    __test_start();
    for (unsigned long i=0; i < times; ++i)
        v[i] = new T(size);
    __test_end();
    for (unsigned long i=0; i < times; ++i)
    {
        USED_PTR(v.at(i));
        delete v.at(i);
    }
    USED(v);
}

template <class T, class gen_fn_t>
void copy_constructor_test(
        typename T::size_type size,
        unsigned long times,
        gen_fn_t generate
    )
/*
    container(container);
*/
{
    T a;
    generate(size, a);
    std::vector<T *> v(times);
    __test_start();
    for (unsigned long i=0; i < times; ++i)
        v[i] = new T(a);
    __test_end();
    for (unsigned long i=0; i < times; ++i)
    {
        USED_PTR(v.at(i));
        delete v.at(i);
    }
    USED(v);
}

template <class T, class gen_fn_t>
void empty_copy_constructor_test(
        UNUSED std::size_t _,
        unsigned long times,
        gen_fn_t
    )
/*
    empty_container(empty_container);
*/
{
    T a;
    std::vector<T *> v(times);
    __test_start();
    for (unsigned long i=0; i < times; ++i)
        v[i] = new T(a);
    __test_end();
    for (unsigned long i=0; i < times; ++i)
    {
        USED_PTR(v.at(i));
        delete v.at(i);
    }
    USED(v);
}

template <class T, typename gen_fn_t>
void iterator_constructor_test(
        typename T::size_type size,
        unsigned long times,
        gen_fn_t generate
    )
/*
    container(a.begin(), a.end());
*/
{
    T a;
    generate(size, a);
    std::vector<T *> v(times);
    __test_start();
    for (unsigned long i=0; i < times; ++i)
        v[i] = new T(a.begin(), a.end());
    __test_end();
    for (unsigned long i=0; i < times; ++i)
    {
        USED_PTR(v.at(i));
        delete v.at(i);
    }
    USED(v);
}

template <class T, class gen_fn_t>
void destructor_test(
        typename T::size_type size,
        unsigned long times,
        gen_fn_t generate
    )
/*
    delete container;
*/
{
    std::vector<T *> v(times);
    for (unsigned long i=0; i < times; ++i)
    {
        v.at(i) = new T;
        generate(size, *v.at(i));
        USED_PTR(v.at(i));
    }
    __test_start();
    for (unsigned long i=0; i < times; ++i)
        delete v[i];
    __test_end();
    USED(v);
}

template <class T, class gen_fn_t>
void empty_destructor_test(UNUSED std::size_t _, unsigned long times, gen_fn_t)
/*
    delete empty_container;
*/
{
    std::vector<T *> v(times);
    for (unsigned long i=0; i < times; ++i)
    {
        v.at(i) = new T;
        USED_PTR(v.at(i));
    }
    __test_start();
    for (unsigned long i=0; i < times; ++i)
        delete v[i];
    __test_end();
    USED(v);
}

#if PRECPP11
template <class T, class gen_fn_t>
void assign_operator_test(
        typename T::size_type size,
        unsigned long times,
        gen_fn_t generate
    )
/*
    container = container;
*/
{
    T a;
    generate(size, a);
    std::vector<T> v(times);
    __test_start();
    for (unsigned long i=0; i < times; ++i)
        v[i] = a;
    __test_end();
    for (unsigned long i=0; i < times; ++i)
    {
        USED_PTR(v.at(i));
    }
    USED(v);
}

template <class T, class gen_fn_t>
void empty_assign_operator_test(
        UNUSED std::size_t _,
        unsigned long times,
        gen_fn_t
    )
/*
    empty_container = empty_container;
*/
{
    T a;
    std::vector<T> v(times);
    __test_start();
    for (unsigned long i=0; i < times; ++i)
        v[i] = a;
    __test_end();
    for (unsigned long i=0; i < times; ++i)
    {
        USED_PTR(v.at(i));
    }
    USED(v);
}
#endif /* PRECPP11 */

template <class T, typename gen_fn_t>
void swap_test(
        typename T::size_type size,
        std::size_t times,
        gen_fn_t generate
    )
/*
    container.swap(container);
*/
{
    T a;
    generate(size, a);
    std::vector<T *> v(times);
    for (std::size_t i=0; i < times; ++i)
    {
        v.at(i) = new T;
        generate(size, *v.at(i));
    }
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

template <class T, class gen_fn_t>
void empty_swap_test(UNUSED std::size_t _, std::size_t times, gen_fn_t)
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


template <class T, class gen_fn_t>
void at_method_test(
        UNUSED typename T::size_type _,
        typename T::size_type times,
        gen_fn_t
    )
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

template <class T, class gen_fn_t>
void access_operator_test(
        UNUSED typename T::size_type _,
        typename T::size_type times,
        gen_fn_t
    )
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

template <class T, class gen_fn_t>
void front_method_test(
        UNUSED typename T::size_type _,
        typename T::size_type times,
        gen_fn_t
    )
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

template <class T, class gen_fn_t>
void back_method_test(
        UNUSED typename T::size_type _,
        typename T::size_type times,
        gen_fn_t
    )
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

template <class T, class gen_fn_t>
void data_method_test(
        UNUSED typename T::size_type _,
        typename T::size_type times,
        gen_fn_t
    )
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

template <class T, typename gen_fn_t>
void begin_test(
        UNUSED typename T::size_type _,
        typename T::size_type times,
        gen_fn_t generate
    )
/*
    container.begin()
*/
{
    std::vector<typename T::iterator> dest(static_cast<std::size_t>(times));
    T v;
    generate(times, v);
    __test_start();
    for (std::size_t i=0; i < static_cast<std::size_t>(times); ++i)
        dest[i] = v.begin();
    __test_end();
    USED(v);
    USED(dest);
}

template <class T, typename gen_fn_t>
void end_test(
        UNUSED typename T::size_type _,
        typename T::size_type times,
        gen_fn_t generate
    )
/*
    container.end()
*/
{
    std::vector<typename T::iterator> dest(static_cast<std::size_t>(times));
    T v;
    generate(times, v);
    __test_start();
    for (std::size_t i=0; i < static_cast<std::size_t>(times); ++i)
        dest[i] = v.end();
    __test_end();
    USED(v);
    USED(dest);
}

template <class T, typename gen_fn_t>
void rbegin_test(
        UNUSED typename T::size_type _,
        typename T::size_type times,
        gen_fn_t generate
    )
/*
    container.rbegin();
*/
{
    std::vector<typename T::reverse_iterator> dest(static_cast<std::size_t>(times));
    T v;
    generate(times, v);
    __test_start();
    for (std::size_t i=0; i < static_cast<std::size_t>(times); ++i)
        dest[i] = v.rbegin();
    __test_end();
    USED(v);
    USED(dest);
}

template <class T, typename gen_fn_t>
void rend_test(
        UNUSED typename T::size_type _,
        typename T::size_type times,
        gen_fn_t generate
    )
/*
    container.rend();
*/
{
    std::vector<typename T::reverse_iterator> dest(
        static_cast<std::size_t>(times));
    T v;
    generate(times, v);
    __test_start();
    for (std::size_t i=0; i < static_cast<std::size_t>(times); ++i)
        dest[i] = v.rend();
    __test_end();
    USED(v);
    USED(dest);
}

template <class T, typename gen_fn_t>
void empty_test(unsigned long size, unsigned long times, gen_fn_t generate)
/*
    container.empty();
*/
{
    std::vector<T *> v(times);
    std::vector<int> dest(times);
    for (unsigned long i=0; i < times; ++i)
    {
        v.at(i) = new T;
        generate(static_cast<typename T::size_type>(size), *v.at(i));
    }
    __test_start();
    for (unsigned long i=0; i < times; ++i)
        dest[i] = v[i]->empty();
    __test_end();
    for (unsigned long i=0; i < times; ++i)
    {
        USED_PTR(v.at(i));
        delete v.at(i);
    }
    USED(v);
    USED(dest);
}

template <class T, typename gen_fn_t>
void size_test(unsigned long size, unsigned long times, gen_fn_t generate)
/*
    container.size();
*/
{
    std::vector<T *> v(times);
    std::vector<typename T::size_type> dest(times);
    for (unsigned long i=0; i < times; ++i)
    {
        v.at(i) = new T;
        generate(static_cast<typename T::size_type>(size), *v.at(i));
    }
    __test_start();
    for (unsigned long i=0; i < times; ++i)
        dest[i] = v[i]->size();
    __test_end();
    for (unsigned long i=0; i < times; ++i)
    {
        USED_PTR(v.at(i));
        delete v.at(i);
    }
    USED(v);
    USED(dest);
}

template <class T, class gen_fn_t>
void max_size_test(UNUSED std::size_t _, unsigned long times, gen_fn_t)
/*
    container.max_size();
*/
{
    std::vector<T *> v(times);
    std::vector<typename T::size_type> dest(times);
    for (unsigned long i=0; i < times; ++i)
        v.at(i) = new T;;
    __test_start();
    for (unsigned long i=0; i < times; ++i)
        dest[i] = v[i]->max_size();
    __test_end();
    for (unsigned long i=0; i < times; ++i)
    {
        USED_PTR(v.at(i));
        delete v.at(i);
    }
    USED(v);
    USED(dest);
}

template <class T, class gen_fn_t>
void capacity_test(unsigned long size, unsigned long times, gen_fn_t)
/*
    container.capacity();
*/
{
    std::vector<T *> v(times);
    std::vector<typename T::size_type> dest(times);
    for (unsigned long i=0; i < times; ++i)
        v.at(i) = new T(static_cast<typename T::size_type>(size));
    __test_start();
    for (unsigned long i=0; i < times; ++i)
        dest[i] = v[i]->capacity();
    __test_end();
    for (unsigned long i=0; i < times; ++i)
    {
        USED_PTR(v.at(i));
        delete v.at(i);
    }
    USED(v);
    USED(dest);
}

template <class T, class gen_fn_t>
void reserve_test(
        typename T::size_type size,
        typename T::size_type times,
        gen_fn_t
    )
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

template <class T, typename gen_fn_t>
void clear_test(
        typename T::size_type size,
        std::size_t times,
        gen_fn_t generate
    )
/*
    container.clear();
*/
{
    std::vector<T> v(times);
    for (std::size_t i=0; i < times; ++i)
    {
        generate(size, v.at(i));
        USED_PTR(v.at(i));
    }
    __test_start();
    for (std::size_t i=0; i < times; ++i)
        v[i].clear();
    __test_end();
    USED(v);
}

template <class T, typename gen_fn_t>
void insert_val_test(std::size_t size, std::size_t times, UNUSED gen_fn_t _)
/*
    container.insert(value);
*/
{
    std::vector<T> v(times);
    __test_start();
    for (std::size_t i=0; i < times; ++i)
    {
        for (std::size_t j=0; j < size; ++j)
            v[i].insert(static_cast<int>(j));
    }
    __test_end();
    USED(v);
}

template <class T, typename gen_fn_t>
void insert_iterator_test(
        std::size_t size,
        std::size_t times,
        UNUSED gen_fn_t _
    )
/*
    container.insert(pos, value);
*/
{
    std::vector<T> v(times);
    __test_start();
    for(std::size_t i=0; i < times; ++i)
    {
        typename T::iterator it = v[i].end();
        for (std::size_t j=0; j < size; ++j)
            it = v[i].insert(it, static_cast<int>(j));
    }
    __test_end();
    USED(v);
}

template <class T, typename gen_fn_t>
void ranged_insert_test(std::size_t size, std::size_t times, UNUSED gen_fn_t _)
/*
    container.insert(iterator, iterator);
*/
{
    std::vector<T> v(times);
    std::vector<int> a(size);
    for (std::size_t i=0; i < size; ++i)
        a.at(i) = static_cast<int>(i);
    __test_start();
    for (std::size_t i=0; i < times; ++i)
        v[i].insert(a.begin(), a.end());
    __test_end();
    USED(v);
}

template <class T, typename gen_fn_t>
void erase_iterator_test(std::size_t size, std::size_t times, gen_fn_t generate)
/*
    container.erase(iterator);
*/
{
    std::vector<T> v(times);
    for (std::size_t i=0; i < times; ++i)
    {
        USED_PTR(v.at(i));
        generate(size, v.at(i));
    }
    __test_start();
    for (std::size_t i=0; i < times; ++i)
    {
        for (std::size_t j=0; j < size; ++j)
            v[i].erase(v[i].begin());
    }
    __test_end();
    USED(v);
}

template <class T, typename gen_fn_t>
void ranged_erase_test(std::size_t size, std::size_t times, gen_fn_t generate)
/*
    container.erase(iterator, iterator);
*/
{
    std::vector<T> v(times);
    for (std::size_t i=0; i < times; ++i)
    {
        USED_PTR(v.at(i));
        generate(size, v.at(i));
    }
    __test_start();
    for (std::size_t i=0; i < times; ++i)
        v[i].erase(v[i].begin(), v[i].end());
    __test_end();
    USED(v);
}

template <class T, typename gen_fn_t>
void erase_val_test(std::size_t size, std::size_t times, gen_fn_t generate)
/*
    container.erase(value);
*/
{
    std::vector<T> v(times);
    for (std::size_t i=0; i < times; ++i)
    {
        USED_PTR(v.at(i));
        generate(size, v.at(i));
    }
    __test_start();
    for (std::size_t i=0; i < times; ++i)
    {
        for (std::size_t j=0; j < size; ++j)
            v[i].erase(static_cast<int>(j));
    }
    __test_end();
    USED(v);
}

template <class T, typename gen_fn_t>
void count_test(std::size_t size, std::size_t times, gen_fn_t generate)
/*
    container.count(value);
*/
{
    std::vector<T> v(times);
    std::vector<std::size_t> dest(size);
    for (std::size_t i=0; i < times; ++i)
    {
        USED_PTR(v.at(i));
        generate(size, v.at(i));
    }
    __test_start();
    for (std::size_t i=0; i < times; ++i)
    {
        for (std::size_t j=0; j < size; ++j)
            dest[j] = v[i].count(static_cast<int>(j));
    }
    __test_end();
    USED(v);
    USED(dest);
}

template <class T, typename gen_fn_t>
void find_test(std::size_t size, std::size_t times, gen_fn_t generate)
/*
    container.find(value);
*/
{
    std::vector<T> v(times);
    std::vector<typename T::iterator> dest(size);
    for (std::size_t i=0; i < times; ++i)
    {
        USED_PTR(v.at(i));
        generate(size, v.at(i));
    }
    __test_start();
    for (std::size_t i=0; i < times; ++i)
    {
        for (std::size_t j=0; j < size; ++j)
            dest[j] = v[i].find(static_cast<int>(j));
    }
    __test_end();
    USED(v);
    USED(dest);
}

template <class T, typename gen_fn_t>
void equal_range_test(std::size_t size, std::size_t times, gen_fn_t generate)
/*
    container.equal_range(value);
*/
{
    std::vector<T> v(times);
    std::vector<typename T::iterator> dest(size);
    for (std::size_t i=0; i < times; ++i)
    {
        USED_PTR(v.at(i));
        generate(size, v.at(i));
    }
    __test_start();
    for (std::size_t i=0; i < times; ++i)
    {
        for (std::size_t j=0; j < size; ++j)
            dest[j] = v[i].equal_range(static_cast<int>(j)).first;
    }
    __test_end();
    USED(v);
    USED(dest);
}

template <class T, typename gen_fn_t>
void lower_bound_test(std::size_t size, std::size_t times, gen_fn_t generate)
/*
    container.lower_bound(value);
*/
{
    std::vector<T> v(times);
    std::vector<typename T::iterator> dest(size);
    for (std::size_t i=0; i < times; ++i)
    {
        USED_PTR(v.at(i));
        generate(size, v.at(i));
    }
    __test_start();
    for (std::size_t i=0; i < times; ++i)
    {
        for (std::size_t j=0; j < size; ++j)
            dest[j] = v[i].lower_bound(static_cast<int>(j));
    }
    __test_end();
    USED(v);
    USED(dest);
}

template <class T, typename gen_fn_t>
void upper_bound_test(std::size_t size, std::size_t times, gen_fn_t generate)
/*
    container.lower_bound(value);
*/
{
    std::vector<T> v(times);
    std::vector<typename T::iterator> dest(size);
    for (std::size_t i=0; i < times; ++i)
    {
        USED_PTR(v.at(i));
        generate(size, v.at(i));
    }
    __test_start();
    for (std::size_t i=0; i < times; ++i)
    {
        for (std::size_t j=0; j < size; ++j)
            dest[j] = v[i].upper_bound(static_cast<int>(j));
    }
    __test_end();
    USED(v);
    USED(dest);
}

template <class T, typename gen_fn_t>
void next_iterator_test(std::size_t size, std::size_t times, gen_fn_t generate)
/*
    ++iterator;
*/
{
    std::vector<T> v(times);
    for (std::size_t i=0; i < times; ++i)
    {
        USED_PTR(v.at(i));
        generate(size, v.at(i));
    }
    __test_start();
    for (std::size_t i=0; i < times; ++i)
    {
        typename T::iterator it = v[i].begin();
        for (std::size_t j=0; j < size; ++j)
            ++it;
    }
    __test_end();
    USED(v);
}

template <class T, typename gen_fn_t>
void prev_iterator_test(std::size_t size, std::size_t times, gen_fn_t generate)
/*
    --iterator;
*/
{
    std::vector<T> v(times);
    for (std::size_t i=0; i < times; ++i)
    {
        USED_PTR(v.at(i));
        generate(size, v.at(i));
    }
    __test_start();
    for (std::size_t i=0; i < times; ++i)
    {
        typename T::iterator it = v[i].end();
        for (std::size_t j=0; j < size; ++j)
            --it;
    }
    __test_end();
    USED(v);
}


#if CPP11
template <class T>
void shrink_to_fit_method_test_1(
        typename T::size_typen size,
        unsigned long times
    )
{
    std::vector<T *> v(static_cast<std::size_t>(times));
    for (unsigned long i=0; i < times; ++i)
        v.at(i) = new T(size);
    __test_start();
    for (unsigned long i=0; i < times; ++i)
        v[i]->shrink_to_fit();
    __test_end();
    for (unsigned long i=0; i < timse; ++i)
    {
        USED_PTR(v.at(i));
        delete v.at(i);
    }
    USED(v);
}
#endif /* CPP11 */

#define run_speed_test(__func, __arg1, __arg2) do { \
	__func<ft::CONTAINER_TYPE<VALUE_TYPE> >( \
        __arg1, \
        __arg2, \
        sized_container_generate<ft::CONTAINER_TYPE<VALUE_TYPE> > \
    ); \
	ft_time = delta(__tv_start_time, __tv_end_time); \
	__func<std::CONTAINER_TYPE<VALUE_TYPE> >( \
        __arg1, \
        __arg2, \
        sized_container_generate<std::CONTAINER_TYPE<VALUE_TYPE> > \
    ); \
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

#define run_set_speed_test(__func, __arg1, __arg2) do { \
	__func<ft::CONTAINER_TYPE<VALUE_TYPE> >( \
        __arg1, \
        __arg2, \
        insert_container_generate<ft::CONTAINER_TYPE<VALUE_TYPE> > \
    ); \
	ft_time = delta(__tv_start_time, __tv_end_time); \
	__func<std::CONTAINER_TYPE<VALUE_TYPE> >( \
        __arg1, \
        __arg2, \
        insert_container_generate<std::CONTAINER_TYPE<VALUE_TYPE> > \
    ); \
	std_time = delta(__tv_start_time, __tv_end_time); \
} while (false)

#define SET_SMALL(__func) do { \
    run_set_speed_test(__func, 5, E6); \
    bm.small(ft_time, std_time); \
} while (false)

#define SET_MEDIUM(__func) do { \
    run_set_speed_test(__func, E3, E4); \
    bm.medium(ft_time, std_time); \
} while (false)

#define SET_LARGE(__func) do { \
    run_set_speed_test(__func, E5, E2); \
    bm.large(ft_time, std_time); \
} while (false)

#define RUN_SET_TESTS(__name, __func) do { \
    bm.next_test(__name); \
    SET_SMALL(__func); \
    SET_MEDIUM(__func); \
    SET_LARGE(__func); \
} while (false)

#define run_ctree_speed_test(__func, __arg1, __arg2) do { \
    GLUE2(ctree_, __func)<rb_tree>(__arg1, __arg2); \
    ft_time = delta(__tv_start_time, __tv_end_time); \
	__func<std::CONTAINER_TYPE<VALUE_TYPE> >( \
        __arg1, \
        __arg2, \
        insert_container_generate<std::CONTAINER_TYPE<VALUE_TYPE> > \
    ); \
	std_time = delta(__tv_start_time, __tv_end_time); \
} while (false)

#define CTREE_SMALL(__func) do { \
    run_ctree_speed_test(__func, 5, E6); \
    bm.small(ft_time, std_time); \
} while (false)

#define CTREE_MEDIUM(__func) do { \
    run_ctree_speed_test(__func, E3, E4); \
    bm.medium(ft_time, std_time); \
} while (false)

#define CTREE_LARGE(__func) do { \
    run_ctree_speed_test(__func, E5, E2); \
    bm.large(ft_time, std_time); \
} while (false)

#define CTREE_RUN_TESTS(__name, __func) do { \
    bm.next_test(__name); \
    CTREE_SMALL(__func); \
    CTREE_MEDIUM(__func); \
    CTREE_LARGE(__func); \
} while (false)

#endif /* SPEED_TEST */
