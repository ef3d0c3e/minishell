/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lineget.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgamba <linogamba@pundalik.org>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 11:59:40 by lgamba            #+#    #+#             */
/*   Updated: 2025/03/17 11:59:41 by lgamba           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include <shell/shell.h>

static int
	lineget_getc(int fd)
{
	char	c;
	ssize_t	n;

	n = read(fd, &c, 1);
	if (n == 1)
		return ((unsigned char)c);
	if (n == 0)
		return (EOF);
	if (errno == EINTR && g_signal != SIGQUIT)
		return (-1);
	return (EOF);
}

t_lineget
	lineget_setup(void)
{
	t_lineget	line;

	line.in_fd = STDIN_FILENO;
	line.out_fd = STDOUT_FILENO;
	line.getc = lineget_getc;
	line.prompt = NULL;
	line.buffer = lineget_buffer_new();
	return (line);
}

void
	lineget_cleanup(t_lineget *line)
{

}
