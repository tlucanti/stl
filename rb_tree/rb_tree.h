
#ifndef RB_TREE_H
# define RB_TREE_H

# ifdef __cplusplus
extern "C" {
# endif /* __cplusplus */

# include <assert.h>
# include <stddef.h>
# include "defs.h"

# ifndef __cplusplus
#  ifdef __LINUX__
#   include <malloc.h>
#  else
#   include <stdlib.h>
#  endif
# endif

typedef enum e_Rb_colors
{
	_Rb_Red,
	_Rb_Black
}	_Rb_colors;

typedef struct s_Rb_node
{
	struct s_Rb_node	*parent;
	struct s_Rb_node	*left;
	struct s_Rb_node	*right;

	_Rb_colors			color;
	void                *key;
}	_Rb_node;

typedef struct s_rb_node
{
    _Rb_node    *node;
}   rb_node;

typedef struct s_rb_tree
{
    rb_node     root;
    rb_node     begin;
    rb_node     end;
    size_t      size;
}   rb_tree;


typedef int (*compare_fun)(void *, void *) WUR;
typedef void *(*copy_fun)(void *) WUR;
typedef void (*del_fun)(void *);

void    *rb_get_key(rb_node node) WUR NOEXCEPT;
rb_node rb_insert(rb_tree *root, void *key, compare_fun compare, int *was_inserted) NOEXCEPT;
rb_node rb_insert_hint(rb_tree *root, rb_node hint, void *key, compare_fun compare, int *was_inserted) NOEXCEPT;
rb_node rb_find(rb_tree *root, void *key, compare_fun compare) WUR NOEXCEPT;
rb_node rb_remove(rb_tree *root, void *key, compare_fun compare) NOEXCEPT;
rb_node rb_next(rb_node node) WUR NOEXCEPT;
rb_node rb_prev(rb_node node) WUR NOEXCEPT;
rb_tree rb_copy(rb_tree *root, copy_fun copy) WUR NOEXCEPT;
void    rb_destroy(rb_tree *root, del_fun del) NOEXCEPT;
rb_node rb_lower_bound(rb_tree *root, void *value, compare_fun cmp) WUR NOEXCEPT;
rb_node rb_upper_bound(rb_tree *root, void *value, compare_fun cmp) WUR NOEXCEPT;
int     rb_equal(rb_tree *tree1, rb_tree *tree2, compare_fun cmp) WUR NOEXCEPT;
int     rb_compare(rb_tree *tree1, rb_tree *tree2, compare_fun cmp) WUR NOEXCEPT;

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* RB_TREE_H */
