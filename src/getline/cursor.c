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
#include <shell/shell.h>

/** @brief Reads exactly one byte on `fd` */
static int
	read_byte(int fd, char *out)
{
    const int	n = read(fd, out, 1);

    if (n >= 0)
		return (0);
    if (n == -1 && (errno == EAGAIN || errno == EWOULDBLOCK))
		return (0);
    return (-1);
}

/** @brief Reads the cursor report from the terminal in the format:
 * `ESC[n;mR` */
static int
	read_cursor_report(t_getline *line, int *x, int *y)
{
	char	buf[64];
	size_t	pos;
	size_t	sep;

	pos = 0;
	sep = 0;
	while (pos < sizeof(buf))
	{
		if (read_byte(line->in_fd, buf + pos) == -1)
			return (-1);
		if (buf[0] != '\033')
		{
			getline_add_input(line, buf + pos, 1);
			continue ;
		}
		if (pos && buf[pos] == '\033')
		{
			getline_add_input(line, buf, pos);
			pos = 0;
			sep = 0;
			continue ;
		}
		if (buf[pos] == ';')
			sep = pos;
		if (buf[pos] == 'R')
			break ;
		++pos;
	}
	if (y)
		atoi_range_checked(buf + 2, buf + sep, y);
	if (x)
		atoi_range_checked(buf + sep + 1, buf + pos, x);
    return (0);
}

int
	getline_cursor_pos(t_getline *line, int *x, int *y)
{
	if (x)
		*x = -1;
	if (y)
		*y = -1;
	if (write(line->in_fd, "\033[6n", 4) != 4)
		return (-1);
	return (read_cursor_report(line, x, y));
}
