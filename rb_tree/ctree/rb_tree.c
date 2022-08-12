
#include "rb_tree.h"

static _Rb_node     *_Rb_node_create(void *key) WUR NOEXCEPT;
static void         _Rb_node_destroy(_Rb_node *node) NOEXCEPT;
static _Rb_node     *_Rb_grandparent(_Rb_node *node) WUR NOEXCEPT;
static _Rb_node     *_Rb_uncle(_Rb_node *node) WUR NOEXCEPT;
static _Rb_node     *_Rb_rotate_right(_Rb_node *node, _Rb_node **root) NOEXCEPT;
static _Rb_node     *_Rb_rotate_left(_Rb_node *node, _Rb_node **root) NOEXCEPT;
static void         _BST_insert(_Rb_node *root, _Rb_node *node, compare_fun compare) NOEXCEPT;
static _Rb_node     *_BST_find(_Rb_node *root, void *value, compare_fun compare) WUR NOEXCEPT;
static _Rb_node     *_BST_find_parent(_Rb_node *root, void *value, compare_fun compare) WUR NOEXCEPT;
static _Rb_node     *_Rb_recolor(_Rb_node *node) WUR NOEXCEPT;
static _Rb_node     *_Rb_insert(_Rb_node **root, _Rb_node *hint, _Rb_node *node, compare_fun compare, int *was_inserted) WUR NOEXCEPT;
static _Rb_node     *_Rb_insert_case_1(_Rb_node *node, _Rb_node *gp, _Rb_node **root) WUR NOEXCEPT;
static _Rb_node     *_Rb_insert_case_2(_Rb_node *node, _Rb_node *gp, _Rb_node **root) WUR NOEXCEPT;
static void         _Rb_balance_after_insert(_Rb_node **root, _Rb_node *node) NOEXCEPT;
static _Rb_node     *_BST_max(_Rb_node *node) WUR NOEXCEPT;
static _Rb_node     *_BST_min(_Rb_node *node) WUR NOEXCEPT;
static _Rb_node     *_BST_remove(_Rb_node *node, del_fun del) WUR NOEXCEPT;
static int          _Rb_is_black_children(_Rb_node *node) WUR NOEXCEPT;
static void         _Rb_fix_double_black(_Rb_node *node, _Rb_node **root) NOEXCEPT;
static void         _Rb_fix_double_black_left(_Rb_node *node, _Rb_node **root) NOEXCEPT;
static void         _Rb_fix_double_black_right(_Rb_node *node, _Rb_node **root) NOEXCEPT;
static void         _Rb_remove_node(_Rb_node **root, _Rb_node *rm, del_fun del) NOEXCEPT;
static _Rb_node     *_Rb_remove(_Rb_node **root, void *value, compare_fun compare, del_fun del, int *was_removed) WUR NOEXCEPT;
static _Rb_node     *_BST_next(_Rb_node *node) WUR NOEXCEPT;
static _Rb_node     *_BST_prev(_Rb_node *node) WUR NOEXCEPT;
static _Rb_node     *_Rb_tree_copy(_Rb_node *root, copy_fun) WUR NOEXCEPT;
static void         _Rb_tree_destroy(_Rb_node *root, del_fun) NOEXCEPT;
static _Rb_node     *_BST_lower_bound(_Rb_node *root, void *value, compare_fun compare) WUR NOEXCEPT;
static _Rb_node     *_BST_upper_bound(_Rb_node *root, void *value, compare_fun compare) WUR NOEXCEPT;
static int          _DFS_compare(_Rb_node *rt1, _Rb_node *rt2, compare_fun compare) WUR NOEXCEPT;
static int          _BST_compare(_Rb_node *rt1, _Rb_node *rt2, compare_fun compare, _Rb_node *end1, _Rb_node *end2) WUR NOEXCEPT;

#ifdef __cplusplus
static _Rb_node   *_Rb_node_create(void *key)
{
    _Rb_node *ret = new _Rb_node;
    ret->parent = NULL;
    ret->left = NULL;
    ret->right = NULL;
    ret->color = _Rb_Red;
    ret->key = key;
    return ret;
}

