/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fds_data.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgamba <linogamba@pundalik.org>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 11:59:40 by lgamba            #+#    #+#             */
/*   Updated: 2025/03/17 11:59:41 by lgamba           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include <shell/shell.h>

void
	fd_data_free(t_fd_data *data)
{
	free(data->filename);
	free(data->duped_to);
	free(data);
}

static int
	int_cmp(const void *a, const void *b)
{
	return ((int)((ptrdiff_t)a - (ptrdiff_t)b));
}

void
	fd_data_init(t_shell *shell)
{
	shell->reg_fds = rb_new(int_cmp, NULL, (void (*)(void *))fd_data_free);
}

t_fd_data
	fd_data_from(enum e_fd_type type, char *filename, int flags, int mode)
{
	return ((t_fd_data){
		.type = type,
		.filename = filename,
		.flags = flags,
		.mode = mode,
		.pipe = -1,
		.duped_from = -1,
		.duped_to = NULL,
	});
}

t_fd_data
	fd_data_clone(t_fd_data *data)
{
	char	*filename;
	size_t	len;
	size_t	to_len;
	int		*duped_to;

	filename = NULL;
	if (data->filename)
		filename = ft_strdup(data->filename);
	to_len = !!data->duped_to;
	while (data->duped_to && data->duped_to[to_len - 1] != -1)
		++to_len;
	duped_to = NULL;
	if (data->duped_to)
		duped_to = ft_memcpy(xmalloc(sizeof(int) * to_len), data->duped_to,
				sizeof(int) * to_len);
	return ((t_fd_data){
		.type = data->type,
		.filename = filename,
		.flags = data->flags,
		.mode = data->mode,
		.pipe = data->pipe,
		.duped_from = data->duped_from,
		.duped_to = duped_to,
	});
}
