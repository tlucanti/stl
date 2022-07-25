
#include "rb_tree.h"

WUR static _Rb_node     *_Rb_node_create(void *key) NOEXCEPT;
static void             _Rb_node_destroy(_Rb_node *node) NOEXCEPT;
WUR static _Rb_node     *_Rb_grandparent(_Rb_node *node) NOEXCEPT;
WUR static _Rb_node     *_Rb_uncle(_Rb_node *node) NOEXCEPT;
static _Rb_node         *_Rb_rotate_right(_Rb_node *node, _Rb_node **root) NOEXCEPT;
static _Rb_node         *_Rb_rotate_left(_Rb_node *node, _Rb_node **root) NOEXCEPT;
WUR static _Rb_node     *_BST_insert(_Rb_node *root, _Rb_node *node, compare_fun compare) NOEXCEPT;
WUR static _Rb_node     *_BST_find(_Rb_node *root, void *value, compare_fun compare) NOEXCEPT;
WUR static _Rb_node     *_Rb_recolor(_Rb_node *node) NOEXCEPT;
WUR static _Rb_node     *_Rb_insert_case_1(_Rb_node *node, _Rb_node *gp, _Rb_node **root) NOEXCEPT;
WUR static _Rb_node     *_Rb_insert_case_2(_Rb_node *node, _Rb_node *gp, _Rb_node **root) NOEXCEPT;
WUR static _Rb_node     *_Rb_insert(_Rb_node **root, _Rb_node *node, compare_fun compare) NOEXCEPT;
WUR static _Rb_node     *_BST_max(_Rb_node *node) NOEXCEPT;
WUR static _Rb_node     *_BST_min(_Rb_node *node) NOEXCEPT;
WUR static _Rb_node     *_BST_remove(_Rb_node *node) NOEXCEPT;
WUR static int          _Rb_is_black_children(_Rb_node *node) NOEXCEPT;
static void             _Rb_fix_double_black(_Rb_node *node, _Rb_node **root) NOEXCEPT;
static void             _Rb_fix_double_black_left(_Rb_node *node, _Rb_node **root) NOEXCEPT;
static void             _Rb_fix_double_black_right(_Rb_node *node, _Rb_node **root) NOEXCEPT;
WUR static _Rb_node     *_Rb_remove(_Rb_node **root, void *value, compare_fun compare) NOEXCEPT;
WUR static _Rb_node     *_Rb_next(_Rb_node *node) NOEXCEPT;
WUR static _Rb_node     *_Rb_prev(_Rb_node *node) NOEXCEPT;
WUR static _Rb_node     *_Rb_tree_copy(_Rb_node *root, copy_fun) NOEXCEPT;
static void             _Rb_tree_destroy(_Rb_node *root, del_fun) NOEXCEPT;

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

rb_node rb_insert(rb_tree *root, void *key, compare_fun compare)
{
    _Rb_node *node = _Rb_node_create(key);
    if (UNLIKELY(root->root.node == NULL))
    {
        root->begin.node = node;
        root->end.node = node;
    }
    else if (compare(key, root->begin.node->key) <= -1)
        root->begin.node = node;
    else if (compare(key, root->end.node->key) >= 1)
        root->end.node = node;
    return (rb_node){_Rb_insert(&root->root.node, node, compare)};
}

rb_node rb_find(rb_tree *root, void *key, compare_fun compare)
{
    return (rb_node){_BST_find(root->root.node, key, compare)};
}

rb_node rb_remove(rb_tree *root, void *key, compare_fun compare)
{
    _Rb_node    *ret = _Rb_remove(&root->root.node, key, compare);
    if (UNLIKELY(root->root.node == NULL))
    {
        root->begin.node = NULL;
        root->end.node = NULL;
    }
    else if (ret == root->begin.node)
        root->begin.node = _Rb_next(root->begin.node);
    else if (ret == root->end.node)
        root->end.node = _Rb_prev(root->end.node);
    return (rb_node){ret};
}

rb_node rb_next(rb_node node)
{
    return (rb_node){_Rb_next(node.node)};
}

rb_node rb_prev(rb_node node)
{
    return (rb_node){_Rb_prev(node.node)};
}

rb_tree rb_copy(rb_tree *root, copy_fun copy)
{
    rb_tree tree = {NULL, NULL, NULL};
    tree.root.node = _Rb_tree_copy(root->root.node, copy);
    tree.begin.node = _BST_min(tree.root.node);
    tree.end.node = _BST_max(tree.root.node);
    return tree;
}

