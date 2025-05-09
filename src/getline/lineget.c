/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   getline.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgamba <linogamba@pundalik.org>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 11:59:40 by lgamba            #+#    #+#             */
/*   Updated: 2025/03/17 11:59:41 by lgamba           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "getline/getline.h"
#include <shell/shell.h>

t_getline
	getline_setup(t_shell *shell)
{
	t_getline	line;

	line.shell = shell;
	line.in_fd = STDIN_FILENO;
	line.out_fd = STDOUT_FILENO;
	line.getc = getline_getc;
	line.prompt = NULL;
	line.buffer = getline_buffer_new();
	line.cursor_index = 0;
	line.display_width = 0;
	stringbuf_init(&line.input_queue, 24);
	getline_setup_keys(&line);
	return (line);
}

void
	getline_cleanup(t_getline *line)
{

}

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
	raw.c_cc[VTIME] = 1;
	if (tcsetattr(line->in_fd, TCSAFLUSH, &raw) == -1)
	{
		shell_perror(line->shell, "tcsetattr fail", SRC_LOCATION);
	}
}

/*
int measure_terminal_width(t_getline *line,
		const char *utf8,
		size_t byte_len) {
	// 1) move to a known safe spot (e.g. start of spare line)
	write(line->out_fd, "\r\n", 2);
	write(line->out_fd, "\x1b[6n", 4);
	if (write(line->in_fd, "\x1b[6n", 4) != 4) return -1;
	int col0 = read_cursor_column(line->in_fd);

	// 2) write the test sequence
	write(line->out_fd, utf8, byte_len);

	// 3) ask again
	write(line->out_fd, "\x1b[6n", 4);
	int col1 = read_cursor_column(line->in_fd);

	// 4) restore up one line
	write(line->out_fd, "\x1b[1A\r", 4);

	return col1 - col0;
}
*/

char
	*getline_read(t_getline *line, char *prompt)
{
	int				c;
	t_keybind_fn	callback;

	getline_raw_mode(line, 1);

	int cx, cy;

	getline_cursor_pos(line, &cx, &cy);
	ft_dprintf(2, "Pos=%d %d\n\r", cx, cy);
	ft_dprintf(0, "🧑‍🌾");
	getline_cursor_pos(line, &cx, &cy);
	ft_dprintf(2, "Pos=%d %d\n\r", cx, cy);
	//while (1)
	//{
	//	c = line->getc(line->in_fd);
	//	if (c == -1)
	//		break ;
	//	if (c == '\003')
	//	{
	//		write(0, "\033[6n", 4);
	//		continue ;
	//	}
	//	if (c == '\004')
	//		break ;
	//	if (getline_handle_key(line, c))
	//		continue ;
	//	getline_buffer_insert(line, c);
	//	ft_dprintf(2, "Width =%d\n\r", read_cursor_column(line->in_fd));
	//	ft_dprintf(2, "Buffer='%.*s'\n\r", line->buffer.buffer.len, line->buffer.buffer.str);

	//}
	getline_raw_mode(line, 0);

	return (NULL);
}

int main(int ac, const char **av, const char **envp)
{
	t_shell shell = shell_new(envp);

	t_getline line = getline_setup(&shell);

	getline_read(&line, ft_strdup(" > "));

	shell_free(&shell);
	return (0);
}
