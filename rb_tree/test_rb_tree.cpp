
#include "rb_tree.h"
#include "test.hpp"

void insert_tests();
void find_tests();
void remove_tests();
void iterator_test();

void random_insert_find_small_tests();
void random_insert_find_medium_tests();
void random_insert_find_large_tests();

void random_remove_small_tests();
void random_remove_medium_tests();
void random_remove_large_tests();

void random_root_removal_small_test();
void random_root_removal_medium_test();
void random_root_removal_large_test();

int main()
{
    signal(SIGSEGV, sigsegv_catcher);
    signal(SIGILL, sigill_cathcer);
    signal(SIGABRT, sigabrt_catcher);
    srand(static_cast<unsigned int>(time(nullptr)));

    run_test(insert_tests);
    run_test(find_tests);
    run_test(remove_tests);
    run_test(iterator_test);

    run_test(random_insert_find_small_tests);
    run_test(random_insert_find_medium_tests);
    run_test(random_insert_find_large_tests);

    run_test(random_remove_small_tests);
    run_test(random_remove_medium_tests);
    run_test(random_remove_large_tests);

    run_test(random_root_removal_small_test);
    run_test(random_root_removal_medium_test);
    run_test(random_root_removal_large_test);

    final();
}

bool _rb_print_toggle = false;

