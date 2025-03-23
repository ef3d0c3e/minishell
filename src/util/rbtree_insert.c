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

static inline int
	insert_repair1(t_rbtree *tree, t_rbnode **node, t_rbnode **nodes)
{
	nodes[0] = nodes[2]->right;
	if (nodes[0] != NULL && nodes[0]->color == 0)
	{
		nodes[0]->color = 1;
		nodes[1]->color = 1;
		nodes[2]->color = 0;
		*node = nodes[2];
	}
	else
	{
		if (*node == nodes[1]->right)
		{
			*node = nodes[1];
			rb_rotate_left(tree, *node);
			nodes[1] = (*node)->parent;
			nodes[2] = nodes[1]->parent;
		}
		nodes[1]->color = 1;
		nodes[2]->color = 0;
		rb_rotate_right(tree, nodes[2]);
		return (1);
	}
	return (0);
}

static inline int
	insert_repair2(t_rbtree *tree, t_rbnode **node, t_rbnode **nodes)
{
	nodes[0] = nodes[2]->left;
	if (nodes[0] != NULL && nodes[0]->color == 0)
	{
		nodes[0]->color = 1;
		nodes[1]->color = 1;
		nodes[2]->color = 0;
		*node = nodes[2];
	}
	else
	{
		if (*node == nodes[1]->left)
		{
			*node = nodes[1];
			rb_rotate_right(tree, *node);
			nodes[1] = (*node)->parent;
			nodes[2] = nodes[1]->parent;
		}
		nodes[1]->color = 1;
		nodes[2]->color = 0;
		rb_rotate_left(tree, nodes[2]);
		return (1);
	}
	return (0);
}

static inline void	insert_repair(t_rbtree *tree, t_rbnode *node)
{
	t_rbnode	*nodes[3];

	while (node != tree->root && node->parent->color == 0)
	{
		nodes[1] = node->parent;
		nodes[2] = nodes[1]->parent;
		if (nodes[2] == NULL)
			break ;
		if (nodes[1] == nodes[2]->left)
		{
			if (insert_repair1(tree, &node, nodes))
				break ;
		}
		else
		{
			if (insert_repair2(tree, &node, nodes))
				break ;
		}
	}
	tree->root->color = 1;
}

static inline t_rbnode
	*insert_new(t_rbtree *tree, void *key, void *data, t_rbnode *parent)
{
	t_rbnode	*current;

	current = xmalloc(sizeof(t_rbnode));
	current->left = NULL;
	current->right = NULL;
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
	if (current->parent != NULL && current->parent->color == 0)
		insert_repair(tree, current);
	tree->root->color = 1;
	return (current);
}

t_rbnode	*rb_insert(t_rbtree *tree, void *key, void *data)
{
	t_rbnode	*current;
	t_rbnode	*parent;
	int			cmp;

	current = tree->root;
	parent = NULL;
	while (current != NULL)
	{
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
	return (insert_new(tree, key, data, parent));
}
