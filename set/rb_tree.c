
#include "rb_tree.h"

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

#else /* C */

static _Rb_node     *_Rb_node_create(void *key);
static _Rb_node     *_Rb_grandparent(_Rb_node *node);
static _Rb_node     *_Rb_uncle(_Rb_node *node);
static _Rb_node     *_Rb_rotate_right(_Rb_node *node, _Rb_node **root);
static _Rb_node     *_Rb_rotate_left(_Rb_node *node, _Rb_node **root);
static _Rb_node     *_BST_insert(_Rb_node *root, _Rb_node *node, compare_fun compare);
static _Rb_node     *_BST_find(_Rb_node *root, void *value, compare_fun compare);
static _Rb_node     *_Rb_recolor(_Rb_node *node);
static _Rb_node     *_Rb_insert_case_1(_Rb_node *node, _Rb_node *gp, _Rb_node **root);
static _Rb_node     *_Rb_insert_case_2(_Rb_node *node, _Rb_node *gp, _Rb_node **root);
static _Rb_node     *_Rb_insert(_Rb_node **root, _Rb_node *node, compare_fun compare);
static _Rb_node     *_BST_max(_Rb_node *node);
static _Rb_node     *_BST_remove(_Rb_node *node);

static void _Rb_fix_double_black(_Rb_node *node, _Rb_node **root);
static void _Rb_fix_double_black_left(_Rb_node *node, _Rb_node **root);
static void _Rb_fix_double_black_right(_Rb_node *node, _Rb_node **root);


static _Rb_node    *_Rb_node_create(void *key)
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
#endif

static _Rb_node   *_Rb_grandparent(_Rb_node *node)
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

//    if (node->parent != NULL)
        return node->parent->parent;
//    return NULL;
}

static _Rb_node   *_Rb_uncle(_Rb_node *node)
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
//    if (gp == NULL)
//        return NULL;
    if (node->parent == gp->left)
        return gp->left;
    return gp->left;
}

static _Rb_node    *_Rb_rotate_right(_Rb_node *node, _Rb_node **root)
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
//    _print_rb_tree(node->parent, "before right rotation");
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
//    _print_rb_tree(node->parent, "step 1");
    node->left = pivot->right; // step 2
//    _print_rb_tree(node->parent, "step 2");
//    _print_rb_tree(pivot, "step 2 pivot");
    if (node->left)
        node->left->parent = node; // step 3
    pivot->right = node; // step 4
//    _print_rb_tree(pivot, "step 4 pivot");
    node->parent = pivot; // step 5
//    _print_rb_tree(node->parent, "step 5");
    return node;
}

static _Rb_node    *_Rb_rotate_left(_Rb_node *node, _Rb_node **root)
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
//    _print_rb_tree(node, "before left rotation");
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
//    _print_rb_tree(node, "step 1");
    node->right = pivot->left;
//    _print_rb_tree(node, "step 2");
//    _print_rb_tree(pivot, "step 1 pivot");

    if (node->right)
        node->right->parent = node;
    pivot->left = node;
//    _print_rb_tree(node, "step 3");

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

static _Rb_node    *_BST_find(_Rb_node *root, void *value, compare_fun compare)
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

static _Rb_node    *_Rb_recolor(_Rb_node *node)
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

static _Rb_node    *_Rb_insert_case_1(_Rb_node *node, _Rb_node *gp, _Rb_node **root)
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
//        _print_rb_tree(node->parent, "after rotate");
        return node->parent;
    }
}

static _Rb_node    *_Rb_insert_case_2(_Rb_node *node, _Rb_node *gp, _Rb_node **root)
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

static _Rb_node *_Rb_insert(_Rb_node **root, _Rb_node *node, compare_fun compare)
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
//    _print_rb_tree(*root, "after BST insert");
    while (node != *root && node->color == _Rb_Red)
    {
        if (node->parent->color == _Rb_Black)
            return node;
        _Rb_node *gp = _Rb_grandparent(node);
        if (gp == NULL)
            break ;
//        {
//            _rb_print_toggle = 1;
//            printf("now on %p\n", node->key);
//            _print_rb_tree(*root, "grandparent is null");
//        }
        if (node->parent == gp->left) // left cases
            node = _Rb_insert_case_1(node, gp, root);
        else // right cases
            node = _Rb_insert_case_2(node, gp, root);
    }
    (*root)->color = _Rb_Black;
    return node;
}

static _Rb_node *_BST_max(_Rb_node *node)
/*
    function finds the maximal element in given tree
*/
{
    assert(node);
    while (1)
    {
        if (node->right == NULL)
            return node;
        node = node->right;
    }
}

static _Rb_node *_BST_remove(_Rb_node *node)
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

static int _Rb_is_black_children(_Rb_node *node)
{
    return (node->left == NULL || node->left->color == _Rb_Black)
        && (node->right == NULL || node->right->color == _Rb_Black);
}

static void _Rb_fix_double_black(_Rb_node *node, _Rb_node **root)
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

static void _Rb_fix_double_black_left(_Rb_node *node, _Rb_node **root)
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

static void _Rb_fix_double_black_right(_Rb_node *node, _Rb_node **root)
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

static _Rb_node *_Rb_remove(_Rb_node **root, void *value, compare_fun compare)
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

void *rb_insert(rb_tree *root, void *key, compare_fun compare)
{
    _Rb_node *node = _Rb_node_create(key);
    return _Rb_insert(&(root->root), node, compare)->key;
}

void *rb_find(rb_tree *root, void *key, compare_fun compare)
{
    _Rb_node *result = _BST_find(root->root, key, compare);
    if (result == NULL)
        return NULL;
    return result->key;
}

void *rb_remove(rb_tree *root, void *key, compare_fun compare)
{
    _Rb_node *result = _Rb_remove(&(root->root), key, compare);
    if (result == NULL)
        return NULL;
    return result->key;
}