void insert_tests()
{
    start("rb_insert tests");

    {
        rb_tree tree = {nullptr};
        SINGLE_OP_INIT

        _START:
        SINGLE_OPERATION(rb_insert(&tree, PTR(50), int_compare), _START, 0);
        print_rb_tree(&tree, "5 tree");
        ASSERT(tree.root, "root insert test 0");
        ASSERT(tree.root->key == PTR(50), "root insert test 1");
        ASSERT(tree.root->color == _Rb_Black, "root insert test 2");

        SINGLE_OPERATION(rb_insert(&tree, PTR(40), int_compare), _START, 1);
        _rb_print_toggle = false;
        print_rb_tree(&tree, "4-5 tree");
        CHECK_EDGE(tree.root, left, PTR(50), PTR(40), _Rb_Black, _Rb_Red, "basic insert test 0");

        SINGLE_OPERATION(rb_insert(&tree, PTR(60), int_compare), _START, 2);
        print_rb_tree(&tree, "4-5-6 tree");
        CHECK_EDGE(tree.root, right, 50, 60, _Rb_Black, _Rb_Red, "basic insert test 1");
        _RECOLOR_1:

        SINGLE_OPERATION(rb_insert(&tree, PTR(20), int_compare), _RECOLOR_1, 3);
        print_rb_tree(&tree, "2-4-5-6 tree");
        CHECK_EDGE(tree.root->left, left, 40, 20, _Rb_Black, _Rb_Red, "recolor insert test 0");

        SINGLE_OPERATION(rb_insert(&tree, PTR(70), int_compare), _RECOLOR_1, 4);
        print_rb_tree(&tree, "2-...-7 tree");
        CHECK_EDGE(tree.root->right, right, 60, 70, _Rb_Black, _Rb_Red, "recolor insert test 1");

        _ROTATE_0:
        SINGLE_OPERATION(rb_insert(&tree, PTR(10), int_compare), _ROTATE_0, 5);
        print_rb_tree(&tree, "1-...-7 tree");
        CHECK_EDGE(tree.root->left, left, 20, 10, _B, _R, "right rotate insert test 0");
        CHECK_EDGE(tree.root->left, right, 20, 40, _B, _R, "right rotate insert test 1");

        _ROTATE_1:
        _rb_print_toggle = false;
        SINGLE_OPERATION(rb_insert(&tree, PTR(80), int_compare), _ROTATE_1, 6);
        print_rb_tree(&tree, "2-...-8 tree");
        CHECK_EDGE(tree.root->right, left, 70, 60, _B, _R, "left rotate insert test 0");
        CHECK_EDGE(tree.root->right, right, 70, 80, _B, _R, "left rotate insert test 1");
    }
    {
        rb_tree tree = {nullptr};
        SINGLE_OP_INIT

        _START_1:
        SINGLE_OPERATION(rb_insert(&tree, PTR(1), int_compare), _START_1, 0);
        print_rb_tree(&tree, "1 tree");
        ASSERT(tree.root->key == PTR(1), "root insert test 3");
        ASSERT(tree.root->color == _Rb_Black, "root insert test 4");

        SINGLE_OPERATION(rb_insert(&tree, PTR(2), int_compare), _START_1, 1);
        print_rb_tree(&tree, "1-2 tree");
        CHECK_EDGE(tree.root, right, 1, 2, _B, _R, "root insert test 5");

        _ROOT_ROTATE_LEFT:
        _rb_print_toggle = false;
        SINGLE_OPERATION(rb_insert(&tree, PTR(3), int_compare), _ROOT_ROTATE_LEFT, 2);
        print_rb_tree(&tree, "1-2-3 tree");
        CHECK_EDGE(tree.root, left, 2, 1, _B, _R, "root insert left rotate test 0");
        CHECK_EDGE(tree.root, right, 2, 3, _B, _R, "root insert left rotate test 1");
    }
    {
        rb_tree tree = {nullptr};
        SINGLE_OP_INIT

        _START_2:
        SINGLE_OPERATION(rb_insert(&tree, PTR(3), int_compare), _START_2, 0);
        print_rb_tree(&tree, "1 tree");
        ASSERT(tree.root->key == PTR(3), "root insert test 6");
        ASSERT(tree.root->color == _Rb_Black, "root insert test 7");

        SINGLE_OPERATION(rb_insert(&tree, PTR(2), int_compare), _START_2, 1);
        print_rb_tree(&tree, "1-2 tree");
        CHECK_EDGE(tree.root, left, 3, 2, _B, _R, "root insert test 8");

        _ROOT_ROTATE_RIGHT:
        _rb_print_toggle = false;
        SINGLE_OPERATION(rb_insert(&tree, PTR(1), int_compare), _ROOT_ROTATE_RIGHT, 2);
        print_rb_tree(&tree, "1-2-3 tree");
        CHECK_EDGE(tree.root, left, 2, 1, _B, _R, "root insert right rotate test 0");
        CHECK_EDGE(tree.root, right, 2, 3, _B, _R, "root insert right rotate test 1");
    }
    {
        rb_tree tree = {nullptr};
        SINGLE_OP_INIT

        rb_insert(&tree, PTR(2), int_compare);
        rb_insert(&tree, PTR(1), int_compare);
        rb_insert(&tree, PTR(3), int_compare);
        rb_insert(&tree, PTR(5), int_compare);
        print_rb_tree(&tree, "pre-triangle right tree");
        rb_insert(&tree, PTR(4), int_compare);
        print_rb_tree(&tree, "post-triangle right tree");
        CHECK_EDGE(tree.root->right, left, 4, 3, _B, _R, "triangle insert right test 0");
        CHECK_EDGE(tree.root->right, right, 4, 5, _B, _R, "triangle insert right test 1");
    }
    {
        rb_tree tree = {nullptr};
        SINGLE_OP_INIT

        rb_insert(&tree, PTR(7), int_compare);
        rb_insert(&tree, PTR(6), int_compare);
        rb_insert(&tree, PTR(5), int_compare);
        rb_insert(&tree, PTR(3), int_compare);
        _rb_print_toggle = false;
        print_rb_tree(&tree, "pre-triangle left tree");
        _rb_print_toggle = false;
        rb_insert(&tree, PTR(4), int_compare);
        _rb_print_toggle = false;
        print_rb_tree(&tree, "post-triangle left tree");
        CHECK_EDGE(tree.root->left, left, 4, 3, _B, _R, "triangle insert left test 0");
        CHECK_EDGE(tree.root->left, right, 4, 5, _B, _R, "triangle insert left test 1");
    }
    {
        rb_tree tree = {nullptr};
        // 338 264 813 620 823 116 285 413 738 532 937 973 606 529 676 659 206
        RB_INSERT(338);
        RB_INSERT(264);
        RB_INSERT(813);
        RB_INSERT(620);
        RB_INSERT(823);
        RB_INSERT(116);
        RB_INSERT(285);
        RB_INSERT(413);
        RB_INSERT(738);
        RB_INSERT(532);
        RB_INSERT(937);
        RB_INSERT(973);
        RB_INSERT(606);
        RB_INSERT(529);
        RB_INSERT(676);
        RB_INSERT(659);
        RB_INSERT(206);
        check_valid_rb_tree(&tree);
    }

    result();
}

