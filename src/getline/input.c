/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgamba <linogamba@pundalik.org>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 11:59:40 by lgamba            #+#    #+#             */
/*   Updated: 2025/03/17 11:59:41 by lgamba           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include <shell/shell.h>

int
	getline_getc(t_getline *line)
{
	char	c;
	ssize_t	n;

	n = read(line->in_fd, &c, 1);
	if (n == 1)
		return ((unsigned char)c);
	if (n == 0)
		return (EOF);
	if (n == 0 && (errno == EAGAIN || errno == EWOULDBLOCK))
		return 0;
	if (errno == EINTR && g_signal != SIGQUIT)
		return (-1);
	return (EOF);
}

void
	getline_add_input(t_getline *line, const char *input, size_t len)
{
	stringbuf_append(&line->input_queue, (t_string){input, len});
}

int
	getline_read_char(t_getline *line)
{
	int	c;

	if (line->input_queue.len)
	{
		c = line->input_queue.str[0];
		stringbuf_replace(&line->input_queue, 0, 1, "");
		return (c);
	}
	return (line->getc(line));
}
