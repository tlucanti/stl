/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rb_tree.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tlucanti <tlucanti@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/14 11:26:42 by tlucanti          #+#    #+#             */
/*   Updated: 2022/08/14 20:03:45 by tlucanti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef RB_TREE_HPP
# define RB_TREE_HPP

# include <cassert>
# include <cstddef>
# include <memory>
# include <functional>
# include "defs.hpp"
# include "utils.hpp"

TLU_NAMESPACE_BEGIN


// =============================================================================
// -------------------------------- vector_base --------------------------------
template <
        class key_T,
        class cmp_T=std::less<key_T>,
        class allocator_T=std::allocator<key_T>
>
class rb_tree
{
// =============================================================================
// --------------------------------- rb_node ----------------------------------
public:
    class rb_node
    {
        friend class rb_tree;

    // ---------------------------- public members -----------------------------
    public:
        enum rb_colors
        {
            rb_red = false,
            rb_black = true
        };

    // -------------------------------------------------------------------------
        constexpr rb_node(
                rb_node     *_parent,
                rb_node     *_left,
                rb_node     *_right,
                rb_colors   _color,
                key_T    *_key
        ) noexcept :
                parent(_parent),
                left(_left),
                right(_right),
                color(_color),
                key(_key)
        {}

    // ---------------------------- private members ----------------------------
    private:
        rb_node     *parent;
        rb_node     *left;
        rb_node     *right;
        rb_colors   color;
        key_T       *key;

    // ---------------------------- public methods -----------------------------
    public:
        constexpr key_T &get_key() const noexcept
        {
            return *key;
        }

    }; /* rb_node */

// ------------------------------ private fields -------------------------------
private:
    rb_node         *_root;
    rb_node         *_begin;
    rb_node         *_end;
    cmp_T           _cmp;
    allocator_T     _alloc;
    size_t          _size;

    static const typename rb_node::rb_colors
        rb_red = rb_node::rb_red,
        rb_black = rb_node::rb_black;
    typedef typename rb_node::rb_colors rb_colors;

// ------------------------------ initialization -------------------------------
public:
    constexpr rb_tree() noexcept :
        _root(nullptr),
        _begin(nullptr),
        _end(nullptr),
        _cmp(cmp_T()),
        _alloc(allocator_T()),
        _size(0)
    {}

// -----------------------------------------------------------------------------
    constexpr rb_tree(const cmp_T &cmp, const allocator_T &alloc) constexpr :
        _root(nullptr),
        _begin(nullptr),
        _end(nullptr),
        _cmp(cmp),
        _alloc(alloc),
        _size(0)
    {}

// -----------------------------------------------------------------------------
#if CPP11
    constexpr rb_tree(rb_tree &&mv) noexcept :
        _root(mv._root),
        _begin(mv._begin),
        _end(mv._end),
        _cmp(std::move(mv._cmp)),
        _alloc(std::move(mv._alloc)),
        _size(mv._size)
    {
        mv._root = nullptr;
        mv._begin = nullptr;
        mv._end = nullptr;
        mv._size = 0;
    }
#endif

// -----------------------------------------------------------------------------
    constexpr rb_tree(const rb_tree &cpy) :
        _root(nullptr),
        _begin(nullptr),
        _end(nullptr),
        _cmp(cpy._cmp),
        _alloc(cpy._alloc),
        _size(0)
    {
        if (cpy._size == 0)
            return ;
        _root = _rb_copy(cpy._root);
        _begin = _bst_min(_root);
        _end = _bst_max(_root);
    }

// -----------------------------------------------------------------------------
    ~rb_tree()
    {
        if (LIKELY(_root != nullptr))
            _rb_destroy(_root);
    }

// ------------------------------ private methods ------------------------------
private:
    constexpr int   compare(const key_T *lhs, const key_T *rhs) const
    {
        if (_cmp(*lhs, *rhs))
            return -1;
        else if (_cmp(*rhs, *lhs))
            return 1;
        return 0;
    }

// ------------------------------ public methods -------------------------------
public:
    constexpr rb_node   *end() const noexcept
    {
        return _end;
    }

// -----------------------------------------------------------------------------
    constexpr rb_node   *begin() const noexcept
    {
        return _begin;
    }

