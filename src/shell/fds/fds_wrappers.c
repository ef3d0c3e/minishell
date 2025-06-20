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
#include <shell/shell.h>

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
	int			status;

	data = rb_find(&shell->reg_fds, (void *)(ptrdiff_t)fd);
	if (!data)
	{
		close(fd);
		ft_asprintf(&err, "Attempt to close unregistered fd: %d", fd);
		shell_fail(shell, err, SRC_LOCATION);
	}
	status = close(fd);
	if (status != -1)
		rb_delete(&shell->reg_fds, (void *)(ptrdiff_t)fd);
	return (status);
}

int
	shell_dup(t_shell *shell, int fd)
{
	t_fd_data	*data;
	t_fd_data	*newdata;
	int			newfd;
	char		*err;

	data = rb_find(&shell->reg_fds, (void *)(ptrdiff_t)fd);
	if (!data)
		return (ft_asprintf(&err, "Attempt to dup unregistered fd: %d", fd),
			shell_error(shell, err, SRC_LOCATION), -1);
	newfd = dup(fd);
	if (newfd == -1)
		return (newfd);
	if (rb_find(&shell->reg_fds, (void *)(ptrdiff_t)newfd))
	{
		close(newfd);
		ft_asprintf(&err, "Attempt to dup to already existing fd: %d", newfd);
		return (shell_error(shell, err, SRC_LOCATION), -1);
	}
	newdata = xmalloc(sizeof(t_fd_data));
	*newdata = fd_data_clone(data);
	newdata->duped_from = fd;
	newdata->type = FDT_DUP;
	rb_insert(&shell->reg_fds, (void *)(ptrdiff_t)newfd, newdata);
	return (newfd);
}

static void
	duped_to_push(t_fd_data *fd, int new)
{
	size_t	len;

	len = 0;
	while (fd->duped_to && fd->duped_to[len] != -1)
		++len;
	fd->duped_to = ft_realloc(fd->duped_to, sizeof(int) * len,
			sizeof(int) * (len + 2));
	fd->duped_to[len] = new;
	fd->duped_to[len + 1] = -1;
}

int
	shell_dup2(t_shell *shell, int oldfd, int newfd)
{
	t_fd_data	*olddata;
	t_fd_data	*newdata;
	int			status;
	char		*err;

	olddata = rb_find(&shell->reg_fds, (void *)(ptrdiff_t)oldfd);
	if (!olddata)
	{
		ft_asprintf(&err, "Attempt to dup2 from unregistered fd: %d", oldfd);
		shell_fail(shell, err, SRC_LOCATION);
	}
	newdata = rb_find(&shell->reg_fds, (void *)(ptrdiff_t)newfd);
	if (!newdata)
	{
		ft_asprintf(&err, "Attempt to dup2 to unregistered fd: %d", newfd);
		shell_fail(shell, err, SRC_LOCATION);
	}
	status = dup2(oldfd, newfd);
	if (status != -1)
		duped_to_push(olddata, newfd);
	return (status);
}