static void         _Rb_node_destroy(_Rb_node *node)
{
    delete node;
}

#else /* C */

static _Rb_node     *_Rb_node_create(void *key)
{
    _Rb_node    *ret;

    ret = (_Rb_node *)malloc(sizeof(_Rb_node));
    ret->parent = NULL;
    ret->left = NULL;
    ret->right = NULL;
    ret->color = _Rb_Red;
    ret->key = key;
    return ret;
}

static void         _Rb_node_destroy(_Rb_node *node)
{
    free(node);
}
#endif

void    *rb_get_key(rb_node node)
{
    if (LIKELY(node.node != NULL))
        return node.node->key;
    return NULL;
}

rb_node rb_insert_hint(rb_tree *root, rb_node hint, void *key, compare_fun compare, int *was_inserted)
{
    int _was_inserted_dummy;

    if (was_inserted == NULL)
        was_inserted = &_was_inserted_dummy;
    *was_inserted = 1;
    _Rb_node *node = _Rb_node_create(key);
    if (UNLIKELY(root->root.node == NULL))
    {
        root->begin.node = node;
        root->end.node = node;
        root->root.node = node;
        node->color = _Rb_Black;
        ++root->size;
        return root->root;
    }
    else if (compare(key, root->begin.node->key) <= -1)
        root->begin.node = node;
    else if (compare(key, root->end.node->key) >= 1)
        root->end.node = node;
    _Rb_node *ret = _Rb_insert(&root->root.node, hint.node, node, compare, was_inserted);
    if (*was_inserted)
        ++root->size;
    return (rb_node){ret};
}

rb_node rb_insert(rb_tree *root, void *key, compare_fun compare, int *was_inserted)
{
    return rb_insert_hint(root, root->root, key, compare, was_inserted);
}

rb_node rb_find(rb_tree *root, void *key, compare_fun compare)
{
    return (rb_node){_BST_find(root->root.node, key, compare)};
}

rb_node rb_remove(rb_tree *root, void *key, compare_fun compare, del_fun del)
{
    int was_removed;

    if (UNLIKELY(root->root.node == NULL))
        return (rb_node){NULL};
    _Rb_node    *ret = _Rb_remove(&root->root.node, key, compare, del, &was_removed);
    if (was_removed)
        --root->size;
    if (UNLIKELY(root->root.node == NULL))
    {
        root->begin.node = NULL;
        root->end.node = NULL;
    }
    else
    {
        root->begin.node = _BST_min(root->root.node);
        root->end.node = _BST_max(root->root.node);
    }
    return (rb_node){ret};
}

rb_node rb_remove_node(rb_tree *root, rb_node node, del_fun del)
{
    --root->size;
    _Rb_node *ret;
    if (root->end.node == node.node)
        ret = NULL;
    else
        ret = _BST_next(node.node);
    _Rb_remove_node(&root->root.node, node.node, del);
    if (UNLIKELY(root->root.node == NULL))
    {
        root->begin.node = NULL;
        root->end.node = NULL;
    }
    else
    {
        root->begin.node = _BST_min(root->root.node);
        root->end.node = _BST_max(root->root.node);
    }
    if (ret == NULL)
        return root->end;
    return (rb_node){ret};
}

rb_node rb_next(rb_node node)
{
    return (rb_node){_BST_next(node.node)};
}

rb_node rb_prev(rb_node node)
{
    return (rb_node){_BST_prev(node.node)};
}

rb_tree rb_copy(rb_tree *root, copy_fun copy)
{
    rb_tree tree = {NULL, NULL, NULL, 0};
    tree.root.node = _Rb_tree_copy(root->root.node, copy);
    tree.begin.node = _BST_min(tree.root.node);
    tree.end.node = _BST_max(tree.root.node);
    tree.size = root->size;
    return tree;
}

void    rb_destroy(rb_tree *root, del_fun del)
{
    if (UNLIKELY(root->root.node == NULL))
        return ;
    _Rb_tree_destroy(root->root.node, del);
    root->root.node = NULL;
    root->begin.node = NULL;
    root->end.node = NULL;
    root->size = 0;
}