void find_tests()
{
    start("rb_find tests");

    {
        rb_tree tree = {nullptr};

        ASSERT(rb_get_key(rb_find(&tree, PTR(0), int_compare)) == nullptr, "empty find test 0");
        FIND_ASSERT(tree, 1, "find basic test 0");
        FIND_ASSERT(tree, 0, "find basic test 1");
        FIND_ASSERT(tree, 3, "find basic test 2");
    }
    {
        rb_tree tree = {nullptr};
        FIND_ASSERT(tree, 50, "find recoloring test 0");
        FIND_ASSERT(tree, 40, "find recoloring test 1");
        FIND_ASSERT(tree, 60, "find recoloring test 2");
        FIND_ASSERT(tree, 20, "find recoloring test 3");
        FIND_ASSERT(tree, 70, "find recoloring test 4");
        FIND_ASSERT(tree, 10, "find recoloring test 5");
        FIND_ASSERT(tree, 80, "find recoloring test 6");
    }
    {
        rb_tree tree = {nullptr};
        FIND_ASSERT(tree, 1, "find left-rotate test 0");
        FIND_ASSERT(tree, 2, "find left-rotate test 1");
        FIND_ASSERT(tree, 3, "find left-rotate test 2");
    }
    {
        rb_tree tree = {nullptr};
        FIND_ASSERT(tree, 3, "find right-rotate test 0");
        FIND_ASSERT(tree, 2, "find right-rotate test 1");
        FIND_ASSERT(tree, 1, "find right-rotate test 2");
    }
    {
        rb_tree tree = {nullptr};
        FIND_ASSERT(tree, 2, "find right-triangle-rotate test 0");
        FIND_ASSERT(tree, 1, "find right-triangle-rotate test 1");
        FIND_ASSERT(tree, 3, "find right-triangle-rotate test 2");
        FIND_ASSERT(tree, 5, "find right-triangle-rotate test 3");
        FIND_ASSERT(tree, 4, "find right-triangle-rotate test 4");
    }
    {
        rb_tree tree = {nullptr};
        FIND_ASSERT(tree, 7, "find left-triangle-rotate test 0");
        FIND_ASSERT(tree, 6, "find left-triangle-rotate test 1");
        FIND_ASSERT(tree, 5, "find left-triangle-rotate test 2");
        FIND_ASSERT(tree, 3, "find left-triangle-rotate test 3");
        FIND_ASSERT(tree, 4, "find left-triangle-rotate test 4");
    }

    result();
}

void _random_insert_find_sized_tests(int div, const std::string &name)
{
    start(("random " + name + " insert/find tests").c_str());

    for (int test = 0; test < 1000; ++test)
    {
//        std::cout << test << std::endl;
        std::vector<int> _moves;
        std::set<int> std_tree;
        rb_tree tree = {nullptr};
        try {
            for (int cnt = 0; cnt < 1000; ++cnt)
            {
                int r = rand() % div + 1;
                //            _moves.push_back(r);
                //            std::cout << "adding " << r << std::endl;
                std_tree.insert(r);
                FIND_ASSERT(tree, r, name + " random test");
                compare_trees(&tree, std_tree);
                check_valid_rb_tree(&tree);
            }
        } catch (std::logic_error &e) {
            std::cout << e.what() << std::endl;
            _rb_print_toggle = true;
            print_rb_tree(&tree, "invalid tree");
            std::cout << "moves:";
            for (std::size_t i=0; i < _moves.size(); ++i)
                std::cout << ' ' << _moves[i];
            std::cout << std::endl;
            throw ;
        }
    }

    result();
}

void random_insert_find_small_tests()
{
    _random_insert_find_sized_tests(10, "small");
}

void random_insert_find_medium_tests()
{
    _random_insert_find_sized_tests(1000, "medium");
}

void random_insert_find_large_tests()
{
    _random_insert_find_sized_tests(2147483647, "large");
}

