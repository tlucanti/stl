
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

static _Rb_node    *_Rb_rotate_right(_Rb_node *node)
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
    _print_rb_tree(node->parent, "before right rotation");
    pivot = node->left;
    if (node->parent)
    {
        if (node == node->parent->left)
            node->parent->left = pivot;
        else
            node->parent->right = pivot;
    }
    pivot->parent = node->parent; // step 1
    _print_rb_tree(node->parent, "step 1");
    node->left = pivot->right; // step 2
    _print_rb_tree(node->parent, "step 2");
    _print_rb_tree(pivot, "step 2 pivot");
    if (node->left)
        node->left->parent = node; // step 3
    pivot->right = node; // step 4
    _print_rb_tree(pivot, "step 4 pivot");
    node->parent = pivot; // step 5
    _print_rb_tree(node->parent, "step 5");
    return node;
}

static _Rb_node    *_Rb_rotate_left(_Rb_node *node)
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
    _print_rb_tree(node, "before left rotation");
    pivot = node->right;
    if (node->parent)
    {
        if (node == node->parent->left)
            node->parent->left = pivot;
        else
            node->parent->right = pivot;
    }
    pivot->parent = node->parent;
    _print_rb_tree(node, "step 1");
    node->right = pivot->left;
    _print_rb_tree(node, "step 2");
    _print_rb_tree(pivot, "step 1 pivot");

    if (node->right)
        node->right->parent = node;
    pivot->left = node;
    _print_rb_tree(node, "step 3");

    node->parent = pivot;
    return node;
}

static _Rb_node     *_BST_insert(_Rb_node *root, _Rb_node *node, int (*compare)(void *, void *))
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

static _Rb_node    *_BST_find(_Rb_node *root, void *value, int (*compare)(void *, void *))
{
    assert(compare);

    if (root == NULL)
        return NULL;
    while (1)
    {
        int cmp = compare(value, root->key);
        if (cmp == 0)
            return root;
        if (compare(root->key, value) < 0)
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
     - uncle is colo_Rb_Red _Rb_Black
     - parent is colo_Rb_Red _Rb_Black
     - grandparent is colo_Rb_Red _Rb_Red

    in this example [a] means _Rb_Black node, and (a) - is _Rb_Red, {a} means any color

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
    in examples [a] means _Rb_Black node, (a) - is _Rb_Red, {a} means, that color does
    not mater. {*} means, that this sub-tree has same _Rb_Black-height

    case one means that we can do recoloring and move violation to the upper
    level (to grandparent and grand-grandparent), until we hit the root of the
    tree case one can be applied, only if uncle and parent has same (_Rb_Red) color

             /                               /
       [grandparent]                   (grandparent)
          /     \                        /       \
     (uncle)   (parent)      -->    [uncle]     [parent]
      /   \     /    \               /   \     /    \
    {*}   {*} {*}   (node)         {*}   {*} {*}   (node)
    
    case two means the we cannot do recoloring due to different colors of
    (uncle) and (parent), because otherwise we will violate `_Rb_Black-height` rule
    so we need to do rotation to balance _Rb_Red nodes between _Rb_Black ones, but
    before this we need to straighten the node-parent-grandprent line in order
    to not break `_Rb_Black-height` rule in next rotation

    

*/
{
    assert(node);
    assert(gp);
    if (gp->right && gp->right->color == _Rb_Red)
        return _Rb_recolor(node); // case 1
    else
    {
        if (node == node->parent->right)
            node = _Rb_rotate_left(node->parent); // case 2
        _Rb_rotate_right(gp)->color = _Rb_Red; // case 3
        node->parent->color = _Rb_Black;
        if (gp == *root)
            *root = node->parent;
        _print_rb_tree(node->parent, "after rotate");
        return node->parent;
    }
}

static _Rb_node    *_Rb_insert_case_2(_Rb_node *node, _Rb_node *gp, _Rb_node **root)
{
    assert(node);
    assert(gp);
    if (gp->left && gp->left->color == _Rb_Red)
        return _Rb_recolor(node); // case 1
    else
    {
        if (node == node->parent->left)
            node = _Rb_rotate_right(node->parent); // case 2
        _Rb_rotate_left(gp)->color = _Rb_Red; // case 3
        node->parent->color = _Rb_Black;
        if (gp == *root)
            *root = node->parent;
        return node->parent;
    }
}

static _Rb_node *_Rb_insert(_Rb_node **root, _Rb_node *node, int (*compare)(void *, void *))
{
    assert(root);
    assert(node);
    if (*root == NULL)
    {
        *root = node;
        node->color = _Rb_Black;
        return node;
    }
    _Rb_node *inserted = _BST_insert(*root, node, compare);
    if (inserted != NULL)
        return inserted;
    _print_rb_tree(*root, "after BST insert");
    if (node->parent->color == _Rb_Black)
        return node;
    while (node != *root && node->color == _Rb_Red)
    {
        _Rb_node *gp = _Rb_grandparent(node);
        if (node->parent == gp->left) // left cases
            node = _Rb_insert_case_1(node, gp, root);
        else // right cases
            node = _Rb_insert_case_2(node, gp, root);
    }
    (*root)->color = _Rb_Black;
    return node;
}

void *rb_insert(rb_tree *root, void *key, int (*compare)(void *, void *))
{
    _Rb_node *node = _Rb_node_create(key);
    return _Rb_insert(&(root->root), node, compare)->key;
}

void *rb_find(rb_tree *root, void *key, int (*compare)(void *, void *))
{
    _Rb_node *result = _BST_find(root->root, key, compare);
    if (result == NULL)
        return NULL;
    return result->key;
}