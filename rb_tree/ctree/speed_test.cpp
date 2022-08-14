/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   speed_test.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tlucanti <tlucanti@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/14 11:26:42 by tlucanti          #+#    #+#             */
/*   Updated: 2022/08/14 19:50:12 by tlucanti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <set>
#include "rb_tree.h"
#include "benchmark.hpp"

#undef CONTAINER_TYPE
# define CONTAINER_TYPE set
#undef VALUE_TYPE
# define VALUE_TYPE int

#include "speed_test.hpp"

int _int_compare(void *lhs, void *rhs)
{
    return static_cast<int>(
            reinterpret_cast<std::ptrdiff_t>(lhs)
            - reinterpret_cast<std::ptrdiff_t>(rhs)
    );
}

void *_int_copy(void *v)
{
    return v;
}

void _int_destroy(void *) {}

void generate_tree(std::size_t size, rb_tree *tree)
{
    for (std::size_t i=0; i < size; ++i)
        rb_insert(tree, reinterpret_cast<void *>(i), _int_compare, nullptr);
}

#define RB_INIT_(__c) do { \
    (__c).root.node = nullptr; \
    (__c).begin.node = nullptr; \
    (__c).end.node = nullptr; \
    (__c).size = 0L; \
} while (false)

template <class T>
void ctree_default_constructor_test(UNUSED std::size_t size, std::size_t times)
{
    std::vector<T> v(times);
    __test_start();
    for (unsigned long i=0; i < times; ++i)
        RB_INIT_(v[i]);
    __test_end();
    for (unsigned long i=0; i < times; ++i)
        USED_PTR(v.at(i));
    USED(v);
}

template <class T>
void ctree_copy_constructor_test(std::size_t size, std::size_t times)
/*
    container(container);
*/
{
    T a = RB_TREE_INITIALIZER;
    generate_tree(size, &a);
    std::vector<T> v(times);
    for (std::size_t i=0; i < times; ++i)
        RB_INIT_(v.at(i));
    __test_start();
    for (unsigned long i=0; i < times; ++i)
        v[i] = rb_copy(&a, _int_copy);
    __test_end();
    for (unsigned long i=0; i < times; ++i)
    {
        USED_PTR(v.at(i));
        rb_destroy(&v.at(i), _int_destroy);
    }
    rb_destroy(&a, _int_destroy);
    USED(v);
}

template <class T>
void ctree_destructor_test(std::size_t size, std::size_t times)
/*
    delete container;
*/
{
    std::vector<T> v(times);
    for (std::size_t i=0; i < times; ++i)
    {
        RB_INIT_(v.at(i));
        generate_tree(size, &v.at(i));
    }
    __test_start();
    for (unsigned long i=0; i < times; ++i)
        rb_destroy(&v[i], _int_destroy);
    __test_end();
    USED(v);
}

template <class T>
void ctree_begin_test(std::size_t, std::size_t times)
/*
    container.begin()
*/
{
    std::vector<rb_node> dest(times);
    T v = RB_TREE_INITIALIZER;
    generate_tree(times, &v);
    __test_start();
    for (std::size_t i=0; i < static_cast<std::size_t>(times); ++i)
        dest[i] = v.begin;
    __test_end();
    rb_destroy(&v, _int_destroy);
    USED(v);
    USED(dest);
}

template <class T>
void ctree_end_test(std::size_t, std::size_t times)
/*
    container.begin()
*/
{
    std::vector<rb_node> dest(times);
    T v = RB_TREE_INITIALIZER;
    generate_tree(times, &v);
    __test_start();
    for (std::size_t i=0; i < static_cast<std::size_t>(times); ++i)
        dest[i] = v.end;
    __test_end();
    rb_destroy(&v, _int_destroy);
    USED(v);
    USED(dest);
}

template <class T>
void ctree_size_test(std::size_t size, std::size_t times)
/*
    container.size();
*/
{
    std::vector<T> v(times);
    std::vector<std::size_t> dest(times);
    for (unsigned long i=0; i < times; ++i)
    {
        RB_INIT_(v.at(i));
        generate_tree(size, &v.at(i));
    }
    __test_start();
    for (unsigned long i=0; i < times; ++i)
        dest[i] = v[i].size;
    __test_end();
    for (unsigned long i=0; i < times; ++i)
    {
        USED_PTR(v.at(i));
        rb_destroy(&v.at(i), _int_destroy);
    }
    USED(v);
    USED(dest);
}

