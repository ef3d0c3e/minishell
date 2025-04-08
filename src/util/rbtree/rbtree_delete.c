/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rbtree_delete.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgamba <linogamba@pundalik.org>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 11:59:40 by lgamba            #+#    #+#             */
/*   Updated: 2025/03/17 11:59:41 by lgamba           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "../util.h"
#include <stdint.h>

/** @brief Transplant a node with <= 1 children */
static void
	rb_transplant_single(
			t_rbtree *tree,
			t_rbnode *z,
			t_rbnode **x,
			t_rbnode **x_parent)
{
	if (!z->left)
	{
		*x = z->right;
		*x_parent = z->parent;
		rb_transplant(tree, z, z->right);
	}
	else if (!z->right)
	{
		*x = z->left;
		*x_parent = z->parent;
		rb_transplant(tree, z, z->left);
	}
}

/** @brief Transplant a node with 2 children */
static uint8_t
	rb_transplant_multiple(
			t_rbtree *tree,
			t_rbnode *z,
			t_rbnode **x,
			t_rbnode **x_parent)
{
	t_rbnode	*y;
	uint8_t		color;

	y = z;
	y = rb_minimum(z->right);
	color = y->color;
	(*x) = y->right;
	if (y->parent == z)
		(*x_parent) = y;
	else
	{
		rb_transplant(tree, y, y->right);
		y->right = z->right;
		y->right->parent = y;
		(*x_parent) = y->parent;
	}
	rb_transplant(tree, z, y);
	y->left = z->left;
	y->left->parent = y;
	y->color = z->color;
	return (color);
}

int
	rb_delete(t_rbtree *tree, const void *key)
{
	t_rbnode		*x;
	t_rbnode		*x_parent;
	uint8_t			original_color;
	t_rbnode *const	z = rb_find_node(tree, key);

	if (!z)
		return (0);
	x = NULL;
	original_color = z->color;
	x_parent = NULL;
	if (!z->left || !z->right)
		rb_transplant_single(tree, z, &x, &x_parent);
	else
		original_color = rb_transplant_multiple(tree, z, &x, &x_parent);
	tree->key_destroy(z->key);
	tree->data_destroy(z->data);
	free(z);
	if (original_color == 1 && (x != NULL || x_parent != NULL))
		rb_delete_fixup(tree, x, x_parent);
	++tree->size;
	return (1);
}
