
#ifndef RB_TREE_HPP
# define RB_TREE_HPP

# include "defs.h"
# include <cassert>
# include <cstddef>

TLU_NAMESPACE_BEGIN

template <class type_T, class cmp_T>
class rb_tree
{
private:
    enum rb_colors
    {
        rb_red,
        rb_black
    };

public:
    class rb_node
    {
//    private:
    public:
        rb_node(
            rb_node *_parent,
            rb_node *_left,
            rb_node *_right,
            rb_colors _color,
            const type_T &_key
        ) :
            parent(_parent),
            left(_left),
            right(_right),
            color(_color),
            key(_key)
        {}

        rb_node     *parent;
        rb_node     *left;
        rb_node     *right;

        rb_colors   color;

    public:
        type_T      key;
    };

    friend class rb_node;

private:
    rb_node     *_root;
    rb_node     *_begin;
    rb_node     *_end;
    const cmp_T &_cmp;

public:

    explicit rb_tree(const cmp_T &cmp)
        : _root(nullptr), _cmp(cmp)
    {}

#if CPP11
    rb_tree(rb_tree &&mv)
        : _root(mv._root), _cmp(std::move(mv._cmp))
    {}
#endif

    rb_tree(const rb_tree &cpy)
        : _root(nullptr), _cmp(cpy._cmp)
    {
        _root = _rb_copy(cpy._root);
    }

    ~rb_node()
    {
        _rb_destroy(_root);
    }

    rb_node *insert(const type_T &key)
    {
        rb_node *node = new rb_node(nullptr, nullptr, nullptr, rb_black, key);
        return _rb_insert(node);
    }
    
    rb_node *find(const type_T &key)
    {
        return _bst_find(key);
    }
    
    rb_node *remove(const type_T &key)
    {
        return _rb_remove(key);
    }
    
    rb_node *next(const type_T &key)
    {
        return _rb_next(key);
    }
    
    rb_node *prev(const type_T &key)
    {
        return _rb_prev(key);
    }
    
    rb_node *lower_bound(const type_T &key)
    {
        return _lower_bound(key);
    }
    
    rb_node *upper_bound(const type_T &key)
    {
        return _upper_bound(key);
    }

private:
    
    rb_node     *_rb_grandparent(rb_node *node)
    {
        return node->parent->parent;
    }

    rb_node     *_rb_uncle(rb_node *node)
    {
        rb_node   *gp;

        gp = _Rb_grandparent(node);
        if (node->parent == gp->left)
            return gp->left;
        return gp->left;
    }

