/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   complete.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgamba <linogamba@pundalik.org>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 11:59:40 by lgamba            #+#    #+#             */
/*   Updated: 2025/03/17 11:59:41 by lgamba           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include <shell/shell.h>

static void
	complete_enable_end(t_getline *line)
{
	line->state.comp.mini_mode = 1;
	line->state.comp.sel = -1;
	line->state.comp.start_row = line->state.comp.cur_y + 1;
	line->state.comp.end_row = line->display_height - 1;
	line->state.comp.col_width = line->display_width / 60;
	if (!line->state.comp.col_width)
		line->state.comp.col_width = line->display_width;
	else
		line->state.comp.col_width = line->display_width
			/ line->state.comp.col_width;
	line->state.comp.scrolled = 0;
	line->state.comp.saved_input = line->input;
	line->state.comp.saved_pos = line->cursor_index;
	line->state.comp.saved_scroll = line->scrolled;
	line->input = getline_buffer_clone(&line->state.comp.saved_input);
	line->state.comp.accept = 0;
	if (line->state.comp.nitems == 1)
	{
		line->state.comp.sel = 0;
		getline_complete_update(line);
		getline_complete_select(line);
		getline_input_move(line, 1);
		return ;
	}
	getline_redraw(line, 1);
}

void
	getline_complete_enable(t_getline *line)
{
	getline_cursor_pos(line, &line->state.comp.cur_x,
		&line->state.comp.cur_y);
	line->state.comp.items = NULL;
	line->state.comp.nitems = 0;
	line->state.comp.accept = -1;
	getline_complete_populate_items(line);
	if (!line->state.comp.nitems)
	{
		getline_change_mode(line, LINE_INPUT);
		return ;
	}
	if (line->display_height - line->state.comp.cur_y < 10)
	{
		line->state.comp.cur_y -= 10;
		ft_dprintf(line->out_fd, "\x1b[%dS", 10);
	}
	complete_enable_end(line);
}

void
	getline_complete_disable(t_getline *line)
{
	getline_complete_free_items(line);
	if (line->state.comp.accept == 0)
	{
		getline_buffer_free(&line->input);
		line->input = line->state.comp.saved_input;
		line->cursor_index = line->state.comp.saved_pos;
		line->scrolled = line->state.comp.saved_scroll;
	}
	else if (line->state.comp.accept == 1)
	{
		getline_buffer_free(&line->state.comp.saved_input);
		line->cursor_index = line->state.comp.word_end;
		line->scrolled = 0;
	}
	getline_cursor_set(line, line->state.comp.cur_x, line->state.comp.cur_y);
	ft_dprintf(line->out_fd, "\x1b[0J");
}

static t_key_handler
	*keybinds(void)
{
	static t_key_handler	keys[] = {
	{"\x1b[C", (void *)getline_complete_move, SIG_I, {.i0 = +1}},
	{"\x06", (void *)getline_complete_move, SIG_I, {.i0 = +1}},
	{"\x09", (void *)getline_complete_move, SIG_I, {.i0 = +1}},
	{"\x1b[D", (void *)getline_complete_move, SIG_I, {.i0 = -1}},
	{"\x02", (void *)getline_complete_move, SIG_I, {.i0 = -1}},
	{"\x1b[Z", (void *)getline_complete_move, SIG_I, {.i0 = -1}},
	{"\x1b[A", (void *)getline_complete_move_row, SIG_I, {.i0 = +1}},
	{"\x1b[5~", (void *)getline_complete_move_page, SIG_I, {.i0 = +1}},
	{"\x0e", (void *)getline_complete_move_row, SIG_I, {.i0 = +1}},
	{"\x1b[B", (void *)getline_complete_move_row, SIG_I, {.i0 = -1}},
	{"\x10", (void *)getline_complete_move_row, SIG_I, {.i0 = -1}},
	{"\x1b[6~", (void *)getline_complete_move_page, SIG_I, {.i0 = -1}},
	{"\xd", (void *)getline_complete_select, SIG_NONE, {0}},
	{NULL, NULL, 0, {0}}
	};

	return (keys);
}

void
	getline_setup_complete_mode(t_line_mode *mode)
{
	size_t	i;

	mode->keybinds = rb_new((int (*)(const void *, const void *))ft_strcmp,
			NULL, NULL);
	mode->enable_mode_fn = getline_complete_enable;
	mode->disable_mode_fn = getline_complete_disable;
	mode->draw_mode_fn = getline_complete_draw;
	i = 0;
	while (keybinds()[i].keyseq)
	{
		rb_insert(&mode->keybinds,
			(void *)keybinds()[i].keyseq,
			&keybinds()[i]);
		++i;
	}
}
