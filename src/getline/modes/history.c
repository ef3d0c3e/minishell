/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   history.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgamba <linogamba@pundalik.org>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 11:59:40 by lgamba            #+#    #+#             */
/*   Updated: 2025/03/17 11:59:41 by lgamba           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include <shell/shell.h>

void
	getline_history_enable(t_getline *line)
{
	line->state.hist.filter = NULL;
	if (line->input.buffer.len)
		line->state.hist.filter = stringbuf_cstr(&line->input.buffer);
	line->state.hist.scroll_index = -1;
	line->state.hist.saved_input = line->input;
	line->state.hist.saved_pos = line->cursor_index;
	line->state.hist.saved_scroll = line->scrolled;
	line->input = getline_buffer_new();
	line->state.hist.accept = 1;
	getline_history_move(line, 1);
}

void
	getline_history_disable(t_getline *line)
{
	if (!line->state.hist.accept)
	{
		getline_buffer_free(&line->input);
		line->input = line->state.hist.saved_input;
		line->cursor_index = line->state.hist.saved_pos;
		line->scrolled = line->state.hist.saved_scroll;
		return ;
	}
	getline_buffer_free(&line->state.hist.saved_input);
	line->cursor_index = line->input.buffer.len;
	line->scrolled = 0;
	getline_buffer_recluster(line, &line->input);
}

static t_key_handler
	*keybinds(void)
{
	static t_key_handler	keys[] = {
		{"\x10", (void *)getline_history_move, SIG_I, { .i0 = +1 }},
		{"\x1b[A", (void *)getline_history_move, SIG_I, { .i0 = +1 }},
		{"\xe", (void *)getline_history_move, SIG_I, { .i0 = -1 }},
		{"\x1b[B", (void *)getline_history_move, SIG_I, { .i0 = -1 }},
		{"\x1b\x1b", (void *)getline_history_cancel, SIG_NONE, { 0 }},
		{NULL, NULL, 0, {0}}
	};

	return (keys);
}

void
	getline_setup_history_mode(t_line_mode *mode)
{
	size_t	i;

	mode->keybinds = rb_new((int (*)(const void *, const void *))ft_strcmp,
			NULL, NULL);
	mode->enable_mode_fn = getline_history_enable;
	mode->disable_mode_fn = getline_history_disable;
	mode->draw_mode_fn = getline_history_draw;
	i = 0;
	while (keybinds()[i].keyseq)
	{
		rb_insert(&mode->keybinds,
				(void *)keybinds()[i].keyseq,
				&keybinds()[i]);
		++i;
	}
}
