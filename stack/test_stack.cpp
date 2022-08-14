/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_stack.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tlucanti <tlucanti@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/14 11:26:42 by tlucanti          #+#    #+#             */
/*   Updated: 2022/08/14 20:11:52 by tlucanti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "test.hpp"
#include "stack.hpp"
#include <vector>
#include <set>
#include <stack>

void stack_tests();

int main()
{
    run_test(stack_tests);

    final();
}

void stack_tests()
{
    start("stack tests");

    {
        ft::stack<int, std::vector<int> > a;
        ft::stack<int, std::set<int> > b;

        std::stack<int, std::vector<int> > stda;
        std::stack<int, std::set<int> > stdb;

        ASSERT(a.empty(), "empty test 0");
        ASSERT(b.empty(), "empty test 1");
        ASSERT(stda.empty(), "empty test 2");
        ASSERT(stdb.empty(), "empty test 3");

        ASSERT(a.size() == 0, "size test 0");
        ASSERT(b.size() == 0, "size test 1");
        ASSERT(stda.size() == 0, "size test 2");
        ASSERT(stdb.size() == 0, "size test 3");

        MAKE_USED(a);
        MAKE_USED(b);
        MAKE_USED(stda);
        MAKE_USED(stdb);
    }
    {
        std::vector<int> v;
        std::set<int> s;

        ft::stack<int, std::vector<int> > a(v);
        ft::stack<int, std::set<int> > b(s);

        std::stack<int, std::vector<int> > stda(v);
        std::stack<int, std::set<int> > stdb(s);

        ft::stack<int, std::vector<int> > aa(a);
        ft::stack<int, std::set<int> > bb(b);

        std::stack<int, std::vector<int> > stdaa(stda);
        std::stack<int, std::set<int> > stdbb(stdb);

        a = aa;
        b = bb;

        stda = stdaa;
        stdb = stdbb;

        MAKE_USED(a);
        MAKE_USED(b);
        MAKE_USED(stda);
        MAKE_USED(stdb);
    }
    {
        std_vec_123(v);
        std::set<int> s;
        s.insert(3);
        s.insert(2);
        s.insert(1);

        ft::stack<int, std::vector<int> > a(v);
        ft::stack<int, std::set<int> > b(s);

        std::stack<int, std::vector<int> > stda(v);
        std::stack<int, std::set<int> > stdb(s);

        ASSERT(a.top() == 3, "top test 0");
        ASSERT(stda.top() == 3, "top test 1");

        ASSERT(not a.empty(), "empty test 4");
        ASSERT(not b.empty(), "empty test 5");
        ASSERT(not stda.empty(), "empty test 6");
        ASSERT(not stdb.empty(), "empty test 7");

        ASSERT(a.size() == 3, "size test 4");
        ASSERT(b.size() == 3, "size test 5");
        ASSERT(stda.size() == 3, "size test 6");
        ASSERT(stdb.size() == 3, "size test 7");
    }
    {
        ft::stack<int, std::vector<int> > a;
        std::stack<int, std::vector<int> > stda;

        a.push(1);
        stda.push(1);
        ASSERT(a.size() == 1, "push test 0");
        ASSERT(stda.size() == 1, "push test 1");
        a.pop();
        stda.pop();
        ASSERT(a.empty(), "pop test 0");
        ASSERT(stda.empty(), "pop test 1");
    }
    {
        typedef std::vector<int> T;

        std_vec_123(v);
        ft::stack<int, T> a(v);
        ft::stack<int, T> b(v);

        ASSERT(not (a > b), "lexical compare test 0");
        ASSERT(not (a < b), "lexical compare test 1");
        ASSERT(a >= b, "lexical compare test 2");
        ASSERT(a <= b, "lexical compare test 3");
        ASSERT(a == b, "lexical compare test 4");
        ASSERT(not (a != b), "lexical compare test 5");
    }

    result();
}
