/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fds_wrappers2.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgamba <linogamba@pundalik.org>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 11:59:40 by lgamba            #+#    #+#             */
/*   Updated: 2025/03/17 11:59:41 by lgamba           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include <shell/shell.h>

int
	shell_pipe(t_shell *shell, int fds[2])
{
	t_fd_data	*read;
	t_fd_data	*write;
	char		*err;

	if (pipe(fds) == -1)
		return (-1);
	read = xmalloc(sizeof(t_fd_data));
	*read = fd_data_from(FDT_PIPE_R, NULL, 0, 0);
	write = xmalloc(sizeof(t_fd_data));
	*write = fd_data_from(FDT_PIPE_W, NULL, 0, 0);
	fd_unique(shell, fds[0], read, SRC_LOCATION);
	fd_unique(shell, fds[1], write, SRC_LOCATION);
	rb_insert(&shell->reg_fds, (void *)(ptrdiff_t)fds[0], read);
	rb_insert(&shell->reg_fds, (void *)(ptrdiff_t)fds[1], write);
	return (0);
}