rb_node rb_lower_bound(rb_tree *root, void *value, compare_fun cmp)
{
    if (root->root.node == NULL)
        return (rb_node){NULL};
    return (rb_node){_BST_lower_bound(root->root.node, value, cmp)};
}

rb_node rb_upper_bound(rb_tree *root, void *value, compare_fun cmp)
{
    if (root->root.node == NULL)
        return (rb_node){NULL};
    return (rb_node){_BST_upper_bound(root->root.node, value, cmp)};
}

int     rb_equal(rb_tree *tree1, rb_tree *tree2, compare_fun cmp)
{
    if (tree1->size != tree2->size)
        return 0;
    else if (UNLIKELY(tree1->size == 0 && tree2->size == 0))
        return 1;
    return _DFS_compare(tree1->root.node, tree2->root.node, cmp);
}

int     rb_compare(rb_tree *tree1, rb_tree *tree2, compare_fun cmp)
{
    if (tree1->size != tree2->size)
    {
        ptrdiff_t diff = static_cast(ptrdiff_t, tree1->size - tree2->size);
        return (0 < diff) - (diff < 0);
    }
    else if (UNLIKELY(tree1->size == 0 && tree2->size == 0))
        return 0;
    return _BST_compare(tree1->root.node, tree2->root.node, cmp, tree1->end.node, tree2->end.node);
}

static _Rb_node     *_Rb_grandparent(_Rb_node *node)
/*
    function returns grandparent of node

    (grandparent)
             \
           (parent)
            /    \
           *    (node)

*/
{
    assert(node);

    return node->parent->parent;
}

UNUSED
static _Rb_node     *_Rb_uncle(_Rb_node *node)
/*
    function returns uncle of node
    there are two cases of uncles:


      (grandparent)
        /      \
    (uncle)   (parent)
               /    \
              *    (node)


      (grandparent)
        /       \
    (parent)   (uncle)
     /    \
    *    (node)

*/
{
    _Rb_node   *gp;

    assert(node);
    gp = _Rb_grandparent(node);
    if (node->parent == gp->left)
        return gp->left;
    return gp->left;
}

static _Rb_node     *_Rb_rotate_right(_Rb_node *node, _Rb_node **root)
/*
    function applies right-rotation to node
    right rotation has 2 steps
     - node's left child becomes node's parent, and node becomes right child of
        it's left child
     - right sub-tree of node's left child becomes node's left sub-tree

    in this example - node (B) had been right-rotated
    and there is two steps:
     - (B)'s left child (A) becomes (B)'s parent and (B) becomes (A)'s right
        child
     - right sub-tree [W] of left node (A) of node (B) becomes left sub-tree of
        node (B)


            /                 /
          (B)               (A)
         /   \             /   \
       (A)   [E]   -->   [Q]   (B)
      /   \                   /   \  
    [Q]   [W]               [W]   [E]

*/
{
    _Rb_node    *pivot;

    assert(node);
    assert(root);
    pivot = node->left;
    if (node == *root)
        *root = pivot;
    if (node->parent)
    {
        if (node == node->parent->left)
            node->parent->left = pivot;
        else
            node->parent->right = pivot;
    }
    pivot->parent = node->parent; // step 1
    node->left = pivot->right; // step 2
    if (node->left)
        node->left->parent = node; // step 3
    pivot->right = node; // step 4
    node->parent = pivot; // step 5
    return node;
}

static _Rb_node     *_Rb_rotate_left(_Rb_node *node, _Rb_node **root)
/*
    functions applies left-rotation to node
    left rotation has 2 steps:
     - node's right child becomes node's parent, and node becomes left child of
        it's right child
     - left sub-tree of node's right child becomes node's right sub-tree

    in this example - node (B) had been left-rotated
    and there is two steps:
     - (B)'s right child (A) becomes (B)'s parent and (B) becomes (A)'s left
        child
     - left sub-tree [W] of right node (A) of node (B) becomes right sub-tree of
        node (B)

         /                       /
       (B)                     (A)
      /   \                   /   \
    [Q]   (A)      -->      (B)   [E]
         /   \             /   \
       [W]   [E]         [Q]   [W]
*/
{
    _Rb_node    *pivot;

    assert(node);
    assert(root);
    pivot = node->right;
    if (node == *root)
        *root = pivot;
    if (node->parent)
    {
        if (node == node->parent->left)
            node->parent->left = pivot;
        else
            node->parent->right = pivot;
    }
    pivot->parent = node->parent;
    node->right = pivot->left;

    if (node->right)
        node->right->parent = node;
    pivot->left = node;

    node->parent = pivot;
    return node;
}