void remove_tests()
{
    start("rb_remove tests");

    {
        rb_tree tree = {nullptr};
        RB_REMOVE(1);
        ASSERT(tree.root == nullptr, "empty remove test 0");
    }
    {
        rb_tree tree = {nullptr};
        RB_INSERT(1);
        RB_REMOVE(1);
        ASSERT(tree.root == nullptr, "root remove test 0");
    }
    {
        rb_tree tree = {nullptr};
        RB_INSERT(2);
        RB_INSERT(1);
        RB_INSERT(3);

        RB_REMOVE(1);
        RB_REMOVE(3);

        RB_NOT_FOUND(1, "red leaf remove test 0");
        RB_NOT_FOUND(1, "red leaf remove test 1");
    }
    {
        rb_tree tree = {nullptr};
        RB_INSERT(5);
        RB_INSERT(2);
        RB_INSERT(7);
        RB_INSERT(3);

        RB_REMOVE(2);

        RB_NOT_FOUND(2, "replacement removal test 0");
    }
    {
        rb_tree tree = {nullptr};
        RB_INSERT(5);
        RB_INSERT(3);
        RB_INSERT(7);
        RB_INSERT(1);
        RB_INSERT(4);

//        RB_PRINT_LOCK("before removal");
        RB_REMOVE(5);
//        RB_PRINT_LOCK("after removal");

        RB_NOT_FOUND(5, "inorder successor removal test 0");
        check_valid_rb_tree(&tree);
    }
    {
        rb_tree tree = {nullptr};
        RB_INSERT(5);
        RB_INSERT(3);
        RB_INSERT(6);
        RB_INSERT(1);
        RB_INSERT(4);
        RB_INSERT(2);

//        RB_PRINT_LOCK("before removal");
        RB_REMOVE(5);
//        RB_PRINT_LOCK("after removal");
        check_valid_rb_tree(&tree);
    }
    {
        rb_tree tree = {nullptr};
        RB_INSERT(5);
        RB_INSERT(2);
        RB_INSERT(9);
        RB_INSERT(1);
        RB_INSERT(4);
        RB_INSERT(8);
        RB_INSERT(10);
        RB_INSERT(0);

        RB_REMOVE(0);
//        RB_PRINT_LOCK("before removal");
        RB_REMOVE(5);
//        RB_PRINT_LOCK("after removal");
        check_valid_rb_tree(&tree);
    }
    {
        rb_tree tree = {nullptr};
        RB_INSERT(3);
        RB_INSERT(1);

//        RB_PRINT_LOCK("before removal");
        RB_REMOVE(3);
//        RB_PRINT_LOCK("after removal");
        check_valid_rb_tree(&tree);
    }
    {
        rb_tree tree = {nullptr};
        RB_INSERT(3);
        RB_INSERT(1);
        RB_INSERT(7);
        RB_INSERT(6);
        RB_INSERT(9);
        RB_INSERT(10);

//        RB_PRINT_LOCK("before removal");
        RB_REMOVE(3);
//        RB_PRINT_LOCK("after removal");
        check_valid_rb_tree(&tree);
    }
    {
        rb_tree tree = {nullptr};
        RB_INSERT(4);
        RB_INSERT(2);
        RB_INSERT(8);
        RB_INSERT(1);
        RB_INSERT(3);
        RB_INSERT(7);
        RB_INSERT(10);
        RB_INSERT(6);
        RB_INSERT(0);

        RB_REMOVE(0);
//        RB_PRINT_LOCK("before removal");
        RB_REMOVE(4);
//        RB_PRINT_LOCK("after removal");
        check_valid_rb_tree(&tree);
    }
    {
        rb_tree tree = {nullptr};
        RB_INSERT(5);
        RB_INSERT(2);
        RB_INSERT(8);
        RB_INSERT(1);
        RB_INSERT(4);
        RB_INSERT(6);
        RB_INSERT(10);
        RB_INSERT(3);
        RB_INSERT(7);
        RB_INSERT(9);

//        RB_PRINT_LOCK("before removal");
        RB_REMOVE(5);
//        RB_PRINT_LOCK("after removal");
        check_valid_rb_tree(&tree);
    }
    {
        rb_tree tree = {nullptr};
        RB_INSERT(9);
        RB_INSERT(4);
        RB_INSERT(10);
        RB_INSERT(7);

//        RB_PRINT_LOCK("before removal");
        RB_REMOVE(10);
//        RB_PRINT_LOCK("after removal");
        check_valid_rb_tree(&tree);
    }

    result();
}

