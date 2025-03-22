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
	struct s_rbnode	*parent;
	struct s_rbnode	*left;
	struct s_rbnode	*right;
	void			*const	key;
	void			*data;
	uint8_t			color;
}	t_rbnode;

/** @brief RB tree */
typedef struct s_rbtree
{
	int	(*key_cmp)(const void *lhs, const void *rhs);
	void (*key_destroy)(void *key);
	void (*value_destroy)(void *value);

	t_rbnode	*root;
}	t_rbtree;

#endif // BTREE_H