    rb_node     *_rb_rotate_right(rb_node *node)
    {
        rb_node    *pivot;

        pivot = node->left;
        if (node == _root)
            _root = pivot;
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

    rb_node     *_rb_rotate_left(rb_node *node)
    {
        rb_node    *pivot;

        pivot = node->right;
        if (node == _root)
            _root = pivot;
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

    rb_node     *_bst_insert(rb_node *node)
    {
        rb_node *root = _root;
        while (true)
        {
            int cmp = _compare_fun(node->key, root->key);
            if (cmp == 0)
                return root;
            if (cmp < 0)
            {
                if (root->left == nullptr)
                {
                    root->left = node;
                    node->parent = root;
                    return nullptr;
                }
                root = root->left;
            }
            else
            {
                if (root->right == nullptr)
                {
                    root->right = node;
                    node->parent = root;
                    return nullptr;
                }
                root = root->right;
            }
        }
    }

    rb_node     *_bst_find(const type_T &value)
    {
        if (_root == nullptr)
            return nullptr;
        rb_node *root = _root;
        while (true)
        {
            int cmp = _compare_fun(value, root->key);
            if (cmp == 0)
                return root;
            else if (cmp < 0)
            {
                if (root->left == nullptr)
                    return nullptr;
                root = root->left;
            }
            else
            {
                if (root->right == nullptr)
                    return nullptr;
                root = root->right;
            }
        }
    }

    rb_node     *_rb_recolor(rb_node *node)
    {
        rb_node    *gp;

        assert(node);
        gp = _rb_grandparent(node);
        gp->color = rb_red;
        gp->left->color = rb_black;
        gp->right->color = rb_black;
        return gp;
    }

    rb_node     *_rb_insert_case_1(rb_node *node, rb_node *gp)
    {
        if (gp->right && gp->right->color == rb_red)
            return _rb_recolor(node); // case 1
        else
        {
            if (node == node->parent->right)
                node = _rb_rotate_left(node->parent); // case 2
            _rb_rotate_right(gp)->color = rb_red; // case 3
            node->parent->color = rb_black;
            if (gp == _root)
                _root = node->parent;
            return node->parent;
        }
    }

    rb_node     *_rb_insert_case_2(rb_node *node, rb_node *gp)
    {
        if (gp->left && gp->left->color == rb_red)
            return _rb_recolor(node); // case 1
        else
        {
            if (node == node->parent->left)
                node = _rb_rotate_right(node->parent); // case 2
            _rb_rotate_left(gp)->color = rb_red; // case 3
            node->parent->color = rb_black;
            if (gp == _root)
                _root = node->parent;
            return node->parent;
        }
    }

    rb_node     *_rb_insert(rb_node *node)
    {
        if (_root == nullptr)
        {
            _root = node;
            node->color = rb_black;
            return node;
        }
        rb_node *inserted = _bst_insert(node);
        if (inserted != nullptr)
            return inserted;
        while (node != _root && node->color == rb_red)
        {
            if (node->parent->color == rb_black)
                return node;
            rb_node *gp = _rb_grandparent(node);
            if (gp == nullptr)
                break ;
            if (node->parent == gp->left) // left cases
                node = _rb_insert_case_1(node, gp);
            else // right cases
                node = _rb_insert_case_2(node, gp);
        }
        _root->color = rb_black;
        return node;
    }

    rb_node     *_bst_max(rb_node *node)
    {
        while (node->right != nullptr)
            node = node->right;
        return node;
    }

    rb_node     *_bst_min(rb_node *node)
    {
        while (node->left != nullptr)
            node = node->left;
        return node;
    }

    rb_node     *_bst_remove(rb_node *node)
    {
        if (node->left == nullptr && node->right == nullptr)
            // case 1
            return node;
        if (node->left == nullptr || node->right == nullptr)
            // case 2
        {
            rb_node *child;
            if (node->right == nullptr)
                child = node->left;
            else
                child = node->right;
            if (node->parent == nullptr)
                return node;
            else if (node->parent->left == node)
                node->parent->left = child;
            else
                node->parent->right = child;
            child->parent = node->parent;
            child->color = node->color;
            return nullptr;
        }
        // case 3
        rb_node *accessor = _bst_max(node->left);
        node->key = accessor->key;
        return _bst_remove(accessor);
    }

    bool          _rb_is_black_children(rb_node *node)
    {
        return (node->left == nullptr || node->left->color == rb_black)
               && (node->right == nullptr || node->right->color == rb_black);
    }

    void         _rb_fix_double_black(rb_node *node)
    {
        if (node == _root)
            return ;
        if (node->parent->left == node)
            _rb_fix_double_black_left(node);
        else
            _rb_fix_double_black_right(node);
    }

    void         _rb_fix_double_black_left(rb_node *node)
    {
        if (node == _root)
            // case 2
            return ;
        rb_node *parent = node->parent;
        rb_node *sibling = parent->right;
        if (sibling->color == rb_black && _rb_is_black_children(sibling))
            // case 3
        {
            sibling->color = rb_red;
            if (parent->color == rb_red)
                parent->color = rb_black;
            else
                _rb_fix_double_black(parent);
            return ;
        }
        else if (sibling->color == rb_red)
            // case 4
        {
            sibling->color = parent->color;
            parent->color = rb_red;
            _rb_rotate_left(parent);
            _rb_fix_double_black_left(node);
            return ;
        }
        else if (sibling->right == nullptr || sibling->right->color == rb_black)
            // case 5
        {
            rb_colors sib_col = sibling->color;
            sibling->color = sibling->left->color;
            sibling->left->color = sib_col;
            _rb_rotate_right(sibling);
            sibling = sibling->parent;
        }
        // case 6
        rb_colors sib_col = sibling->color;
        sibling->color = parent->color;
        parent->color = sib_col;
        if (sibling->right)
            sibling->right->color = rb_black;
        _rb_rotate_left(parent);
    }

    void         _rb_fix_double_black_right(rb_node *node)
    {
        if (node == _root)
            // case 2
            return ;
        rb_node *parent = node->parent;
        rb_node *sibling = parent->left;
        if (sibling->color == rb_black && _rb_is_black_children(sibling))
            // case 3
        {
            sibling->color = rb_red;
            if (parent->color == rb_red)
                parent->color = rb_black;
            else
                _rb_fix_double_black(parent);
            return ;
        }
        else if (sibling->color == rb_red)
            // case 4
        {
            sibling->color = parent->color;
            parent->color = rb_red;
            _rb_rotate_right(parent);
            _rb_fix_double_black_right(node);
            return ;
        }
        else if (sibling->left == nullptr || sibling->left->color == rb_black)
            // case 5
        {
            rb_colors sib_col = sibling->color;
            sibling->color = sibling->right->color;
            sibling->right->color = sib_col;
            _rb_rotate_left(sibling);
            sibling = sibling->parent;
        }
        // case 6
        rb_colors sib_col = sibling->color;
        sibling->color = parent->color;
        parent->color = sib_col;
        if (sibling->left)
            sibling->left->color = rb_black;
        _rb_rotate_right(parent);
    }

    rb_node     *_rb_remove(const type_T &value)
    {
        if (_root == nullptr)
            return nullptr;
        rb_node *rm = _bst_find(value);
        if (rm == nullptr)
            return nullptr;
        rb_node *leaf = _bst_remove(rm);
        if (leaf == nullptr)
            return rm;
        if (leaf == _root)
        {
            if (leaf->left)
                _root = leaf->left;
            else if (leaf->right)
                _root = leaf->right;
            else
                _root = nullptr;
            if (_root)
            {
                _root->parent = nullptr;
                _root->color = rb_black;
            }
            return rm;
        }
        if (leaf->color != rb_red)
            _rb_fix_double_black(leaf);
        if (leaf->parent->left == leaf)
            leaf->parent->left = nullptr;
        else
            leaf->parent->right = nullptr;
        return rm;
    }

    rb_node    *_rb_next(rb_node *node)
    {
        if (node->right)
            return _bst_min(node->right);
        while (node->parent->right == node)
            node = node->parent;
        return node->parent;
    }

    rb_node    *_rb_prev(rb_node *node)
    {
        if (node->left)
            return _bst_max(node->left);
        while (node->parent->left == node)
            node = node->parent;
        return node->parent;
    }

    int _compare_fun(const type_T &lhs, const type_T &rhs)
    {
        if (_cmp(lhs, rhs))
            return -1;
        else if (rhs < lhs)
            return 0;
        return 1;
    }

    rb_node *_rb_copy(rb_node *root)
    {
        rb_node *node = new rb_node;
        node->key = new type_T(*(root->key));
        node->color = root->color;
        if (root->left)
            node->left = _rb_copy(root->left);
        if (root->right)
            node->right = _rb_copy(root->right);
        return node;
    }

    void _rb_destroy(rb_node *node)
    {
        delete node->key;
        if (node->left)
            _rb_destroy(node->left);
        if (node->right)
            _rb_destroy(node->right);
    }
};

TLU_NAMESPACE_END

#endif /* RB_TREE_HPP */
