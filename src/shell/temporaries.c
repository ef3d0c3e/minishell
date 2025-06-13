/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   temporaries.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgamba <linogamba@pundalik.org>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 11:59:40 by lgamba            #+#    #+#             */
/*   Updated: 2025/03/17 11:59:41 by lgamba           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "util/util.h"
#include <shell/shell.h>
#include <stdio.h>

/**
 * @file Handles temporary heap variables
 *
 * Variables are added to this tree so they can be removed when `fork` is called
 * and finishes.
 */

static int
	ptr_cmp(const void *a, const void *b)
{
	return ((int)((ptrdiff_t)a - (ptrdiff_t)b));
}

void
	temporaries_init(t_shell *shell)
{
	shell->atexit = rb_new(ptr_cmp, NULL, NULL);
}

static void
	cleanup_fn(size_t depth, t_rbnode *node, void *cookie)
{
	(void)depth;
	(void)cookie;

	if (node->data)
		((void (*)(void *))node->data)(node->key);
	else
		free(node->key);
}

void
	temporaries_cleanup(t_shell *shell)
{
	rb_apply(&shell->atexit, cleanup_fn, NULL);
	rb_free(&shell->atexit);
}
