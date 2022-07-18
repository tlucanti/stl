
#include "rb_tree.h"

_Rb_node   *_Rb_grandparent(_Rb_node *node)
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

_Rb_node   *_Rb_uncle(_Rb_node *node)
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

void _Rb_rotate_right(_Rb_node *node)
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
    pivot = node->left;
    pivot->parent = node->parent;
    node->left = pivot->right;
    node->left->parent = node;
    pivot->left = node;
    node->parent = pivot;
    return pivot;
}

_Rb_node    *_Rb_rotate_left(_Rb_node *node)
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
    pivot = node->right;
    pivot->parent = node->parent;
    node->right = pivot->left;
    node->right->parent = node;
    pivot->right = node;
    node->parent = pivot;
    return node;
}

void _BST_insert(_Rb_node *root, _Rb_node *node)
/*
    function inserts element `node` to Binary Search Tree with root `root`
*/
{
    assert(root);
    assert(node);
    while (1)
    {
        if (node->key < root->key)
        {
            if (root->left == NULL)
            {
                root->left = node;
                return ;
            }
            root = root->left;
        }
        else
        {
            if (root->right == NULL)
            {
                root->right = node;
                return ;
            }
            root = root->right;
        }
    }
}

_Rb_node    *_Rb_recolor(_Rb_node *node)
/*
    function does recoloring to serval neighbours:
     - uncle is colored Black
     - parent is colored Black
     - grandparent is colored Red

    in this example [a] means Black node, and (a) - is Red, {a} means any color

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
    gp->color = Red;
    gp->left->color = Black;
    gp->right->color = Black;
    return gp;
}

_Rb_node    *_Rb_insert_case_1(_Rb_node *node, _Rb_node *gp)
/*
    function eliminates violations after inserting node to tree
    in examples [a] means Black node, (a) - is Red, {a} means, that color does
    not mater. {*} means, that this sub-tree has same black-height

    case one means that we can do recoloring and move violation to the upper
    level (to grandparent and grand-grandparent), until we hit the root of the
    tree case one can be applied, only if uncle and parent has same (Red) color

             /                               /
       [grandparent]                   (grandparent)
          /     \                        /       \
     (uncle)   (parent)      -->    [uncle]     [parent]
      /   \     /    \               /   \     /    \
    {*}   {*} {*}   (node)         {*}   {*} {*}   (node)
    
    case two means the we cannot do recoloring due to different colors of
    (uncle) and (parent), because otherwise we will violate `black-height` rule
    so we need to do rotation to balance red nodes between black ones, but
    before this we need to straighten the node-parent-grandprent line in order
    to not break `black-height` rule in next rotation

    

*/
{
    assert(node);
    assert(gp);
    if (gp->right->color == Red)
        return _Rb_recolor(node); // case 1
    else
    {
        if (node == node->parent->right)
            node = _Rb_rotate_left(node->parent); // case 2
        _Rb_rotate_right(gp)->color = Red; // case 3
        node->parent->color = Black;
        return node->parent;
    }
}

_Rb_node    *_Rb_insert_case_2(_Rb_node *node, _Rb_node *gp)
{
    assert(node);
    assert(gp);
    if (gp->left->color == Red)
        return _Rb_recolor(node); // case 1
    else
    {
        if (node == node->parent->left)
            node = _Rb_rotate_right(node->parent); // case 2
        _Rb_rotate_left(gp)->color = Red; // case 3
        node->parent->color = Black;
        return node->parent;
    }
}

void _Rb_insert(_Rb_node **root, _Rb_node *node)
{
    assert(root);
    assert(node);
    if (*root == NULL)
    {
        *root = node;
        node->color = Black;
        return ;
    }
    _BST_insert(root, node);
    node->color = Red;
    while (node != *root && node->color == Red)
    {
        _Rb_node gp = _Rb_grandparent(node);
        if (node->parent == gp->left) // left cases
            node = _Rb_insert_case_1(node, gp);
        else // right cases
            node = _Rb_insert_case_2(node, gp);
    }
    (*root)->color = Black;
}
