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

/** @brief Replaces the subtree rooted at u with the subtree rooted at v.
 * If u->parent is NULL, then v becomes root; otherwise
 * v is attached to u->parent in place of u. */
static void
	rb_transplant(t_rbtree *tree, t_rbnode *u, t_rbnode *v)
{
	if (u->parent == NULL)
		tree->root = v;
	else if (u == u->parent->left)
		u->parent->left = v;
	else
		u->parent->right = v;
	if (v != NULL)
		v->parent = u->parent;
}

/** @brief Rebalance after deleting a black node. We track x’s parent separately
 * because if x is NULL, x->parent would be invalid. */
static void
	rb_delete_fixup(t_rbtree *tree, t_rbnode *x, t_rbnode *x_parent)
{
	while ((x == NULL || x->color == 1) && x != tree->root)
	{
		/* Distinguish whether x is a left child or right child. */
		if (x_parent && x == x_parent->left)
		{
			t_rbnode *w = x_parent->right; /* w is x’s sibling. */
			/* Case 1: w is red => recolor & rotate. */
			if (w && w->color == 0)
			{
				w->color = 1;
				x_parent->color = 0;
				rb_rotate_left(tree, x_parent);
				w = x_parent->right;
			}
			/* Case 2: w is black and both w’s children are black => recolor. */
			if ((w == NULL) ||
					((w->left == NULL || w->left->color == 1) &&
					 (w->right == NULL || w->right->color == 1)))
			{
				if (w) w->color = 0;
				x = x_parent;
				x_parent = x_parent->parent;
			}
			else
			{
				/* Case 3: w is black, w->left is red, w->right is black => rotate. */
				if (w->right == NULL || w->right->color == 1)
				{
					if (w->left)
						w->left->color = 1;
					w->color = 0;
					rb_rotate_right(tree, w);
					w = x_parent->right;
				}
				/* Case 4: w is black, w->right is red => recolor & rotate. */
				if (w)
					w->color = x_parent->color;
				x_parent->color = 1;
				if (w && w->right)
					w->right->color = 1;
				rb_rotate_left(tree, x_parent);
				x = tree->root; 
				break;
			}
		}
		else
		{
			/* Symmetric “mirror” case where x is a right child. */
			t_rbnode *w = x_parent->left;
			if (w && w->color == 0)
			{
				w->color = 1;
				x_parent->color = 0;
				rb_rotate_right(tree, x_parent);
				w = x_parent->left;
			}
			if ((w == NULL) ||
					((w->right == NULL || w->right->color == 1) &&
					 (w->left == NULL || w->left->color == 1)))
			{
				if (w) w->color = 0;
				x = x_parent;
				x_parent = x_parent->parent;
			}
			else
			{
				if (w->left == NULL || w->left->color == 1)
				{
					if (w->right)
						w->right->color = 1;
					w->color = 0;
					rb_rotate_left(tree, w);
					w = x_parent->left;
				}
				if (w)
					w->color = x_parent->color;
				x_parent->color = 1;
				if (w && w->left)
					w->left->color = 1;
				rb_rotate_right(tree, x_parent);
				x = tree->root;
				break;
			}
		}
	}
	/* Finally, color x black so root is definitely black. */
	if (x)
		x->color = 1;
}

int
	rb_delete(t_rbtree *tree, const void *key)
{
	/* 1) Find the node to delete (z) by comparing keys. */
	t_rbnode *z = tree->root;
	while (z != NULL)
	{
		int cmp = tree->key_cmp(key, z->key);
		if (cmp < 0)
			z = z->left;
		else if (cmp > 0)
			z = z->right;
		else
			break;  /* Found the node matching 'key' */
	}
	if (z == NULL)
		return 0;   /* Key not found */

	/* Keep track of certain pointers and the original color to decide if fixup is needed. */
	t_rbnode *y = z;
	uint8_t   y_original_color = y->color;
	t_rbnode *x = NULL;
	t_rbnode *x_parent = NULL;

	/* 2) Standard BST deletion logic, with a twist for RB trees. */
	if (z->left == NULL)
	{
		/* Case: z has at most one (right) child. */
		x = z->right;
		x_parent = z->parent;
		rb_transplant(tree, z, z->right);
	}
	else if (z->right == NULL)
	{
		/* Case: z has a left child but no right child. */
		x = z->left;
		x_parent = z->parent;
		rb_transplant(tree, z, z->left);
	}
	else
	{
		/* Case: z has two children, so find its successor y in z->right's subtree. */
		y = rb_minimum(z->right);  /* The leftmost node in z->right subtree is successor. */
		y_original_color = y->color;
		x = y->right;

		if (y->parent == z)
		{
			/* If the successor is directly z->right, the new parent is y. */
			x_parent = y;
		}
		else
		{
			/* Otherwise, splice y out from its current location first. */
			rb_transplant(tree, y, y->right);
			y->right = z->right;
			y->right->parent = y;
			x_parent = y->parent;
		}
		/* Now, replace z with y. */
		rb_transplant(tree, z, y);
		y->left = z->left;
		y->left->parent = y;
		/* The successor y acquires z's old color, preserving the black-height. */
		y->color = z->color;
	}

	/* 3) Destroy the key/data for the node being physically removed. */
	tree->key_destroy(z->key);
	tree->data_destroy(z->data);
	free(z);

	/* 4) If we removed a black node, fix the red‐black properties. */
	if (y_original_color == 1 && (x != NULL || x_parent != NULL))
	{
		/*
		 * Even if x == NULL, we still need to pass x and x_parent
		 * into fixup to handle the "double black" scenario.
		 */
		rb_delete_fixup(tree, x, x_parent);
	}
	return (1);  /* Key found & deletion succeeded */
}