void iterator_test()
{
    start("iterator tests");

    {
        rb_tree tree = {nullptr};

        RB_INSERT(1);
        RB_INSERT(2);
        RB_INSERT(3);
        RB_INSERT(4);
        RB_INSERT(5);
        RB_INSERT(6);
        RB_INSERT(7);
        RB_INSERT(8);
        RB_INSERT(9);
        RB_INSERT(10);

//        RB_PRINT_LOCK("search tree");
        rb_node node = rb_find(&tree, PTR(1), int_compare);
        ASSERT(rb_get_key(node) == PTR(1), "iterator test 0");
        node = rb_next(node);
        ASSERT(rb_get_key(node) == PTR(2), "iterator test 1");
        node = rb_next(node);
        ASSERT(rb_get_key(node) == PTR(3), "iterator test 2");
        node = rb_next(node);
        ASSERT(rb_get_key(node) == PTR(4), "iterator test 3");
        node = rb_next(node);
        ASSERT(rb_get_key(node) == PTR(5), "iterator test 4");
        node = rb_next(node);
        ASSERT(rb_get_key(node) == PTR(6), "iterator test 5");
        node = rb_next(node);
        ASSERT(rb_get_key(node) == PTR(7), "iterator test 6");
        node = rb_next(node);
        ASSERT(rb_get_key(node) == PTR(8), "iterator test 7");
        node = rb_next(node);
        ASSERT(rb_get_key(node) == PTR(9), "iterator test 8");
        node = rb_next(node);
        ASSERT(rb_get_key(node) == PTR(10), "iterator test 9");

        node = rb_prev(node);
        ASSERT(rb_get_key(node) == PTR(9), "iterator test 10");
        node = rb_prev(node);
        ASSERT(rb_get_key(node) == PTR(8), "iterator test 11");
        node = rb_prev(node);
        ASSERT(rb_get_key(node) == PTR(7), "iterator test 12");
        node = rb_prev(node);
        ASSERT(rb_get_key(node) == PTR(6), "iterator test 13");
        node = rb_prev(node);
        ASSERT(rb_get_key(node) == PTR(5), "iterator test 14");
        node = rb_prev(node);
        ASSERT(rb_get_key(node) == PTR(4), "iterator test 15");
        node = rb_prev(node);
        ASSERT(rb_get_key(node) == PTR(3), "iterator test 16");
        node = rb_prev(node);
        ASSERT(rb_get_key(node) == PTR(2), "iterator test 17");
        node = rb_prev(node);
        ASSERT(rb_get_key(node) == PTR(1), "iterator test 18");
    }

    result();
}

void _random_remove_sized_tests(int div, const std::string &name, bool root_removing)
{
    start(("random " + name + " remove tests").c_str());

    for (int test = 0; test < 1000; ++test)
    {
        std::vector<int> insert_moves;
        std::set<int> std_tree;
        rb_tree tree = {nullptr};
        rb_tree starting_tree = {nullptr};
        generate_rb_tree(&tree, insert_moves, 1000, div, &starting_tree);
        std::size_t cnt = 0;
        try {
            for (; cnt < insert_moves.size(); ++cnt)
            {
//                RB_PRINT_LOCK("before removing");
                if (root_removing)
                {
//                    std::cout << "removing " << tree.root->key << std::endl;
                    if (tree.root == nullptr)
                        break ;
                    std::size_t rm = reinterpret_cast<std::size_t>(tree.root->key);
                    RB_REMOVE(rm);
//                    RB_PRINT_LOCK("after removal");
                    std_tree.erase(static_cast<int>(rm));
                    RB_NOT_FOUND(rm, name + " random test");
                }
                else
                {
//                    std::cout << "removing " << insert_moves[cnt] << std::endl;
                    rb_remove(&tree, PTR(insert_moves[cnt]), int_compare);
//                    RB_PRINT_LOCK("after removal");
                    std_tree.erase(insert_moves[cnt]);
                    RB_NOT_FOUND(insert_moves[cnt], name + " random test");
                }
                compare_trees(&tree, std_tree);
                check_valid_rb_tree(&tree);
            }
        } catch (std::exception &e) {
            std::cout << e.what() << std::endl;
            _rb_print_toggle = true;
            print_rb_tree(&starting_tree, "staring tree");
            print_rb_tree(&tree, "invalid tree");
            std::cout << "moves:";
            for (std::size_t i=0; i < insert_moves.size(); ++i)
                std::cout << ' ' << insert_moves[i];
            std::cout << std::endl << "test fall at case " << insert_moves[cnt]
                << "(index " << cnt << ')' << std::endl;
            throw ;
        }
    }

    result();
}

void random_remove_small_tests()
{
    _random_remove_sized_tests(10, "small", false);
}

void random_remove_medium_tests()
{
    _random_remove_sized_tests(1000, "medium", false);
}

void random_remove_large_tests()
{
    _random_remove_sized_tests(2147483647, "large", false);
}

void random_root_removal_small_test()
{
    _random_remove_sized_tests(10, "small", true);
}

void random_root_removal_medium_test()
{
    _random_remove_sized_tests(1000, "medium", true);
}

void random_root_removal_large_test()
{
    _random_remove_sized_tests(2147483647, "large", true);
}
