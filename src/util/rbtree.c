/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rbtree.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgamba <linogamba@pundalik.org>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 11:59:40 by lgamba            #+#    #+#             */
/*   Updated: 2025/03/17 11:59:41 by lgamba           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "util.h"

t_rbtree
	rb_new(
	int	(*key_cmp)(const void *lhs, const void *rhs),
	void (*key_destroy)(void *key),
	void (*data_destroy)(void *data))
{
	t_rbtree	tree;

	tree.key_cmp = key_cmp;
	tree.key_destroy = key_destroy;
	tree.data_destroy = data_destroy;

	tree.root = NULL;
	//tree.root->parent = NULL;
	//tree.root->left = NULL;
	//tree.root->right = NULL;
	//tree.root->color = 1;
	//tree.root->key = NULL;
	//tree.root->data = NULL;

	return (tree);
}

static inline void
	free_node(t_rbtree *tree, t_rbnode *node)
{
	if (!node)
		return ;
	tree->data_destroy(node->data);
	tree->key_destroy(node->key);
	free_node(tree, node->left);
	free_node(tree, node->right);
	free(node);
}

void
	rb_free(t_rbtree *tree)
{
	free_node(tree, tree->root);
}

void
	rb_apply_impl(t_rbtree *tree, t_rbnode *root, void(*fn)(size_t depth, t_rbnode *node), size_t depth)
{
	fn(depth, root);
	if (root->left)
		rb_apply_impl(tree, root->left, fn, depth + 1);
	if (root->right)
		rb_apply_impl(tree, root->right, fn, depth + 1);
}

void
	rb_apply(t_rbtree *tree, void(*fn)(size_t depth, t_rbnode *node))
{
	rb_apply_impl(tree, tree->root, fn, 0);
}



#include <string.h>
#include <stdio.h>

void	print_fn(size_t depth, t_rbnode *node)
{
	for (size_t i = 0; i < depth; ++i)
		printf(" | ");
	printf("%s: %s\n", node->key, node->data);
}
int main()
{
	t_rbtree	env;

	env = rb_new(
		(void*)strcmp,
		(void*)free,
		(void*)free);

	rb_insert(&env, strdup("A"), strdup("Var"));
	rb_insert(&env, strdup("B"), strdup("World"));
	rb_insert(&env, strdup("C"), strdup("Test"));
	rb_insert(&env, strdup("D"), strdup("Test"));
	rb_insert(&env, strdup("E"), strdup("Test"));
	rb_insert(&env, strdup("Z"), strdup("Last"));
	rb_apply(&env, print_fn);
	rb_free(&env);
}
