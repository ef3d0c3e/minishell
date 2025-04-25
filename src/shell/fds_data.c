/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fds_data.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgamba <linogamba@pundalik.org>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 11:59:40 by lgamba            #+#    #+#             */
/*   Updated: 2025/03/17 11:59:41 by lgamba           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include <shell/shell.h>
#include <stddef.h>

void
	fd_data_free(t_fd_data *data)
{
	free(data->filename);
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
		.original_fd = -1,
		.shadowing = -1,
	});
}

t_fd_data
	fd_data_clone(t_fd_data *data)
{
	char	*filename;
	size_t	len;

	filename = NULL;
	if (data->filename)
	{
		len = ft_strlen(data->filename);
		filename = ft_memcpy(xmalloc(len + 1), data->filename, len + 1);
	}
	return ((t_fd_data){
		.type = data->type,
		.filename = filename,
		.flags = data->flags,
		.mode = data->mode,
		.pipe = data->pipe,
		.original_fd = data->original_fd,
		.shadowing = data->shadowing,
	});
}
