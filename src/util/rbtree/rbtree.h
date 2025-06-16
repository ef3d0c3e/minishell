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
#ifndef RBTREE_H
# define RBTREE_H

# ifndef UTIL_INTERNAL
#  error "Include <util/util.h> instead."
# endif // UTIL_INTERNAL

# include "../includes.h"

/**
 * @file Red-Black tree implementation
 * based on this: http://www.sgi.com/tech/stl/stl_tree.h
 * and this: https://github.com/xieqing/red-black-tree
 */

/** @brief RB tree node */
typedef struct s_rbnode
{
	/** @brief Parent */
	struct s_rbnode	*parent;
	/** @brief Left child */
	struct s_rbnode	*left;
	/** @brief Right child */
	struct s_rbnode	*right;
	/** @brief Node key */
	void			*key;
	/** @brief Data stored by this node */
	void			*data;
	/** @brief Node color, 0: red, 1: black */
	uint8_t			color;
}	t_rbnode;

/** @brief RB tree */
typedef struct s_rbtree
{
	/** @brief Key comparison function */
	int			(*key_cmp)(const void *lhs, const void *rhs);
	/** @brief Key destroy function */
	void		(*key_destroy)(void *key);
	/** @brief Data destroy function */
	void		(*data_destroy)(void *data);
	/** @brief Tree root node */
	t_rbnode	*root;
	/** @brief Number of nodes in the tree */
	size_t		size;
}	t_rbtree;

/**
 * @brief Creates a new red-black tree
 *
 * @param key_cmp Key comparison function
 * @param key_destroy Key destroy function
 * @param data_destroy Data destroy function
 *
 * @returns The newly constructed red-black tree
 */
t_rbtree
rb_new(
	int (*key_cmp)(const void *lhs, const void *rhs),
	void (*key_destroy)(void *key),
	void (*data_destroy)(void *data));

/**
 * @brief Frees a red-black tree
 *
 * This function only frees the node-related data for the tree. Essentially
 * clearing the tree, meaning it can be used again as an empty tree (keeping
 * the current comparison/deletion functions).
 *
 * @param tree Tree to free
 */
void
rb_free(t_rbtree *tree);

/**
 * @brief Inserts new key/value pair into the tree
 *
 * @param tree Tree to insert into
 * @param key Key
 * @param data Value
 *
 * @return The inserted key/value pair's resulting node
 */
t_rbnode
*rb_insert(t_rbtree *tree, void *key, void *data);

/**
 * @brief Deletes a key/value pair by calling the destroy functions
 *
 * @param tree Tree to delete into
 * @param key Key to delete
 *
 * @return 1 If value was deleted, 0 otherwise
 */
int
rb_delete(t_rbtree *tree, const void *key);

/**
 * @brief Tries to find node by key
 *
 * @param tree Tree to search
 * @param key Key to search in tree
 *
 * @returns The associated node, NULL if not found
 */
t_rbnode
*rb_find_node(t_rbtree *tree, const void *key);

/**
 * @brief Tries to value by key
 *
 * @param tree Tree to search
 * @param key Key to search in tree
 *
 * @returns The value associated with key, NULL if not found
 */
void
*rb_find(t_rbtree *tree, const void *key);

/**
 * @brief Applies function to tree
 *
 * The tree is traversed in DFS order
 *
 * @param tree Tree to apply function
 * @param fn Function to apply on every node
 * @param depth Node's depth
 * @param node Node
 * @param data Caller-provided data
 */
void
rb_apply(
	t_rbtree *tree,
	void (*fn)(size_t depth, t_rbnode *node, void *data),
	void *data);

/* ************************************************************************** */
/* Internal                                                                   */
/* ************************************************************************** */

void
rb_rotate_left(t_rbtree *rbt, t_rbnode *x);
void
rb_rotate_right(t_rbtree *rbt, t_rbnode *x);
/**
 * @brief Returns the minimum of subtree designated by `node`
 *
 * @returns node Subtree to get the minimal value from
 *
 * @returns The minimal value of subtree designated by node. A.k.a the leftmost
 * node
 */
t_rbnode
*rb_minimum(t_rbnode *node);
/**
 * @brief Returns the maximum of subtree designated by `node`
 *
 * @returns node Subtree to get the maximal value from
 *
 * @returns The maximal value of subtree designated by node. A.k.a the rightmost
 * node
 */
t_rbnode
*rb_maximum(t_rbnode *node);
/**
 * @brief Replaces the subtree rooted at u with the subtree rooted at v.
 * If u->parent is NULL, then v becomes root; otherwise
 * v is attached to u->parent in place of u.
 *
 * @param tree The tree to operate in
 * @param u First subtree
 * @param v Second subtree
 */
void
rb_transplant(t_rbtree *tree, t_rbnode *u, t_rbnode *v);
/**
 * @brief Rebalance after deleting a black node. We track x's parent separately
 * because if x is NULL, x->parent would be invalid.
 *
 * @param tree The tree to rebalance
 * @param x Deleted node
 * @param x_parent Deleted node's parent
 * */
void
rb_delete_fixup(t_rbtree *tree, t_rbnode *x, t_rbnode *x_parent);

#endif // BTREE_H
