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
#include "../util.h"

t_rbtree
	rb_new(
	int (*key_cmp)(const void *lhs, const void *rhs),
	void (*key_destroy)(void *key),
	void (*data_destroy)(void *data))
{
	t_rbtree	tree;

	tree.key_cmp = key_cmp;
	tree.key_destroy = key_destroy;
	tree.data_destroy = data_destroy;
	tree.root = NULL;
	tree.size = 0;
	return (tree);
}

static inline void
	free_node(t_rbtree *tree, t_rbnode *node)
{
	if (!node)
		return ;
	if (tree->data_destroy)
		tree->data_destroy(node->data);
	if (tree->key_destroy)
		tree->key_destroy(node->key);
	free_node(tree, node->left);
	free_node(tree, node->right);
	free(node);
}

void
	rb_free(t_rbtree *tree)
{
	free_node(tree, tree->root);
	tree->root = NULL;
}

static inline void
	rb_apply_impl(
	size_t depth,
	t_rbnode *root,
	void (*fn)(size_t depth, t_rbnode *node, void *data),
	void *data)
{
	if (!root)
		return ;
	rb_apply_impl(depth + 1, root->left, fn, data);
	fn(depth, root, data);
	rb_apply_impl(depth + 1, root->right, fn, data);
}

void
	rb_apply(
	t_rbtree *tree,
	void (*fn)(size_t depth, t_rbnode *node, void *data),
	void *data)
{
	rb_apply_impl(0, tree->root, fn, data);
}
/*
#include <string.h>
#include <stdio.h>

void	print_fn(size_t depth, t_rbnode *node, void* cookie)
{
	for (size_t i = 0; i < depth; ++i)
		printf(" | ");
	printf("%s: %s\n", node->key, node->data);
}

int main()
{
	t_rbtree	env;

	env = rb_new(
		(int(*)(const void *a, const void *b))strcmp,
		free,
		free);

	rb_insert(&env, strdup("A"), strdup("Var"));
	rb_insert(&env, strdup("B"), strdup("World"));
	rb_insert(&env, strdup("C"), strdup("Test"));
	rb_insert(&env, strdup("D"), strdup("Test"));
	rb_insert(&env, strdup("E"), strdup("Test"));
	rb_insert(&env, strdup("F"), strdup("TE"));
	rb_insert(&env, strdup("G"), strdup("Last"));
	rb_insert(&env, strdup("H"), strdup("Last"));
	printf("found=%s\n", rb_find(&env, "B"));
	rb_apply(&env, print_fn, NULL);
	rb_delete(&env, "A");
	rb_delete(&env, "C");
	rb_delete(&env, "B");
	rb_apply(&env, print_fn, NULL);
	rb_free(&env);
}
*/
