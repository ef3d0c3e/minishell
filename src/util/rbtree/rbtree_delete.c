/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rbtree_delete.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgamba <linogamba@pundalik.org>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 11:59:40 by lgamba            #+#    #+#             */
/*   Updated: 2025/03/17 11:59:41 by lgamba           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "../util.h"

	static inline t_rbnode
*rb_successor(t_rbtree *tree, t_rbnode *node)
{
	t_rbnode	*p;

	p = node->right;
	if (p != NULL)
	{
		while (p->left)
			p = p->left;
	}
	else
	{
		p = node->parent;
		while (p->right)
		{
			node = p;
			p = p->parent;
		}
		if (p == tree->root)
			return (NULL);
	}
	return (p);
}

	void
delete_repair(t_rbtree *tree, t_rbnode *current)
{
	t_rbnode *sibling;

	do {
		if (current == current->parent->left)
		{
			sibling = current->parent->right;

			if (sibling->color == 0)
			{
				/* perform an adjustment (3-children parent cluster has two representations) */
				sibling->color = 1;
				current->parent->color = 0;
				rb_rotate_left(tree, current->parent);
				sibling = current->parent->right;
			}

			/* sibling node must be 1 now */

			if (sibling->right->color == 1 && sibling->left->color == 1) {
				/* 2-children sibling cluster, fuse by recoloring */
				sibling->color = 0;
				if (current->parent->color == 0) { /* 3/4-children parent cluster */
					current->parent->color = 1;
					break; /* goto break */
				} else { /* 2-children parent cluster */
					current = current->parent; /* goto loop */
				}
			}
			else
			{
				/* 3/4-children sibling cluster */

				/* perform an adjustment (3-children sibling cluster has two representations) */
				if (sibling->right->color == 1) {
					sibling->left->color = 1;
					sibling->color = 0;
					rb_rotate_right(tree, sibling);
					sibling = current->parent->right;
				}

				/* transfer by rotation and recoloring */
				sibling->color = current->parent->color;
				current->parent->color = 1;
				sibling->right->color = 1;
				rb_rotate_left(tree, current->parent);
				break; /* goto break */
			}
		}
		else
		{
			sibling = current->parent->left;

			if (sibling->color == 0) {
				/* perform an adjustment (3-children parent cluster has two representations) */
				sibling->color = 1;
				current->parent->color = 0;
				rb_rotate_right(tree, current->parent);
				sibling = current->parent->left;
			}

			/* sibling node must be 1 now */

			if (sibling->right->color == 1 && sibling->left->color == 1) {
				/* 2-children sibling cluster, fuse by recoloring */
				sibling->color = 0;
				if (current->parent->color == 0) { /* 3/4-children parent cluster */
					current->parent->color = 1;
					break; /* goto break */
				} else { /* 2-children parent cluster */
					current = current->parent; /* goto loop */
				}
			} else {
				/* 3/4-children sibling cluster */

				/* perform an adjustment (3-children sibling cluster has two representations) */
				if (sibling->left->color == 1) {
					sibling->right->color = 1;
					sibling->color = 0;
					rb_rotate_left(tree, sibling);
					sibling = current->parent->left;
				}

				/* transfer by rotation and recoloring */
				sibling->color = current->parent->color;
				current->parent->color = 1;
				sibling->left->color = 1;
				rb_rotate_right(tree, current->parent);
				break; /* goto break */
			}
		}
	} while (current != tree->root->left);
}


	int
rb_delete(t_rbtree *tree, const void *key)
{
	t_rbnode *target, *child;
	t_rbnode	*node;

	node = tree->root;

	if (node->left == NULL || node->right == NULL)
		target = node;
	else
	{
		target = rb_successor(tree, node); /* node->right must not be NIL, thus move down */
		node->data = target->data; /* data swapped */
	}

	child = target->right;
	if (target->left)
		child = target->left;
	if (target->color == 1)
	{
		if (child->color == 0)
		{
			/* deletion from 3-children cluster (1 target node, 0 child node) */
			child->color = 1;
		}
		else if (target == tree->root->left)
		{
			/* deletion from 2-children root cluster (1 target node, 1 child node) */
		}
		else
		{
			/* deletion from 2-children cluster (1 target node, ...) */
			delete_repair(tree, target);
		}
	}
	else
	{
		/* deletion from 4-children cluster (0 target node) */
		/* deletion from 3-children cluster (0 target node) */
	}

	if (child != NULL)
		child->parent = target->parent;

	if (target == target->parent->left)
		target->parent->left = child;
	else
		target->parent->right = child;

	free(target);

	/* keep or discard data */
	tree->key_destroy(node->key);
	tree->data_destroy(node->data);
	return (1);
}
