/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rbtree_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgamba <linogamba@pundalik.org>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 11:59:40 by lgamba            #+#    #+#             */
/*   Updated: 2025/03/17 11:59:41 by lgamba           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "../util.h"

static inline t_rbnode
	*find_impl(t_rbtree *tree, t_rbnode *root, const void *key)
{
	const int	cmp = tree->key_cmp(root->key, key);

	if (!cmp)
		return (root);
	if (cmp > 0)
	{
		if (!root->left)
			return (NULL);
		return find_impl(tree, root->left, key);
	}
	if (!root->right)
		return (NULL);
	return find_impl(tree, root->right, key);
}

t_rbnode
	*rb_find_node(t_rbtree *tree, const void *key)
{
	if (!tree->root)
		return (NULL);
	return (find_impl(tree, tree->root, key));
}

void
*rb_find(t_rbtree *tree, const void *key)
{
	t_rbnode	*node;

	node = rb_find_node(tree, key);
	if (!node)
		return (NULL);
	return (node->data);
}
