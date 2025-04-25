/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fds_wrappers.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgamba <linogamba@pundalik.org>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 11:59:40 by lgamba            #+#    #+#             */
/*   Updated: 2025/03/17 11:59:41 by lgamba           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "util/util.h"
#include <shell/shell.h>
#include <stddef.h>

int
	shell_open(t_shell *shell, const char *filename, int flags, int mode)
{
	int			fd;
	t_fd_data	*data;
	char		*err;


	fd = open(filename, flags, mode);
	if (fd < 0)
		return (fd);
	data = rb_find(&shell->reg_fds, (void *)(ptrdiff_t)fd);
	if (data)
	{
		close(fd);
		ft_asprintf(&err, "Duplicate file descriptor %d: '%s' and '%s'",
			data->filename, filename);
		shell_fail(shell, err, SRC_LOCATION);
	}
	data = xmalloc(sizeof(t_fd_data));
	*data = fd_data_from(FDT_OPEN, ft_strdup(filename), flags, mode);
	rb_insert(&shell->reg_fds, (void *)(ptrdiff_t)fd, data);
	return (fd);
}

int
	shell_close(t_shell *shell, int fd)
{
	t_fd_data	*data;
	char		*err;
	
	data = rb_find(&shell->reg_fds, (void *)(ptrdiff_t)fd);
	if (!data)
	{
		close(fd);
		ft_asprintf(&err, "Attempt to close unregistered fd: %d", fd);
		shell_fail(shell, err, SRC_LOCATION);
	}
	rb_delete(&shell->reg_fds, (void *)(ptrdiff_t)fd);
	return (close(fd));
}
