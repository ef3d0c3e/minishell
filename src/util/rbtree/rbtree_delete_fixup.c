/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rbtree_delete_fixup.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgamba <linogamba@pundalik.org>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 11:59:40 by lgamba            #+#    #+#             */
/*   Updated: 2025/03/17 11:59:41 by lgamba           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "../util.h"

static int
	rb_delete_fixup_left2(t_rbtree *tree, t_rbnode **x, t_rbnode **x_parent,
			t_rbnode *w)
{
	if (w->right == NULL || w->right->color == 1)
	{
		if (w->left)
			w->left->color = 1;
		w->color = 0;
		rb_rotate_right(tree, w);
		w = (*x_parent)->right;
	}
	if (w)
		w->color = (*x_parent)->color;
	(*x_parent)->color = 1;
	if (w && w->right)
		w->right->color = 1;
	rb_rotate_left(tree, *x_parent);
	*x = tree->root;
	return (0);
}

static int
	rb_delete_fixup_left1(t_rbtree *tree, t_rbnode **x, t_rbnode **x_parent)
{
	t_rbnode	*w;

	w = (*x_parent)->right;
	if (w && w->color == 0)
	{
		w->color = 1;
		(*x_parent)->color = 0;
		rb_rotate_left(tree, *x_parent);
		w = (*x_parent)->right;
	}
	if ((w == NULL) || ((w->left == NULL || w->left->color == 1)
			&& (w->right == NULL || w->right->color == 1)))
	{
		if (w)
			w->color = 0;
		*x = *x_parent;
		(*x_parent) = (*x_parent)->parent;
	}
	else
		return (rb_delete_fixup_left2(tree, x, x_parent, w));
	return (1);
}

static int
	rb_delete_fixup_right2(t_rbtree *tree, t_rbnode **x, t_rbnode **x_parent,
			t_rbnode *w)
{
	if (w->left == NULL || w->left->color == 1)
	{
		if (w->right)
			w->right->color = 1;
		w->color = 0;
		rb_rotate_left(tree, w);
		w = (*x_parent)->left;
	}
	if (w)
		w->color = (*x_parent)->color;
	(*x_parent)->color = 1;
	if (w && w->left)
		w->left->color = 1;
	rb_rotate_right(tree, *x_parent);
	*x = tree->root;
	return (0);
}

static int
	rb_delete_fixup_right1(t_rbtree *tree, t_rbnode **x, t_rbnode **x_parent)
{
	t_rbnode	*w;

	w = (*x_parent)->left;
	if (w && w->color == 0)
	{
		w->color = 1;
		(*x_parent)->color = 0;
		rb_rotate_right(tree, *x_parent);
		w = (*x_parent)->left;
	}
	if ((w == NULL) || ((w->right == NULL || w->right->color == 1)
			&& (w->left == NULL || w->left->color == 1)))
	{
		if (w)
			w->color = 0;
		x = x_parent;
		(*x_parent) = (*x_parent)->parent;
	}
	else
		return (rb_delete_fixup_right2(tree, x, x_parent, w));
	return (1);
}

void
	rb_delete_fixup(t_rbtree *tree, t_rbnode *x, t_rbnode *x_parent)
{
	while ((x == NULL || x->color == 1) && x != tree->root)
	{
		if (x_parent && x == x_parent->left)
		{
			if (!rb_delete_fixup_left1(tree, &x, &x_parent))
				break ;
		}
		else
		{
			if (!rb_delete_fixup_right1(tree, &x, &x_parent))
				break ;
		}
	}
	if (x)
		x->color = 1;
}
