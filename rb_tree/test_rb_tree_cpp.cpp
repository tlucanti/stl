
# include "test.hpp"
# include "rb_tree.hpp"

void basic_method_test();

int main()
{
    signal(SIGSEGV, sigsegv_catcher);
    signal(SIGILL, sigill_cathcer);
    signal(SIGABRT, sigabrt_catcher);
    srand(static_cast<unsigned int>(time(nullptr)));

    run_test(basic_method_test);

    final();
}

void basic_method_test()
{
    start("basic method test");

    typedef tlucanti::rb_tree<int>::rb_node int_node;
    tlucanti::rb_tree<int> tree;

    RBCPP_INSERT_ASSERT(3, "basic insert test 0");
    RBCPP_INSERT_ASSERT(1, "basic insert test 1");
    RBCPP_INSERT_ASSERT(5, "basic insert test 2");
    RBCPP_INSERT_ASSERT(33, "basic insert test 3");
    RBCPP_FIND_ASSERT(33, "basic find test 0");
    RBCPP_REMOVE_ASSERT(33, nullptr, "basic remove test 0");
    RBCPP_FIND_ASSERT(3, "basic find test 1");

    int_node *n3 = tree.find(3);
    RBCPP_NEXT_ASSERT(n3, 5, "basic iterator test 0");
    RBCPP_PREV_ASSERT(n3, 1, "basic iterator test 1");
    RBCPP_LOWERBOUND_ASSERT(3, 3, "basic bound test 0");
    RBCPP_LOWERBOUND_ASSERT(4, 5, "basic bound test 1");
    RBCPP_LOWERBOUND_ASSERT(5, 5, "basic bound test 2");
    RBCPP_LOWERBOUND_ASSERT(6, nullptr, "basic bound test 3");
    RBCPP_LOWERBOUND_ASSERT(6, nullptr, "basic bound test 4");

    RBCPP_UPPERBOUND_ASSERT(3, 5, "basic bound test 5");
    RBCPP_UPPERBOUND_ASSERT(4, 5, "basic bound test 6");
    RBCPP_UPPERBOUND_ASSERT(5, nullptr, "basic bound test 7");
    RBCPP_UPPERBOUND_ASSERT(6, nullptr, "basic bound test 8");

    result();
}