static void     _BST_insert(_Rb_node *root, _Rb_node *node, compare_fun compare)
/*
    function inserts element `node` to Binary Search Tree with root `root`
*/
{
    assert(root);
    assert(node);
    assert(compare);
    while (1)
    {
        if (compare(node->key, root->key) < 0)
        {
            if (root->left == NULL)
            {
                root->left = node;
                node->parent = root;
                return ;
            }
            root = root->left;
        }
        else
        {
            if (root->right == NULL)
            {
                root->right = node;
                node->parent = root;
                return ;
            }
            root = root->right;
        }
    }
}

static _Rb_node     *_BST_find(_Rb_node *root, void *value, compare_fun compare)
{
    assert(compare);

    if (root == NULL)
        return NULL;
    while (1)
    {
        int cmp = compare(value, root->key);
        if (cmp == 0)
            return root;
        else if (cmp < 0)
        {
            if (root->left == NULL)
                return NULL;
            root = root->left;
        }
        else
        {
            if (root->right == NULL)
                return NULL;
            root = root->right;
        }
    }
}

static _Rb_node     *_BST_find_parent(_Rb_node *root, void *value, compare_fun compare)
{
    assert(compare);

    if (root == NULL)
        return NULL;
    while (1)
    {
        int cmp = compare(value, root->key);
        if (cmp == 0)
            return root;
        else if (cmp < 0)
        {
            if (root->left == NULL)
                return root;
            root = root->left;
        }
        else
        {
            if (root->right == NULL)
                return root;
            root = root->right;
        }
    }
}

static _Rb_node     *_Rb_recolor(_Rb_node *node)
/*
    function does recoloring to serval neighbours:
     - uncle is colored Black
     - parent is colored Black
     - grandparent is colored Red

    in this example [a] means black node, and (a) - is red, {a} means any color

             /                               /
       {grandparent}                   (grandparent)
          /     \                         /     \
    {uncle}     {parent}      -->   [uncle]     [parent]
                 /    \                          /    \
                *    {node}                     *    (node)
*/
{
    _Rb_node    *gp;

    assert(node);
    gp = _Rb_grandparent(node);
    gp->color = _Rb_Red;
    gp->left->color = _Rb_Black;
    gp->right->color = _Rb_Black;
    return gp;
}

static _Rb_node     *_Rb_insert(_Rb_node **root, _Rb_node *hint, _Rb_node *node, compare_fun compare, int *was_inserted)
{
    assert(root);
    assert(hint);
    assert(compare);
    assert(was_inserted);

    _Rb_node *parent = _BST_find_parent(hint, node->key, compare);
    if (parent != NULL && compare(node->key, parent->key) == 0)
    {
        _Rb_node_destroy(node);
        *was_inserted = 0;
        return parent;
    }
    else if (parent == NULL)
        _BST_insert(*root, node, compare);
    else
        _BST_insert(parent, node, compare);
    _Rb_balance_after_insert(root, node);
    return node;
}

