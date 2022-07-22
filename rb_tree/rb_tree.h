
#ifndef _RB_TREE_H
# define _RB_TREE_H

# ifdef __cplusplus
extern "C" {
# endif /* __cplusplus */

# include <assert.h>
# include <stddef.h>

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

typedef struct s_rb_tree
{
    _Rb_node *root;
}   rb_tree;

typedef int (*compare_fun)(void *, void *);

void *rb_insert(rb_tree *root, void *key, compare_fun compare);
void *rb_find(rb_tree *root, void *key, compare_fun compare);
void *rb_remove(rb_tree *root, void *key, compare_fun compare);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* _RB_TREE_H */
