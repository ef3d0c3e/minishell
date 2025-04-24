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
#include <shell/shell.h>

static int
	ptr_cmp(const void *a, const void *b)
{
	return ((int)((ptrdiff_t)a - (ptrdiff_t)b));
}

void
	temporaries_init(t_shell *shell)
{
	shell->temporaries = rb_new(ptr_cmp, NULL, free);
}
