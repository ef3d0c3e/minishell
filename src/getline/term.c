/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   term.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgamba <linogamba@pundalik.org>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 11:59:40 by lgamba            #+#    #+#             */
/*   Updated: 2025/03/17 11:59:41 by lgamba           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include <shell/shell.h>

void
	getline_raw_mode(t_getline *line, int set)
{
	struct termios	raw;

	if (!set)
	{
		tcsetattr(line->in_fd, TCSAFLUSH, &line->tio);
		return ;
	}
	if (!isatty(line->in_fd))
		return ;
	if (tcgetattr(line->in_fd, &line->tio) == -1)
	{
		shell_perror(line->shell, "tcgetattr fail", SRC_LOCATION);
		return ;
	}

	raw = line->tio;
    raw.c_iflag &= ~(BRKINT | ICRNL | INPCK | ISTRIP | IXON);
    raw.c_oflag &= ~(OPOST);
    raw.c_cflag |= (CS8);
    raw.c_lflag &= ~(ECHO | ICANON | IEXTEN | ISIG);
	raw.c_cc[VMIN]  = 1;
	raw.c_cc[VTIME] = 0;
	if (tcsetattr(line->in_fd, TCSANOW, &raw) == -1)
	{
		shell_perror(line->shell, "tcsetattr fail", SRC_LOCATION);
	}
}

int
	getline_text_width(t_getline *line, const char *utf8, size_t byte_len)
{
	int	col0;
	int	col1;

	write(line->out_fd, "\r\n", 2);
	if (getline_cursor_pos(line, &col0, NULL) == -1)
		return (-1);

	write(line->out_fd, utf8, byte_len);
	if (getline_cursor_pos(line, &col1, NULL) == -1)
		return (-1);
	write(line->out_fd, "\x1b[1A\r", 5); 

	// col0=1 col1=1
	//ft_dprintf(2, "[%d %d]", col0, col1);
	return (col1 - col0);
}
