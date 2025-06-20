/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cursor.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgamba <linogamba@pundalik.org>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 11:59:40 by lgamba            #+#    #+#             */
/*   Updated: 2025/03/17 11:59:41 by lgamba           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "ft_printf.h"
#include <shell/shell.h>

/** @brief Reads exactly one byte on `fd` */
static int
	read_byte(int fd, char *out)
{
	const int	n = read(fd, out, 1);

	if (n >= 0)
		return (0);
	if (n == -1 && (errno == EAGAIN || errno == EWOULDBLOCK || errno == EINTR))
		return (0);
	return (-1);
}

/** @brief Reads the cursor report from the terminal in the format:
 * `ESC[n;mR` */
static int
	read_cursor_report(t_getline *line, char *buf, size_t *sep, size_t *pos)
{
	while (*pos < sizeof(buf))
	{
		if (read_byte(line->in_fd, buf + *pos) == -1)
			return (0);
		if (buf[0] != '\033')
		{
			getline_recycle_input(line, buf, 1);
			continue ;
		}
		if (*pos && buf[*pos] == '\033')
		{
			getline_recycle_input(line, buf, *pos);
			*pos = 0;
			buf[*pos++] = '\033';
			sep = 0;
			continue ;
		}
		if (buf[*pos] == ';')
			*sep = *pos;
		if (buf[*pos] == 'R')
			break ;
		++*pos;
	}
	return (1);
}

int
	getline_cursor_pos(t_getline *line, int *x, int *y)
{
	size_t	sep;
	size_t	pos;
	char	buf[64];

	if (x)
		*x = -1;
	if (y)
		*y = -1;
	if (write(line->out_fd, "\033[6n", 4) != 4)
		return (-1);
	sep = 0;
	pos = 0;
	if (!read_cursor_report(line, buf, &sep, &pos))
		return (0);
	if (y)
		atoi_range_checked(buf + 2, buf + sep, y);
	if (x)
		atoi_range_checked(buf + sep + 1, buf + pos, x);
	return (1);
}

void
	getline_cursor_set(t_getline *line, int col, int row)
{
	ft_dprintf(line->out_fd, "\x1b[%d;%dH", row, col);
}

void
	getline_cursor_visible(t_getline *line, int visible)
{
	if (!line->shell->cap.strs.civis || !line->shell->cap.strs.cnorm)
		return ;
	if (visible)
		ft_dprintf(line->out_fd, "%s", line->shell->cap.strs.cnorm);
	else
		ft_dprintf(line->out_fd, "%s", line->shell->cap.strs.civis);
	// TODO
}