void rb_destroy(rb_tree *root, del_fun del)
{
    _Rb_tree_destroy(root->root.node, del);
    root->begin.node = NULL;
    root->end.node = NULL;
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

static _Rb_node     *_BST_insert(_Rb_node *root, _Rb_node *node, compare_fun compare)
/*
    function inserts element `node` to Binary Search Tree with root `root`
*/
{
    assert(root);
    assert(node);
    assert(compare);
    while (1)
    {
        int cmp = compare(node->key, root->key);
        if (cmp == 0)
            return root;
        if (cmp < 0)
        {
            if (root->left == NULL)
            {
                root->left = node;
                node->parent = root;
                return NULL;
            }
            root = root->left;
        }
        else
        {
            if (root->right == NULL)
            {
                root->right = node;
                node->parent = root;
                return NULL;
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

static _Rb_node     *_Rb_insert(_Rb_node **root, _Rb_node *node, compare_fun compare)
{
    assert(root);
    assert(node);
    assert(compare);
    if (*root == NULL)
    {
        *root = node;
        node->color = _Rb_Black;
        return node;
    }
    _Rb_node *inserted = _BST_insert(*root, node, compare);
    if (inserted != NULL)
        return inserted;
    while (node != *root && node->color == _Rb_Red)
    {
        if (node->parent->color == _Rb_Black)
            return node;
        _Rb_node *gp = _Rb_grandparent(node);
        if (gp == NULL)
            break ;
        if (node->parent == gp->left) // left cases
            node = _Rb_insert_case_1(node, gp, root);
        else // right cases
            node = _Rb_insert_case_2(node, gp, root);
    }
    (*root)->color = _Rb_Black;
    return node;
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

static _Rb_node     *_BST_remove(_Rb_node *node)
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
        return NULL;
    }
    // case 3
    _Rb_node *accessor = _BST_max(node->left);
    node->key = accessor->key;
    return _BST_remove(accessor);
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

static _Rb_node     *_Rb_remove(_Rb_node **root, void *value, compare_fun compare)
{
    assert(root);
    assert(compare);
    if (*root == NULL)
        return NULL;
    _Rb_node *rm = _BST_find(*root, value, compare);
    if (rm == NULL)
        return NULL;
    _Rb_node *leaf = _BST_remove(rm);
    if (leaf == NULL)
        return rm;
    if (leaf == *root)
    {
        if (leaf->left)
            *root = leaf->left;
        else if (leaf->right)
            *root = leaf->right;
        else
            *root = NULL;
        if (*root)
        {
            (*root)->parent = NULL;
            (*root)->color = _Rb_Black;
        }
        return rm;
    }
    if (leaf->color != _Rb_Red)
        _Rb_fix_double_black(leaf, root);
    if (leaf->parent->left == leaf)
        leaf->parent->left = NULL;
    else
        leaf->parent->right = NULL;
    return rm;
}

static _Rb_node    *_Rb_next(_Rb_node *node)
{
    assert(node);
    if (node->right)
        return _BST_min(node->right);
    while (node->parent && node->parent->right == node)
        node = node->parent;
    if (LIKELY(node->parent != NULL))
        return node->parent;
    return node;
}

static _Rb_node    *_Rb_prev(_Rb_node *node)
{
    assert(node);
    if (node->left)
        return _BST_max(node->left);
    while (node->parent && node->parent->left == node)
        node = node->parent;
    if (LIKELY(node->parent != NULL))
        return node->parent;
    return node;
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

static _Rb_node     *_Rb_lower_bound(_Rb_node *root, void *value, compare_fun compare)
{
    _Rb_node *ret = root;
    while (root != NULL)
    {
        if (compare(value, ret->key) >= 0)
        {
            ret = root;
            root = root->left;
        }
        else
            root = root->right;
    }
    if (compare(value, ret->key) < 0)
        return NULL;
    return ret;
}

static _Rb_node     *_Rb_upper_bound(_Rb_node *root, void *value, compare_fun compare)
{
    _Rb_node *ret = root;
    while (root != NULL)
    {
        if (compare(value, ret->key) <= 0)
        {
            ret = root;
            root = root->right;
        }
        else
            root = root->left;
    }
    if (compare(value, ret->key) > 0)
        return NULL;
    return ret;
}
