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
#include "lineget/lineget.h"
#include "ft_printf.h"
#include "util/util.h"
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
	lineget_setup(t_shell *shell)
{
	t_lineget	line;

	line.shell = shell;
	line.in_fd = STDIN_FILENO;
	line.out_fd = STDOUT_FILENO;
	line.getc = lineget_getc;
	line.prompt = NULL;
	line.buffer = lineget_buffer_new();
	line.cursor_index = 0;
	line.display_width = 0;
	lineget_setup_keys(&line);
	return (line);
}

void
	lineget_cleanup(t_lineget *line)
{

}

void
	lineget_raw_mode(t_lineget *line, int set)
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

char
	*lineget_read(t_lineget *line, char *prompt)
{
	unsigned char	key_sequece[16];
	size_t			sequence_len;
	int				c;
	t_keybind_fn	callback;

	lineget_raw_mode(line, 1);
	sequence_len = 0;
	while (1)
	{
		c = line->getc(line->in_fd);
		if (c == -1)
			break ;
		if (c == '\004')
			break ;
		if (lineget_handle_key(line, c))
			continue ;
		ft_dprintf(2, "Got: %d\n", c);
	}
	lineget_raw_mode(line, 0);
	
	return (NULL);
}

int main(int ac, const char **av, const char **envp)
{
	t_shell shell = shell_new(envp);

	t_lineget line = lineget_setup(&shell);

	lineget_read(&line, ft_strdup(" > "));

	shell_free(&shell);
	return (0);
}
