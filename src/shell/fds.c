/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fds.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgamba <linogamba@pundalik.org>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 11:59:40 by lgamba            #+#    #+#             */
/*   Updated: 2025/03/17 11:59:41 by lgamba           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include <shell/shell.h>

void
	fd_data_free(t_fd_data *fd)
{

}

static int
	int_cmp(const void *a, const void *b)
{
	return (*(int *)a - *(int *)b);
}

void
	fd_data_init(t_shell *shell)
{
	shell->reg_fds = rb_new(int_cmp, NULL, (void (*)(void *))fd_data_free);
}