static _Rb_node     *_Rb_insert_case_1(_Rb_node *node, _Rb_node *gp, _Rb_node **root)
/*
    function eliminates violations after inserting node to tree
    in examples [a] means black node, (a) - is red, {a} means, that color does
    not mater. {*} means, that this sub-tree has same black-height

    case one means that we can do recoloring and move violation to the upper
    level (to grandparent and grand-grandparent), until we hit the root of the
    tree case one can be applied, only if uncle and parent has same (red) color

             /                               /
       [grandparent]                   (grandparent)
          /     \                        /       \
     (uncle)   (parent)      -->    [uncle]     [parent]
      /   \     /    \               /   \     /    \
    {*}   {*} {*}   (node)         {*}   {*} {*}   (node)
    
    case two means that we cannot do recoloring due to different colors of
    (uncle) and (parent), because otherwise we will violate `black-height` rule,
    so we need to do rotation to balance _Rb_Red nodes between _Rb_Black ones, but
    before this we need to straighten the node-parent-grandparent line in order
    to not break `black-height` rule in next rotation

    

*/
{
    assert(node);
    assert(gp);
    assert(root);
    if (gp->right && gp->right->color == _Rb_Red)
        return _Rb_recolor(node); // case 1
    else
    {
        if (node == node->parent->right)
            node = _Rb_rotate_left(node->parent, root); // case 2
        _Rb_rotate_right(gp, root)->color = _Rb_Red; // case 3
        node->parent->color = _Rb_Black;
        if (gp == *root)
            *root = node->parent;
        return node->parent;
    }
}

static _Rb_node     *_Rb_insert_case_2(_Rb_node *node, _Rb_node *gp, _Rb_node **root)
{
    assert(node);
    assert(gp);
    assert(root);
    if (gp->left && gp->left->color == _Rb_Red)
        return _Rb_recolor(node); // case 1
    else
    {
        if (node == node->parent->left)
            node = _Rb_rotate_right(node->parent, root); // case 2
        _Rb_rotate_left(gp, root)->color = _Rb_Red; // case 3
        node->parent->color = _Rb_Black;
        if (gp == *root)
            *root = node->parent;
        return node->parent;
    }
}

static void         _Rb_balance_after_insert(_Rb_node **root, _Rb_node *node)
{
    while (node != *root && node->color == _Rb_Red)
    {
        if (node->parent->color == _Rb_Black)
            break ;
        _Rb_node *gp = _Rb_grandparent(node);
        if (gp == NULL)
            break ;
        if (node->parent == gp->left) // left cases
            node = _Rb_insert_case_1(node, gp, root);
        else // right cases
            node = _Rb_insert_case_2(node, gp, root);
    }
    (*root)->color = _Rb_Black;
}

static _Rb_node     *_BST_max(_Rb_node *node)
/*
    function finds the maximal element in given tree
*/
{
    assert(node);
    while (node->right != NULL)
        node = node->right;
    return node;
}

static _Rb_node     *_BST_min(_Rb_node *node)
{
    assert(node);
    while (node->left != NULL)
        node = node->left;
    return node;
}

static _Rb_node     *_BST_remove(_Rb_node *node, del_fun del)
/*
    function performs binary search tree removing algorithm. It has three cases:

     - case 1:
        back to _Rb_remove function to fix double black problem (if node is
        black) or simply delete node if it is red

     - case 2:
        if node have only one child - do node replacement and finish deletion
        color root of nodes's subtree (nodes only child) is taken from node's
        color. After this case no fixes need in r/b tree cause no r/b tree rules
        is violated

           (parent)           (parent)
            /   \              /   \
        (node)  NULL  -->     (*)  NULL
           \
           [*]

     - case 3:
        in case thee deleting node has both non-null child, so we need to find
        indorder successor of node (the maximal element in left sub-tree of
        node), replace node with it (but keep node's color), and than delete
        inorder successor (it can be done simply by recursive call deleting
        function)

        in current example (D) is node's inorder successor

              (node)
              /   \
            (A)   (B)
           /   \
         (C)   (D) <-- node's inorder successor
*/
{
    assert(node);

    if (node->left == NULL && node->right == NULL)
    // case 1
        return node;
    if (node->left == NULL || node->right == NULL)
    // case 2
    {
        _Rb_node *child;
        if (node->right == NULL)
            child = node->left;
        else
            child = node->right;
        if (node->parent == NULL)
            return node;
        else if (node->parent->left == node)
            node->parent->left = child;
        else
            node->parent->right = child;
        child->parent = node->parent;
        child->color = node->color;
        del(node->key);
        _Rb_node_destroy(node);
        return NULL;
    }
    // case 3
    _Rb_node *accessor = _BST_max(node->left);
    node->key = accessor->key;
    return _BST_remove(accessor, del);
}