// -----------------------------------------------------------------------------
    constexpr rb_node   *insert(const key_T &key)
    {
        bool _;
        return insert(_root, key, _);
    }

// -----------------------------------------------------------------------------
    constexpr rb_node   *insert(const key_T &key, bool &was_inserted)
    {
        return insert(_root, key, was_inserted);
    }

// -----------------------------------------------------------------------------
    constexpr rb_node   *insert(
            rb_node *hint,
            const key_T &key,
            bool &was_inserted
        )
    {
        AUTO(key_T   *) key_ptr = new key_T(key);
        AUTO(rb_node *) node = new rb_node(nullptr, nullptr, nullptr,
            rb_red, key_ptr);
        was_inserted = true;
        if (UNLIKELY(_root == nullptr))
        {
            _begin = node;
            _end = node;
            _root = node;
            _root->color = rb_black;
            ++_size;
            return _root;
        }
        else if (compare(key_ptr, _root->key) <= -1)
            _begin = node;
        else if (compare(key_ptr, _root->key) >= 1)
            _end = node;

        rb_node *ret = _rb_insert(hint, node, was_inserted);
        if (was_inserted)
            ++_size;
        return ret;
    }
    
// -----------------------------------------------------------------------------
    constexpr rb_node   *find(
            const key_T &key,
            rb_node *start_from=nullptr
        ) const noexcept
    {
        if (start_from == nullptr)
            start_from = _root;
        return _bst_find(&key, start_from);
    }
    
// -----------------------------------------------------------------------------
    constexpr rb_node   *remove(const key_T &key) noexcept
    {
        if (UNLIKELY(_root == nullptr))
            return nullptr;
        rb_node *ret = _rb_remove(&key);
        if (_root == nullptr)
        {
            _begin = nullptr;
            _end = nullptr;
        }
        else
        {
            _begin = _bst_min(_root);
            _end = _bst_max(_root);
        }
        return ret;
    }

// -----------------------------------------------------------------------------
    constexpr rb_node   *remove_node(rb_node *node) noexcept
    {
        --_size;
        rb_node *ret;
        if (_end == node)
            ret = nullptr;
        else
            ret = next(node);
        _rb_remove_node(node);
        if (UNLIKELY(_root == nullptr))
        {
            _begin = nullptr;
            _end = nullptr;
        }
        else
        {
            _begin = _bst_min(_root);
            _end = _bst_max(_root);
        }
        if (ret == nullptr)
            return _end;
        return ret;
    }
    
// -----------------------------------------------------------------------------
    static constexpr rb_node    *next(rb_node *node)
    {
        return _bst_next(node);
    }
    
// -----------------------------------------------------------------------------
    static constexpr rb_node    *prev(rb_node *node)
    {
        return _bst_prev(node);
    }
    
// -----------------------------------------------------------------------------
    constexpr rb_node   *lower_bound(const key_T &key) const noexcept
    {
        if (_root == nullptr)
            return nullptr;
        return _bst_lower_bound(&key);
    }
    
// -----------------------------------------------------------------------------
    constexpr rb_node   *upper_bound(const key_T &key) const noexcept
    {
        if (_root == nullptr)
            return nullptr;
        return _bst_upper_bound(&key);
    }

// -----------------------------------------------------------------------------
    constexpr std::size_t   size() const noexcept
    {
        return _size;
    }

// -----------------------------------------------------------------------------
    constexpr void  assign(const rb_tree &cpy)
    {
        if (_root != nullptr)
            _rb_destroy(_root);
        _root = _rb_copy(cpy._root);
        _begin = _bst_min(_root);
        _end = _bst_max(_root);
        _size = cpy._size;
    }

// -----------------------------------------------------------------------------
    constexpr void  destroy()
    {
        if (_root == nullptr)
            return ;
        _rb_destroy(_root);
        _root = nullptr;
        _begin = nullptr;
        _end = nullptr;
        _size = 0;
    }

// -----------------------------------------------------------------------------
    constexpr allocator_T   get_allocator() const noexcept
    {
        return _alloc;
    }

// -----------------------------------------------------------------------------
    constexpr cmp_T     get_cmp() const noexcept
    {
        return _cmp;
    }

