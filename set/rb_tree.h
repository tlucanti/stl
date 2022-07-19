
#ifndef RB_TREE_H
# define RB_TREE_H

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
} rb_tree;

void *rb_insert(rb_tree *root, void *key, int (*compare)(void *, void *));
void *rb_find(rb_tree *root, void *key, int (*compare)(void *, void *));

extern void _print_rb_tree(_Rb_node *tree, const char *msg);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* RB_TREE_H */
