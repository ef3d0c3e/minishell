/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fds_util.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgamba <linogamba@pundalik.org>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 11:59:40 by lgamba            #+#    #+#             */
/*   Updated: 2025/03/17 11:59:41 by lgamba           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include <shell/shell.h>

int
	fd_check(t_shell *shell, int fd, int read, int write)
{
	const t_fd_data	*data = rb_find(&shell->reg_fds, (void *)(ptrdiff_t)fd);
	int				val;

	if (!data)
		return (-1);
	val = 1;
	if (read)
		val &= (data->flags & O_ACCMODE) == O_RDONLY
			|| (data->flags & O_ACCMODE) == O_RDWR;
	if (write)
		val &= (data->flags & O_ACCMODE) == O_WRONLY
			|| (data->flags & O_ACCMODE) == O_RDWR;
	return (val);
}

char
	*fd_print(const t_fd_data *data)
{
	static const char	*names[] = {"OPEN", "DUP", "PIPE(R)", "PIPE(W)"};
	size_t				i;
	t_string_buffer		b;
	char				*s;

	stringbuf_init(&b, 24);
	ft_asprintf(&s, "%s %s fl=%04o m=%04o", names[data->type], data->filename,
			data->flags, data->mode);
	stringbuf_append(&b, (t_string){s, ft_strlen(s)});
	free(s);
	if (data->duped_from != -1)
	{
		ft_asprintf(&s, " duped_from=%d", data->duped_from);
		stringbuf_append(&b, (t_string){s, ft_strlen(s)});
		free(s);
	}
	i = 0;
	while (data->duped_to && data->duped_to[i] != -1)
	{
		ft_asprintf(&s, " duped_to=%d", data->duped_to[i++]);
		stringbuf_append(&b, (t_string){s, ft_strlen(s)});
		free(s);
	}
	return (stringbuf_cstr(&b));
}

void
	fd_unique(t_shell *shell, int fd, const t_fd_data *data, const char *loc)
{
	const t_fd_data	*found = rb_find(&shell->reg_fds, (void *)(ptrdiff_t)fd);
	char			*err;

	if (!found)
		return ;
	ft_asprintf(&err, "Duplicate file descriptors [%d]:\n * %s\n * %s",
		fd_print(found),
		fd_print(data));
	shell_error(shell, err, loc);
	shell_exit(shell, 1);
}
