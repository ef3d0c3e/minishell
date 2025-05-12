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
#include "ft_printf.h"
#include "tokenizer/tokenizer.h"
#include "util/util.h"
#include <shell/shell.h>
#include <termios.h>

t_getline
	getline_setup(t_shell *shell)
{
	t_getline	line;

	line.shell = shell;
	line.in_fd = STDIN_FILENO;
	line.out_fd = STDOUT_FILENO;
	line.getc = getline_getc;
	line.highlighter_fn = NULL;
	line.buffer = getline_buffer_new();
	line.cursor_index = 0;
	line.render = getline_render_new();
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
	raw.c_cc[VTIME] = 0;
	if (tcsetattr(line->in_fd, TCSANOW, &raw) == -1)
	{
		shell_perror(line->shell, "tcsetattr fail", SRC_LOCATION);
	}
}

void
	getline_input_add(t_getline *line, int c)
{
	t_u8_iterator	it;

	getline_buffer_insert(line, c);
	if (line->buffer.cp_len)
		return ;
	it = it_new((t_string){line->buffer.buffer.str, line->buffer.buffer.len});
	it_next(&it);
	while (it.byte_next < line->cursor_index)
		it_next(&it);
	getline_recluster(line, &line->buffer, it);
	getline_redraw(line, 1);
	//getline_cluster_print(line);
}


char
	*getline_read(t_getline *line, const char *prompt)
{
	int		c;
	char	*ret;

	getline_raw_mode(line, 1);
	getline_set_prompt(line, prompt);
	while (1)
	{
		c = getline_read_char(line);
		if (c == -1)
			continue ;
		if (c == '\003')
			break ;
		if (c == '\004')
			break ;
		if (getline_handle_key(line, c))
			continue ;
		getline_input_add(line, c);
	}
	getline_buffer_free(&line->prompt);
	getline_raw_mode(line, 0);
	ret = stringbuf_cstr(&line->buffer.buffer);
	line->buffer.buffer.str = NULL;
	getline_buffer_free(&line->buffer);
	return (ret);
}

static void highlighter(t_getline *line)
{
	t_token_list	list;

	list = tokenizer_tokenize((t_string){line->buffer.buffer.str, line->buffer.buffer.len});
	for (size_t i = 0; i < list.size; ++i)
	{
		if (list.tokens[i].type == TOK_KEYWORD)
		{
			getline_highlight_add(&line->buffer, (t_buffer_attr){
				list.tokens[i].start, list.tokens[i].end,
				0xFF0000, 0, 0, 0,
			});
		}
	}


}

int main(int ac, const char **av, const char **envp)
{
	t_shell shell = shell_new(envp);

	t_getline line = getline_setup(&shell);
	line.highlighter_fn = highlighter;

	getline_read(&line, ft_strdup("[prompt]"));

	shell_free(&shell);
	return (0);
}