// -----------------------------------------------------------------------------
    constexpr void  swap(rb_tree &swp) noexcept
    {
        std::swap(_root, swp._root);
        std::swap(_begin, swp._begin);
        std::swap(_end, swp._end);
        std::swap(_size, swp._size);
        std::swap(_alloc, swp._alloc);
        std::swap(_cmp, swp._cmp);
    }

// ------------------------------ private methods ------------------------------
private:
    constexpr rb_node   *_rb_node_create(key_T *key)
    {
        return new rb_node(nullptr, nullptr, nullptr, rb_red, key);
    }
    
// -----------------------------------------------------------------------------
    constexpr rb_node   *_rb_grandparent(rb_node *node) const noexcept
    {
        return node->parent->parent;
    }

// -----------------------------------------------------------------------------
    constexpr rb_node   *_rb_uncle(rb_node *node) const noexcept
    {
        rb_node   *gp;

        gp = _rb_grandparent(node);
        if (node->parent == gp->left)
            return gp->left;
        return gp->left;
    }

// -----------------------------------------------------------------------------
    constexpr rb_node   *_rb_rotate_right(rb_node *node) noexcept
    {
        rb_node *pivot = node->left;
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

// -----------------------------------------------------------------------------
    constexpr rb_node   *_rb_rotate_left(rb_node *node) noexcept
    {
        rb_node *pivot = node->right;
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

// -----------------------------------------------------------------------------
    constexpr rb_node   *_rb_insert(
            rb_node *hint,
            rb_node *node,
            bool &was_inserted
        ) noexcept
    {
        rb_node *parent = _bst_find_parent(hint, node->key);
        if (parent != nullptr and compare(node->key, parent->key) == 0)
        {
            delete node->key;
            delete node;
            was_inserted = false;
            return parent;
        }
        else if (parent == nullptr)
            _bst_insert(_root, node);
        else
            _bst_insert(parent, node);
        _rb_balance_after_insert(node);
        return node;
    }

// -----------------------------------------------------------------------------
    constexpr void  _bst_insert(rb_node *root, rb_node *node)
    {
        while (true)
        {
            if (compare(node->key, root->key) < 0)
            {
                if (root->left == nullptr)
                {
                    root->left = node;
                    node->parent = root;
                    return ;
                }
                root = root->left;
            }
            else
            {
                if (root->right == nullptr)
                {
                    root->right = node;
                    node->parent = root;
                    return ;
                }
                root = root->right;
            }
        }
    }

// -----------------------------------------------------------------------------
    constexpr rb_node   *_bst_find(const key_T *value, rb_node *root) const
    {
        if (root == nullptr)
            return nullptr;
        while (true)
        {
            int cmp = compare(value, root->key);
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

// -----------------------------------------------------------------------------
    constexpr rb_node   *_bst_find_parent(rb_node *root, const key_T *value)
    {
        if (root == NULL)
            return NULL;
        while (true)
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

// -----------------------------------------------------------------------------
    constexpr rb_node   *_rb_recolor(rb_node *node) noexcept
    {
        rb_node *gp = _rb_grandparent(node);
        gp->color = rb_red;
        gp->left->color = rb_black;
        gp->right->color = rb_black;
        return gp;
        }

// -----------------------------------------------------------------------------
    constexpr rb_node   *_rb_insert_case_1(rb_node *node, rb_node *gp) noexcept
    {
        if (gp->right and gp->right->color == rb_red)
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

// -----------------------------------------------------------------------------
    constexpr rb_node   *_rb_insert_case_2(rb_node *node, rb_node *gp) noexcept
    {
        if (gp->left and gp->left->color == rb_red)
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

// -----------------------------------------------------------------------------
    constexpr void      _rb_balance_after_insert(rb_node *node) noexcept
    {
        while (node != _root and node->color == rb_red)
        {
            if (node->parent->color == rb_black)
                break ;
            rb_node *gp = _rb_grandparent(node);
            if (gp == nullptr)
                break ;
            if (node->parent == gp->left) // left cases
                node = _rb_insert_case_1(node, gp);
            else // right cases
                node = _rb_insert_case_2(node, gp);
        }
        _root->color = rb_black;
    }

// -----------------------------------------------------------------------------
    static constexpr rb_node    *_bst_max(rb_node *node)
    {
        while (node->right != nullptr)
            node = node->right;
        return node;
    }

// -----------------------------------------------------------------------------
    static constexpr rb_node    *_bst_min(rb_node *node)
    {
        while (node->left != nullptr)
            node = node->left;
        return node;
    }

// -----------------------------------------------------------------------------
    constexpr rb_node   *_bst_remove(rb_node *node)
    {
        if (node->left == nullptr and node->right == nullptr)
        // case 1
            return node;
        if (node->left == nullptr or node->right == nullptr)
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
            delete node;
            return nullptr;
        }
        // case 3
        rb_node *accessor = _bst_max(node->left);
        node->key = accessor->key;
        return _bst_remove(accessor);
    }

// -----------------------------------------------------------------------------
    constexpr bool  _rb_is_black_children(rb_node *node) const noexcept
    {
        return (node->left == nullptr or node->left->color == rb_black)
            and (node->right == nullptr or node->right->color == rb_black);
    }

// -----------------------------------------------------------------------------
    constexpr void  _rb_fix_double_black(rb_node *node) noexcept
    {
        if (node == _root)
            return ;
        if (node->parent->left == node)
            _rb_fix_double_black_left(node);
        else
            _rb_fix_double_black_right(node);
    }

// -----------------------------------------------------------------------------
    constexpr void  _rb_fix_double_black_left(rb_node *node) noexcept
    {
        if (node == _root)
        // case 2
            return ;
        rb_node *parent = node->parent;
        rb_node *sibling = parent->right;
        if (sibling->color == rb_black and _rb_is_black_children(sibling))
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
        else if (sibling->right == nullptr or sibling->right->color == rb_black)
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

// -----------------------------------------------------------------------------
    constexpr void  _rb_fix_double_black_right(rb_node *node) noexcept
    {
        if (node == _root)
            // case 2
            return ;
        rb_node *parent = node->parent;
        rb_node *sibling = parent->left;
        if (sibling->color == rb_black and _rb_is_black_children(sibling))
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
        else if (sibling->left == nullptr or sibling->left->color == rb_black)
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

// -----------------------------------------------------------------------------
    constexpr void  _rb_remove_node(rb_node *rm)
    {
        if (_root == nullptr)
            return ;
        delete rm->key;
        rb_node *leaf = _bst_remove(rm);
        if (leaf == nullptr)
            return ;
        if (leaf == _root)
        {
            if (leaf->left)
                _root = leaf->left;
            else if (leaf->right)
                _root = leaf->right;
            else
            {
                delete _root;
                _root = nullptr;
                return ;
            }
            delete leaf;
            _root->parent = nullptr;
            _root->color = rb_black;
            return ;
        }
        if (leaf->color != rb_red)
            _rb_fix_double_black(leaf);
        if (leaf->parent->left == leaf)
            leaf->parent->left = nullptr;
        else
            leaf->parent->right = nullptr;
        delete leaf;
    }

// -----------------------------------------------------------------------------
    constexpr rb_node   *_rb_remove(const key_T *value)
    {
        if (_root == nullptr)
            return nullptr;
        rb_node *rm = _bst_find(value, _root);
        if (rm == nullptr)
            return nullptr;
        rb_node *ret;
        if (rm == _end)
            ret = nullptr;
        else
            ret = _bst_next(rm);
        _rb_remove_node(rm);
        --_size;
        return ret;
    }

// -----------------------------------------------------------------------------
    static constexpr rb_node    *_bst_next(rb_node *node)
    {
        if (node->right)
            return _bst_min(node->right);
        while (node->parent and node->parent->right == node)
            node = node->parent;
        if (LIKELY(node->parent != nullptr))
            return node->parent;
        return nullptr;
    }

// -----------------------------------------------------------------------------
    static constexpr rb_node    *_bst_prev(rb_node *node)
    {
        if (node->left)
            return _bst_max(node->left);
        while (node->parent and node->parent->left == node)
            node = node->parent;
        if (LIKELY(node->parent != nullptr))
            return node->parent;
        return nullptr;
    }

// -----------------------------------------------------------------------------
    constexpr rb_node   *_rb_copy(rb_node *root)
    {
        AUTO(key_T *) node_ptr = new key_T(*root->key);
        rb_node *new_root = _rb_node_create(node_ptr);
        new_root->color = root->color;
        if (root->left)
            new_root->left = _rb_copy(root->left);
        if (root->right)
            new_root->right = _rb_copy(root->right);
        return new_root;
    }

// -----------------------------------------------------------------------------
    constexpr void      _rb_destroy(rb_node *root)
    {
        delete root->key;
        if (root->left)
            _rb_destroy(root->left);
        if (root->right)
            _rb_destroy(root->right);
        delete root;
    }

// -----------------------------------------------------------------------------
    constexpr rb_node   *_bst_lower_bound(const key_T *value) const noexcept
    {
        rb_node *root = _root;
        rb_node *ret = root;
        while (root != nullptr)
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
            return nullptr;
        return ret;
    }

// -----------------------------------------------------------------------------
    constexpr rb_node   *_bst_upper_bound(const key_T *value) const noexcept
    {
        rb_node *root = _root;
        rb_node *ret = root;
        while (root != nullptr)
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
            return nullptr;
        return ret;
    }

// -----------------------------------------------------------------------------
    constexpr bool  _dfs_compare(rb_node *rt1, rb_node *rt2) const
    {
        unsigned int left = 1;
        unsigned int right = 1;

        if (compare(rt1->key, rt2->key) != 0)
            return false;
        if (rt1->left and rt2->left)
            left = _dfs_compare(rt1->left, rt2->left);
        else if (rt1->left or rt2->left)
            return false;
        if (rt1->right and rt2->right)
            right = _dfs_compare(rt1->right, rt2->right);
        else if (rt1->right or rt2->right)
            return false;
        return left and right;
    }

// -----------------------------------------------------------------------------
    constexpr int   _bst_compare(
            rb_node *rt1,
            rb_node *rt2,
            rb_node *end1,
            rb_node *end2
        ) const
    {
        while (true)
        {
            int cmp = compare(rt1->key, rt2->key);
            if (cmp)
                return cmp;
            if (rt1 == end1 or rt2 == end2)
            {
                if (rt1 == end1 and rt2 == end2)
                    return 0;
                if (rt2 == end2)
                    return 1;
                return -1;
            }
            rt1 = _bst_next(rt1);
            rt2 = _bst_next(rt2);
        }
    }

// -----------------------------------------------------------------------------
    constexpr bool  _equal(const rb_tree &tree2) const
    {
        if (_size != tree2._size)
            return false;
        else if (UNLIKELY(_size == 0 and tree2._size == 0))
            return true;
        return _dfs_compare(_root, tree2._root);
    }

// -----------------------------------------------------------------------------
    constexpr int   _compare(const rb_tree &tree2) const
    {
        if (_size != tree2._size)
            return tlucanti::sign(_size - tree2._size);
        else if (UNLIKELY(_size == 0 and tree2._size == 0))
            return 0;
        return _bst_compare(_root, tree2._root, _end, tree2._end);
    }

// -------------------------- lexicographical compare --------------------------
public:
    friend constexpr bool   operator ==(
            const rb_tree &tree1,
            const rb_tree &tree2
        )
    {
        return tree1._equal(tree2);
    }

// -----------------------------------------------------------------------------
    friend constexpr bool   operator !=(
            const rb_tree &tree1,
            const rb_tree &tree2
        )
    {
        return not tree1._equal(tree2);
    }

// -----------------------------------------------------------------------------
    friend constexpr bool   operator >(
            const rb_tree &tree1,
            const rb_tree &tree2
        )
    {
        return tree1._compare(tree2) > 0;
    }

// -----------------------------------------------------------------------------
    friend constexpr bool   operator >=(
            const rb_tree &tree1,
            const rb_tree &tree2
        )
    {
        return tree1._compare(tree2) >= 0;
    }

// -----------------------------------------------------------------------------
    friend constexpr bool   operator <(
            const rb_tree &tree1,
            const rb_tree &tree2
        )
    {
        return tree1._compare(tree2) < 0;
    }

// -----------------------------------------------------------------------------
    friend constexpr bool   operator <=(
            const rb_tree &tree1,
            const rb_tree &tree2
        )
    {
        return tree1._compare(tree2) <= 0;
    }

}; /* rb_tree */

TLU_NAMESPACE_END

#endif /* RB_TREE_HPP */
