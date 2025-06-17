/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   getc.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgamba <linogamba@pundalik.org>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 11:59:40 by lgamba            #+#    #+#             */
/*   Updated: 2025/03/17 11:59:41 by lgamba           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include <shell/shell.h>

int
	getline_handlers_getc(t_getline *line)
{
	char	c;
	ssize_t	n;

	n = read(line->in_fd, &c, 1);
	if (n == 1)
		return ((unsigned char)c);
	if (n == 0)
		return (EOF);
	if (n == 0 && (errno == EAGAIN || errno == EWOULDBLOCK))
		return (0);
	if (errno == EINTR && g_signal != SIGQUIT)
		return (-1);
	return (EOF);
}
