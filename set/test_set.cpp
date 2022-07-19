
#include "rb_tree.h"
#include "test.hpp"

void insert_tests();
void find_tests();

int main()
{
//    signal(SIGSEGV, sigsegv_catcher);
//    signal(SIGILL, sigill_cathcer);
//    signal(SIGABRT, sigabrt_catcher);

    run_test(insert_tests);
    run_test(find_tests);

    final();
}

int int_compare(void *lhs, void *rhs)
{
    return static_cast<int>(
        reinterpret_cast<std::ptrdiff_t>(lhs)
        - reinterpret_cast<std::ptrdiff_t>(rhs)
    );
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

    result();
}

void find_tests()
{
    start("rb_find tests");

    {
        rb_tree tree = {nullptr};

        ASSERT(rb_find(&tree, PTR(0), int_compare) == nullptr, "empty find test 0");
        rb_insert(&tree, PTR(1), int_compare);
        ASSERT(rb_find(&tree, PTR(1), int_compare) == PTR(1), "find basic test 0");
        rb_insert(&tree, PTR(0), int_compare);
    }

    result();
}