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
#include "ft_printf.h"
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
	while (i < 3/*LINE_MODE_SIZE*/)
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
	if (line->state.action != ACT_ENTER)
	{
		free(input);
		return (NULL);
	}
	return (input);
}

char
	*getline_read(t_getline *line, const char *prompt)
{
	int		c;

	getline_raw_mode(line, 1);
	line->mode = LINE_INPUT;
	if (line->modes[line->mode].enable_mode_fn)
		line->modes[line->mode].enable_mode_fn(line);
	getline_set_prompt(line, prompt);
	getline_redraw(line, 1);
	while (1)
	{
		if (line->mode == LINE_INPUT && getline_process_action(line))
			break ;
		c = getline_read_char(line);
		if (c == -1)
			continue ;
		if (getline_handle_key(line, c))
			continue ;
		if (line->mode != LINE_INPUT)
		{
			getline_change_mode(line, LINE_INPUT);
			continue ;
		}
		getline_input_add(line, c);
		getline_redraw(line, 1);
	}
	ft_dprintf(line->out_fd, "\n\r");
	getline_raw_mode(line, 0);
	return (get_input(line));
}