template <class T>
void ctree_insert_val_test(std::size_t size, std::size_t times)
/*
    container.insert(value);
*/
{
    std::vector<T> v(times);
    for (std::size_t i=0; i < times; ++i)
        RB_INIT_(v.at(i));
    __test_start();
    for (std::size_t i=0; i < times; ++i)
    {
        for (std::size_t j=0; j < size; ++j)
            rb_insert(&v[i], reinterpret_cast<void *>(j),
                _int_compare, nullptr);
    }
    __test_end();
    for (std::size_t i=0; i < times; ++i)
    {
        USED_PTR(v.at(i));
        rb_destroy(&v.at(i), _int_destroy);
    }
    USED(v);
}

template <class T>
void ctree_insert_iterator_test(std::size_t size, std::size_t times)
/*
    container.insert(pos, value);
*/
{
    std::vector<T> v(times);
    for (std::size_t i=0; i < times; ++i)
        RB_INIT_(v.at(i));
    __test_start();
    for(std::size_t i=0; i < times; ++i)
    {
        rb_node it = v[i].end;
        for (std::size_t j=0; j < size; ++j)
            it = rb_insert_hint(&v[i], it, reinterpret_cast<void *>(j),
                _int_compare, nullptr);
    }
    __test_end();
    for (std::size_t i=0; i < times; ++i)
    {
        USED_PTR(v.at(i));
        rb_destroy(&v.at(i), _int_destroy);
    }
    USED(v);
}

template <class T>
void ctree_erase_iterator_test(std::size_t size, std::size_t times)
/*
    container.erase(iterator);
*/
{
    std::vector<T> v(times);
    for (std::size_t i=0; i < times; ++i)
    {
        USED_PTR(v.at(i));
        RB_INIT_(v.at(i));
        generate_tree(size, &v.at(i));
    }
    __test_start();
    for (std::size_t i=0; i < times; ++i)
    {
        for (std::size_t j=0; j < size; ++j)
            rb_remove_node(&v[i], v[i].begin, _int_destroy);
    }
    __test_end();
    USED(v);
}

template <class T>
void ctree_erase_val_test(std::size_t size, std::size_t times)
/*
    container.erase(value);
*/
{
    std::vector<T> v(times);
    for (std::size_t i=0; i < times; ++i)
    {
        USED_PTR(v.at(i));
        RB_INIT_(v.at(i));
        generate_tree(size, &v.at(i));
    }
    __test_start();
    for (std::size_t i=0; i < times; ++i)
    {
        for (std::size_t j=0; j < size; ++j)
            rb_remove(&v[i], reinterpret_cast<void *>(j),
                _int_compare, _int_destroy);
    }
    __test_end();
    USED(v);
}

template <class T>
void ctree_find_test(std::size_t size, std::size_t times)
/*
    container.find(value);
*/
{
    std::vector<T> v(times);
    std::vector<rb_node> dest(size);
    for (std::size_t i=0; i < times; ++i)
    {
        USED_PTR(v.at(i));
        RB_INIT_(v.at(i));
        generate_tree(size, &v.at(i));
    }
    __test_start();
    for (std::size_t i=0; i < times; ++i)
    {
        for (std::size_t j=0; j < size; ++j)
            dest[j] = rb_find(&v[i], reinterpret_cast<void *>(j), _int_compare);
    }
    __test_end();
    for (std::size_t i=0; i < times; ++i)
        rb_destroy(&v.at(i), _int_destroy);
    USED(v);
    USED(dest);
}

