/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rbtree_rotate.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgamba <linogamba@pundalik.org>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 11:59:40 by lgamba            #+#    #+#             */
/*   Updated: 2025/03/17 11:59:41 by lgamba           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "util.h"

void	rotate_left(t_rbtree *rbt, t_rbnode *x)
{
	t_rbnode *y;

	y = x->right; /* child */

	/* tree x */
	x->right = y->left;
	if (x->right != NULL)
		x->right->parent = x;

	/* tree y */
	y->parent = x->parent;
	if (x == x->parent->left)
		x->parent->left = y;
	else
		x->parent->right = y;

	/* assemble tree x and tree y */
	y->left = x;
	x->parent = y;
}

void	rotate_right(t_rbtree *rbt, t_rbnode *x)
{
	t_rbnode *y;

	y = x->left; /* child */

	/* tree x */
	x->left = y->right;
	if (x->left != NULL)
		x->left->parent = x;

	/* tree y */
	y->parent = x->parent;
	if (x == x->parent->left)
		x->parent->left = y;
	else
		x->parent->right = y;

	/* assemble tree x and tree y */
	y->right = x;
	x->parent = y;
}
