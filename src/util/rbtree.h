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

#include <stdint.h>
# ifndef UTIL_INTERNAL
#  error "Include <util/util.h> instead."
# endif // UTIL_INTERNAL

/**
 * @brief Red-Black tree implementation
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
	int	(*key_cmp)(const void *lhs, const void *rhs);
	/** @brief Key destroy function */
	void (*key_destroy)(void *key);
	/** @brief Data destroy function */
	void (*data_destroy)(void *data);
	/** @brief Tree root node */
	t_rbnode	*root;
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
	int	(*key_cmp)(const void *lhs, const void *rhs),
	void (*key_destroy)(void *key),
	void (*data_destroy)(void *data));

/** @brief Frees a red-black tree */
void
rb_free(t_rbtree *tree);

t_rbnode
*rb_insert(t_rbtree *tree, void *key, void *data);

/* ************************************************************************** */
/* Internal                                                                   */
/* ************************************************************************** */

#endif // BTREE_H
