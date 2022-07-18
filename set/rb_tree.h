
#ifndef RB_TREE_H
# define RB_TREE_H

typedef enum e_Rb_colors
{
	Red,
	Black
}	_Rb_colors;

typedef struct s_Rb_node
{
	struct s_Rb_node	*parent;
	struct s_Rb_node	*left;
	struct s_Rb_node	*right;

	_Rb_colors			color;
	size_t				key;
}	_Rb_node;

#endif /* RB_TREE_H */