template <class T>
void ctree_lower_bound_test(std::size_t size, std::size_t times)
/*
    container.find(value);
*/
{
    std::vector<T> v(times);
    std::vector<rb_node> dest(size);
    for (std::size_t i=0; i < times; ++i)
    {
        USED_PTR(v.at(i));
        RB_INIT_(v.at(i));
        generate_tree(size, &v.at(i));
    }
    __test_start();
    for (std::size_t i=0; i < times; ++i)
    {
        for (std::size_t j=0; j < size; ++j)
            dest[j] = rb_lower_bound(&v[i],
                reinterpret_cast<void *>(j), _int_compare);
    }
    __test_end();
    for (std::size_t i=0; i < times; ++i)
        rb_destroy(&v.at(i), _int_destroy);
    USED(v);
    USED(dest);
}

template <class T>
void ctree_upper_bound_test(std::size_t size, std::size_t times)
/*
    container.find(value);
*/
{
    std::vector<T> v(times);
    std::vector<rb_node> dest(size);
    for (std::size_t i=0; i < times; ++i)
    {
        USED_PTR(v.at(i));
        RB_INIT_(v.at(i));
        generate_tree(size, &v.at(i));
    }
    __test_start();
    for (std::size_t i=0; i < times; ++i)
    {
        for (std::size_t j=0; j < size; ++j)
            dest[j] = rb_upper_bound(&v[i],
                reinterpret_cast<void *>(j), _int_compare);
    }
    __test_end();
    for (std::size_t i=0; i < times; ++i)
        rb_destroy(&v.at(i), _int_destroy);
    USED(v);
    USED(dest);
}

template <class T>
void ctree_next_iterator_test(std::size_t size, std::size_t times)
/*
    ++iterator;
*/
{
    std::vector<T> v(times);
    for (std::size_t i=0; i < times; ++i)
    {
        USED_PTR(v.at(i));
        RB_INIT_(v.at(i));
        generate_tree(size, &v.at(i));
    }
    __test_start();
    for (std::size_t i=0; i < times; ++i)
    {
        rb_node it = v[i].begin;
        for (std::size_t j=0; j < size; ++j)
            it = rb_next(it);
    }
    __test_end();
    for (std::size_t i=0; i < times; ++i)
        rb_destroy(&v.at(i), _int_destroy);
    USED(v);
}

template <class T>
void ctree_prev_iterator_test(std::size_t size, std::size_t times)
/*
    --iterator;
*/
{
    std::vector<T> v(times);
    for (std::size_t i=0; i < times; ++i)
    {
        USED_PTR(v.at(i));
        RB_INIT_(v.at(i));
        generate_tree(size, &v.at(i));
    }
    __test_start();
    for (std::size_t i=0; i < times; ++i)
    {
        rb_node it = v[i].begin;
        for (std::size_t j=0; j < size; ++j)
            it = rb_next(it);
    }
    __test_end();
    for (std::size_t i=0; i < times; ++i)
        rb_destroy(&v.at(i), _int_destroy);
    USED(v);
}

int main()
{
    double std_time;
    double ft_time;

    tlucanti::Benchmark bm("set");

    bm.next_test("rb_tree constructor test");
    CTREE_SMALL(default_constructor_test);

    CTREE_RUN_TESTS("rb_copy test", copy_constructor_test);

    CTREE_RUN_TESTS("~set() destructor test", destructor_test);

    bm.next_test(".begin() iterator test");
    CTREE_SMALL(begin_test);

    bm.next_test(".end() iterator test");
    CTREE_SMALL(end_test);

    bm.next_test(".size() method test");
    run_ctree_speed_test(size_test, 0, E6);
    bm.small(ft_time, std_time);
    run_ctree_speed_test(size_test, 100, E5);
    bm.medium(ft_time, std_time);

    CTREE_RUN_TESTS(".insert(value) method test", insert_val_test);

    CTREE_RUN_TESTS(".insert(hint, value) method test", insert_iterator_test);

    CTREE_RUN_TESTS(".erase(pos) method test", erase_iterator_test);

    CTREE_RUN_TESTS(".erase(value) method test", erase_val_test);

    CTREE_RUN_TESTS(".find() method test", find_test);

    CTREE_RUN_TESTS(".lower_bound() method test", lower_bound_test);

    CTREE_RUN_TESTS(".upper_bound() method test", upper_bound_test);

    CTREE_RUN_TESTS("++iterator method test", next_iterator_test);

    CTREE_RUN_TESTS("--iterator method test", prev_iterator_test);
}
