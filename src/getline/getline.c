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
#include <shell/shell.h>

t_getline
	getline_setup(t_shell *shell)
{
	t_getline	line;

	line.shell = shell;
	line.in_fd = STDIN_FILENO;
	line.out_fd = STDOUT_FILENO;
	line.input = getline_buffer_new();
	line.cursor_index = 0;
	line.scrolled = 0;
	line.display_width = 0;
	line.display_height = 0;
	line.sequence_len = 0;
	line.history = getline_history_init(8192);
	stringbuf_init(&line.input_queue, 24);
	getline_setup_handlers(&line);
	line.mode = LINE_INPUT;
	getline_setup_modes(&line);
	if (line.modes[line.mode].enable_mode_fn)
		line.modes[line.mode].enable_mode_fn(&line);
	return (line);
}

void
	getline_cleanup(t_getline *line)
{
	size_t	i;

	stringbuf_free(&line->input_queue);
	i = 0;
	while (i < 2/*LINE_MODE_SIZE*/)
		rb_free(&line->modes[i++].keybinds);
	getline_history_free(line);
}

void
	getline_input_add(t_getline *line, int c)
{
	t_u8_iterator	it;

	getline_buffer_insert(line, c);
	if (line->input.cp_len)
		return ;
	it = it_new((t_string){line->input.buffer.str, line->input.buffer.len});
	it_next(&it);
	while (it.byte_next < line->cursor_index)
		it_next(&it);
	getline_recluster(line, &line->input, it);
}

static char
	*get_input(t_getline *line)
{
	char *const	input = stringbuf_cstr(&line->input.buffer);

	line->input.buffer.str = NULL;
	getline_buffer_free(&line->input);
	getline_buffer_free(&line->prompt);
	stringbuf_free(&line->input_queue);
	stringbuf_init(&line->input_queue, 24);
	line->cursor_index = 0;
	line->scrolled = 0;
	line->display_width = 0;
	line->display_height = 0;
	line->sequence_len = 0;
	return (input);
}

char
	*getline_read(t_getline *line, const char *prompt)
{
	int		c;
	char	*ret;

	getline_raw_mode(line, 1);
	getline_set_prompt(line, prompt);
	getline_redraw(line, 1);
	while (1)
	{
		c = getline_read_char(line);
		if (c == -1)
			continue ;
		if (c == '\003' || c == '\004')
			break ;
		if (c == '\x0d' && line->mode == LINE_INPUT)
			break ;
		if (getline_handle_key(line, c))
			continue ;
		getline_input_add(line, c);
		getline_redraw(line, 1);
	}
	ft_dprintf(line->out_fd, "\n\r");
	getline_raw_mode(line, 0);
	return (get_input(line));
}

/*
static void highlighter(t_getline *line)
{
	t_token_list	list;

	list = tokenizer_tokenize((t_string){line->input.buffer.str, line->input.buffer.len});
	for (size_t i = 0; i < list.size; ++i)
	{
		if (list.tokens[i].type == TOK_KEYWORD)
			getline_highlight_add(&line->input, (t_buffer_attr){
				list.tokens[i].start, list.tokens[i].end,
				0xFF0000, 0, 0, 0,
			});
		else if (token_isword(list.tokens[i].type))
			getline_highlight_add(&line->input, (t_buffer_attr){
				list.tokens[i].start, list.tokens[i].end,
				0x00FF00, 0, 0, 0,
			});
	}
	//for (size_t i = 0; i < line->input.s_attrs.size; ++i)
	//	ft_dprintf(2, "{%zu..%zu} ", line->input.s_attrs.data[i].start, line->input.s_attrs.data[i].end);
	//ft_dprintf(2, "\n\r");
	token_list_free(&list);
}

int main(int ac, const char **av, const char **envp)
{
	t_shell shell = shell_new(envp);

	t_getline line = getline_setup(&shell);
	line.highlighter_fn = highlighter;

	char *in = getline_read(&line, "[prompt]");
	ft_dprintf(2, "input=%s\n", in);
	free(in);


	getline_cleanup(&line);
	shell_free(&shell);
	return (0);
}
*/
