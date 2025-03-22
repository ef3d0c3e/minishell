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

void insert_repair(t_rbtree *tree, t_rbnode *node)
{
    t_rbnode *uncle;

    /* Loop while node is not root and its parent is red (0) */
    while (node != tree->root && node->parent->color == 0) {
        t_rbnode *parent = node->parent;
        t_rbnode *gparent = parent->parent;

        if (gparent == NULL)
            break;  /* Should never happen if tree is valid */

        if (parent == gparent->left) {
            uncle = gparent->right;
            /* If uncle exists and is red */
            if (uncle != NULL && uncle->color == 0) {
                /* Case 1: recolor and move up the tree */
                parent->color = 1;
                uncle->color = 1;
                gparent->color = 0;
                node = gparent;
            } else {
                /* Case 2: uncle is black (or NULL) */
                if (node == parent->right) {
                    /* Left-Right case */
                    node = parent;
                    rotate_left(tree, node);
                    parent = node->parent;  /* update parent pointer */
                    gparent = parent->parent;
                }
                /* Left-Left case */
                parent->color = 1;
                gparent->color = 0;
                rotate_right(tree, gparent);
                /* Tree is balanced; exit loop */
                break;
            }
        } else {
            /* parent is right child of gparent */
            uncle = gparent->left;
            if (uncle != NULL && uncle->color == 0) {
                /* Case 1: recolor and move up the tree */
                parent->color = 1;
                uncle->color = 1;
                gparent->color = 0;
                node = gparent;
            } else {
                /* Case 2: uncle is black (or NULL) */
                if (node == parent->left) {
                    /* Right-Left case */
                    node = parent;
                    rotate_right(tree, node);
                    parent = node->parent;
                    gparent = parent->parent;
                }
                /* Right-Right case */
                parent->color = 1;
                gparent->color = 0;
                rotate_left(tree, gparent);
                /* Tree is balanced; exit loop */
                break;
            }
        }
    }
    tree->root->color = 1;
}

t_rbnode
	*rb_insert(t_rbtree *tree, void *key, void *data)
{
	t_rbnode	*current;
	t_rbnode	*parent;

	/* do a binary search to find where it should be */
	current = tree->root;
	parent = NULL;

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

	if (parent == NULL)
		tree->root = current;
	else if (tree->key_cmp(key, parent->key) < 0)
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
	if (current->parent != NULL && current->parent->color == 0) {
		/* insertion into 3-children cluster (parent node is RED) */
		/* insertion into 4-children cluster (parent node is RED) */
		printf("Insert repair called\n");
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
	tree->root->color = 1;

	return (current);
}
