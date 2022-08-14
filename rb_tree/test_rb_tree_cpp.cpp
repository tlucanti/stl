/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_rb_tree_cpp.cpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tlucanti <tlucanti@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/14 11:26:42 by tlucanti          #+#    #+#             */
/*   Updated: 2022/08/14 17:27:32 by tlucanti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "test.hpp"
# include "rb_tree.hpp"

void basic_method_test();
void basic_comparison_test();

const volatile int null_ = 0;
#define NULLPTR (reinterpret_cast<void *>(null_))

int main()
{
    signal(SIGSEGV, sigsegv_catcher);
    signal(SIGILL, sigill_cathcer);
    signal(SIGABRT, sigabrt_catcher);
    srand(static_cast<unsigned int>(time(nullptr)));

    run_test(basic_method_test);
    run_test(basic_comparison_test);

    final();
}

void basic_method_test()
{
    start("basic method test");

    typedef tlucanti::rb_tree<int>::rb_node int_node;
    tlucanti::rb_tree<int> tree;

    ASSERT(tree.size() == 0, "size test 0");
    RBCPP_INSERT_ASSERT(3, "basic insert test 0");
    ASSERT(tree.size() == 1, "size test 1");
    RBCPP_INSERT_ASSERT(1, "basic insert test 1");
    ASSERT(tree.size() == 2, "size test 2");
    RBCPP_INSERT_ASSERT(5, "basic insert test 2");
    ASSERT(tree.size() == 3, "size test 3");
    RBCPP_INSERT_ASSERT(33, "basic insert test 3");
    ASSERT(tree.size() == 4, "size test 4");
    RBCPP_FIND_ASSERT(33, "basic find test 0");
    ASSERT(tree.size() == 4, "size test 5");
    RBCPP_REMOVE_ASSERT(33, NULLPTR, "basic remove test 0");
    RBCPP_FIND_ASSERT(3, "basic find test 1");
    ASSERT(tree.size() == 3, "size test 6");

    int_node *n3 = tree.find(3);
    RBCPP_NEXT_ASSERT(n3, 5, "basic iterator test 0");
    RBCPP_PREV_ASSERT(n3, 1, "basic iterator test 1");
    RBCPP_LOWERBOUND_ASSERT(3, 3, "basic bound test 0");
    RBCPP_LOWERBOUND_ASSERT(4, 5, "basic bound test 1");
    RBCPP_LOWERBOUND_ASSERT(5, 5, "basic bound test 2");
    RBCPP_LOWERBOUND_ASSERT(6, NULLPTR, "basic bound test 3");
    RBCPP_LOWERBOUND_ASSERT(6, NULLPTR, "basic bound test 4");

    RBCPP_UPPERBOUND_ASSERT(3, 5, "basic bound test 5");
    RBCPP_UPPERBOUND_ASSERT(4, 5, "basic bound test 6");
    RBCPP_UPPERBOUND_ASSERT(5, NULLPTR, "basic bound test 7");
    RBCPP_UPPERBOUND_ASSERT(6, NULLPTR, "basic bound test 8");

    result();
}

void basic_comparison_test()
{
    start("basic comparison test");

    {
        tlucanti::rb_tree<int> tree1;
        tlucanti::rb_tree<int> tree2;

        ASSERT(tree1 == tree2, "compare test 0");
        tree1.insert(1);
        ASSERT(tree1 != tree2, "miss-size compare test 0");
        ASSERT(tree1 > tree2, "miss-size invert compare test 0");
        tree2.insert(1);

        ASSERT(tree1 == tree2, "compare test 2");

        tree1.insert(2);

        ASSERT(tree1 != tree2, "miss-size compare test 0");
        ASSERT(tree1 > tree2, "miss-size compare test 1");
        tree2.insert(3);

        ASSERT(tree1 != tree2, "compare test 0");
        ASSERT(tree1 < tree2, "compare test 1");
    }

    result();
}
