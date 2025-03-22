/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rbtree_insert.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgamba <linogamba@pundalik.org>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 11:59:40 by lgamba            #+#    #+#             */
/*   Updated: 2025/03/17 11:59:41 by lgamba           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "util.h"
#include <stdio.h>

void	rotate_left(t_rbtree *rbt, t_rbnode *x);
void	rotate_right(t_rbtree *rbt, t_rbnode *x);

void
	insert_repair(t_rbtree *tree, t_rbnode *current)
{
	t_rbnode *uncle;

	do {
		/* current node is RED and parent node is RED */

		if (current->parent == current->parent->parent->left) {
			uncle = current->parent->parent->right;
			if (uncle->color == 0) {
				/* insertion into 4-children cluster */

				/* split */
				current->parent->color = 1;
				uncle->color = 1;

				/* send grandparent node up the tree */
				current = current->parent->parent; /* goto loop or break */
				current->color = 0;
			} else {
				/* insertion into 3-children cluster */

				/* equivalent BST */
				if (current == current->parent->right) {
					current = current->parent;
					rotate_left(tree, current);
				}

				/* 3-children cluster has two representations */
				current->parent->color = 1; /* thus goto break */
				current->parent->parent->color = 0;
				rotate_right(tree, current->parent->parent);
			}
		} else {
			uncle = current->parent->parent->left;
			if (uncle->color == 0) {
				/* insertion into 4-children cluster */

				/* split */
				current->parent->color = 1;
				uncle->color = 1;

				/* send grandparent node up the tree */
				current = current->parent->parent; /* goto loop or break */
				current->color = 0;
			} else {
				/* insertion into 3-children cluster */

				/* equivalent BST */
				if (current == current->parent->left) {
					current = current->parent;
					rotate_right(tree, current);
				}

				/* 3-children cluster has two representations */
				current->parent->color = 1; /* thus goto break */
				current->parent->parent->color = 0;
				rotate_left(tree, current->parent->parent);
			}
		}
	} while (current->parent->color == 0);
}

t_rbnode
	*rb_insert(t_rbtree *tree, void *key, void *data)
{
	t_rbnode *current, *parent;

	/* do a binary search to find where it should be */

	current = tree->root->left;
	parent = tree->root;

	while (current != NULL) {
		int cmp;
		cmp = tree->key_cmp(key, current->key);

		if (cmp == 0)
		{
			tree->key_destroy(current->key);
			tree->data_destroy(current->data);
			current->key = key;
			current->data = data;
			return (current);
		}

		parent = current;
		if (cmp < 0)
			current = current->left;
		else
			current = current->right;
	}

	/* replace the termination NIL pointer with the new node pointer */
	current = xmalloc(sizeof(t_rbnode));
	current->left = current->right = NULL;
	current->parent = parent;
	current->color = 0;
	current->key = key;
	current->data = data;

	if (parent == tree->root || tree->key_cmp(key, parent->key) < 0)
		parent->left = current;
	else
		parent->right = current;

	/*
	 * insertion into a red-black tree:
	 *   0-children root cluster (parent node is BLACK) becomes 2-children root cluster (new root node)
	 *     paint root node BLACK, and done
	 *   2-children cluster (parent node is BLACK) becomes 3-children cluster
	 *     done
	 *   3-children cluster (parent node is BLACK) becomes 4-children cluster
	 *     done
	 *   3-children cluster (parent node is RED) becomes 4-children cluster
	 *     rotate, and done
	 *   4-children cluster (parent node is RED) splits into 2-children cluster and 3-children cluster
	 *     split, and insert grandparent node into parent cluster
	 */
	if (current->parent->color == 0) {
		/* insertion into 3-children cluster (parent node is RED) */
		/* insertion into 4-children cluster (parent node is RED) */
		insert_repair(tree, current);
	} else {
		/* insertion into 0-children root cluster (parent node is BLACK) */
		/* insertion into 2-children cluster (parent node is BLACK) */
		/* insertion into 3-children cluster (parent node is BLACK) */
	}

	/*
	 * the root is always BLACK
	 * insertion into 0-children root cluster or insertion into 4-children root cluster require this recoloring
	 */
	tree->root->left->color = 1;

	return (current);
}