static int          _Rb_is_black_children(_Rb_node *node)
{
    return (node->left == NULL || node->left->color == _Rb_Black)
        && (node->right == NULL || node->right->color == _Rb_Black);
}

static void         _Rb_fix_double_black(_Rb_node *node, _Rb_node **root)
{
    assert(node);
    assert(root);
    assert(*root);
    if (node == *root)
        return ;
    if (node->parent->left == node)
        _Rb_fix_double_black_left(node, root);
    else
        _Rb_fix_double_black_right(node, root);
}

static void         _Rb_fix_double_black_left(_Rb_node *node, _Rb_node **root)
{
    assert(node);
    assert(root);
    assert(*root);
    if (node == *root)
    // case 2
        return ;
    _Rb_node *parent = node->parent;
    _Rb_node *sibling = parent->right;
    if (sibling->color == _Rb_Black && _Rb_is_black_children(sibling))
    // case 3
    {
        sibling->color = _Rb_Red;
        if (parent->color == _Rb_Red)
            parent->color = _Rb_Black;
        else
            _Rb_fix_double_black(parent, root);
        return ;
    }
    else if (sibling->color == _Rb_Red)
    // case 4
    {
        sibling->color = parent->color;
        parent->color = _Rb_Red;
        _Rb_rotate_left(parent, root);
        _Rb_fix_double_black_left(node, root);
        return ;
    }
    else if (sibling->right == NULL || sibling->right->color == _Rb_Black)
    // case 5
    {
        _Rb_colors sib_col = sibling->color;
        sibling->color = sibling->left->color;
        sibling->left->color = sib_col;
        _Rb_rotate_right(sibling, root);
        sibling = sibling->parent;
    }
    // case 6
    _Rb_colors sib_col = sibling->color;
    sibling->color = parent->color;
    parent->color = sib_col;
    if (sibling->right)
        sibling->right->color = _Rb_Black;
    _Rb_rotate_left(parent, root);
}

static void         _Rb_fix_double_black_right(_Rb_node *node, _Rb_node **root)
{
    assert(node);
    assert(root);
    assert(*root);
    if (node == *root)
        // case 2
        return ;
    _Rb_node *parent = node->parent;
    _Rb_node *sibling = parent->left;
    if (sibling->color == _Rb_Black && _Rb_is_black_children(sibling))
        // case 3
    {
        sibling->color = _Rb_Red;
        if (parent->color == _Rb_Red)
            parent->color = _Rb_Black;
        else
            _Rb_fix_double_black(parent, root);
        return ;
    }
    else if (sibling->color == _Rb_Red)
        // case 4
    {
        sibling->color = parent->color;
        parent->color = _Rb_Red;
        _Rb_rotate_right(parent, root);
        _Rb_fix_double_black_right(node, root);
        return ;
    }
    else if (sibling->left == NULL || sibling->left->color == _Rb_Black)
        // case 5
    {
        _Rb_colors sib_col = sibling->color;
        sibling->color = sibling->right->color;
        sibling->right->color = sib_col;
        _Rb_rotate_left(sibling, root);
        sibling = sibling->parent;
    }
    // case 6
    _Rb_colors sib_col = sibling->color;
    sibling->color = parent->color;
    parent->color = sib_col;
    if (sibling->left)
        sibling->left->color = _Rb_Black;
    _Rb_rotate_right(parent, root);
}

void _Rb_remove_node(_Rb_node **root, _Rb_node *rm, del_fun del)
{
    assert(root);
    assert(rm);
    assert(del);

    if (*root == NULL)
        return ;
    del(rm->key);
    _Rb_node *leaf = _BST_remove(rm, del);
    if (leaf == NULL)
        return ;
    if (leaf == *root)
    {
        if (leaf->left)
            *root = leaf->left;
        else if (leaf->right)
            *root = leaf->right;
        else
        {
            _Rb_node_destroy(*root);
            *root = NULL;
            return ;
        }
        _Rb_node_destroy(leaf);
        (*root)->parent = NULL;
        (*root)->color = _Rb_Black;
        return ;
    }
    if (leaf->color != _Rb_Red)
        _Rb_fix_double_black(leaf, root);
    if (leaf->parent->left == leaf)
        leaf->parent->left = NULL;
    else
        leaf->parent->right = NULL;
    _Rb_node_destroy(leaf);
}

