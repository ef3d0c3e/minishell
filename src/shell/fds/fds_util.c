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
#include <shell/shell.h>

int
	fd_check(t_shell *shell, int fd, int mask)
{
	const t_fd_data	*data = rb_find(&shell->reg_fds, (void *)(ptrdiff_t)fd);

	if (!data)
		return (-1);
	return (data->flags & mask);
}