static _Rb_node     *_Rb_remove(_Rb_node **root, void *value, compare_fun compare, del_fun del, int *was_removed)
{
    assert(root);
    assert(compare);
    assert(was_removed);

    *was_removed = 0;
    if (*root == NULL)
        return NULL;
    _Rb_node *rm = _BST_find(*root, value, compare);
    if (rm == NULL)
        return NULL;
    *was_removed = 1;
    _Rb_node *ret = _BST_next(rm);
    _Rb_remove_node(root, rm, del);
    return ret;
}

static _Rb_node    *_BST_next(_Rb_node *node)
{
    assert(node);
    if (node->right)
        return _BST_min(node->right);
    while (node->parent && node->parent->right == node)
        node = node->parent;
    if (LIKELY(node->parent != NULL))
        return node->parent;
    return NULL;
}

static _Rb_node    *_BST_prev(_Rb_node *node)
{
    assert(node);
    if (node->left)
        return _BST_max(node->left);
    while (node->parent && node->parent->left == node)
        node = node->parent;
    if (LIKELY(node->parent != NULL))
        return node->parent;
    return NULL;
}

static _Rb_node     *_Rb_tree_copy(_Rb_node *root, copy_fun copy)
{
    assert(root);
    assert(copy);

    _Rb_node *new_root = _Rb_node_create(copy(root->key));
    new_root->color = root->color;
    if (root->left)
        new_root->left = _Rb_tree_copy(root->left, copy);
    if (root->right)
        new_root->right = _Rb_tree_copy(root->right, copy);
    return new_root;
}

static void         _Rb_tree_destroy(_Rb_node *root, del_fun del)
{
    assert(root);
    assert(del);

    del(root->key);
    if (root->left)
        _Rb_tree_destroy(root->left, del);
    if (root->right)
        _Rb_tree_destroy(root->right, del);
    _Rb_node_destroy(root);
}

static _Rb_node     *_BST_lower_bound(_Rb_node *root, void *value, compare_fun compare)
{
    assert(root);
    _Rb_node *ret = root;
    while (root != NULL)
    {
        if (compare(root->key, value) >= 0)
        {
            ret = root;
            root = root->left;
        }
        else
            root = root->right;
    }
    if (compare(ret->key, value) < 0)
        return NULL;
    return ret;
}

static _Rb_node     *_BST_upper_bound(_Rb_node *root, void *value, compare_fun compare)
{
    assert(root);
    _Rb_node *ret = root;
    while (root != NULL)
    {
        if (compare(root->key, value) > 0)
        {
            ret = root;
            root = root->left;
        }
        else
            root = root->right;
    }
    if (compare(ret->key, value) <= 0)
        return NULL;
    return ret;
}

static int _DFS_compare(_Rb_node *rt1, _Rb_node *rt2, compare_fun compare)
{
    assert(rt1);
    assert(rt2);
    assert(compare);

    int left = 1;
    int right = 1;

    if (compare(rt1->key, rt2->key) != 0)
        return 0;
    if (rt1->left && rt2->left)
        left = _DFS_compare(rt1->left, rt2->left, compare);
    else if (rt1->left || rt2->left)
        return 0;
    if (rt1->right && rt2->right)
        right = _DFS_compare(rt1->right, rt2->right, compare);
    else if (rt1->right || rt2->right)
        return 0;
    return left & right;
}

static int          _BST_compare(_Rb_node *rt1, _Rb_node *rt2, compare_fun compare, _Rb_node *end1, _Rb_node *end2)
{
    while (1)
    {
        int cmp = compare(rt1->key, rt2->key);
        if (cmp)
            return cmp;
        if (rt1 == end1 || rt2 == end2)
        {
            if (rt1 == end1 && rt2 == end2)
                return 0;
            if (rt2 == end2)
                return 1;
            return -1;
        }
        rt1 = _BST_next(rt1);
        rt2 = _BST_next(rt2);
    }
}