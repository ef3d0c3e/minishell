/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input_move.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgamba <linogamba@pundalik.org>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 11:59:40 by lgamba            #+#    #+#             */
/*   Updated: 2025/03/17 11:59:41 by lgamba           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include <shell/shell.h>

static void
	move_left(t_getline *line, int num)
{
	t_u8_iterator	it;

	it = it_new((t_string){line->input.buffer.str, line->input.buffer.len});
	it_next(&it);
	while (it.codepoint.len && it.byte_next < line->cursor_index)
		it_next(&it);
	while (!line->input.s_clusters.data[it.cp_pos].width)
		it_prev(&it);
	while (--num)
	{
		while (!line->input.s_clusters.data[it.cp_pos].width)
			it_prev(&it);
		if (num)
			it_prev(&it);
	}
	line->cursor_index = it.byte_pos;
	line->input.cp_pos = SIZE_MAX;
}

static void
	move_right(t_getline *line, int num)
{
	t_u8_iterator	it;

	it = it_new((t_string){line->input.buffer.str, line->input.buffer.len});
	it_next(&it);
	while (it.codepoint.len && it.byte_pos <= line->cursor_index)
		it_next(&it);
	while (it.cp_pos < line->input.s_clusters.size
			&& !line->input.s_clusters.data[it.cp_pos].width)
		it_next(&it);
	while (--num)
	{
		while (it.cp_pos < line->input.s_clusters.size
				&& !line->input.s_clusters.data[it.cp_pos].width)
			it_next(&it);
		if (num)
			it_next(&it);
	}
	line->cursor_index = it.byte_pos;
	line->input.cp_pos = SIZE_MAX;
}

void
	getline_input_move(t_getline *line, int offset)
{
	if (line->input.cp_len)
		return ;
	if (line->cursor_index != 0 && offset < 0)
		move_left(line, -offset);
	else if (offset > 0)
		move_right(line, offset);
	else if (line->scrolled)
		--line->scrolled;
	getline_redraw(line, 0);
}

void
	getline_input_move_word(t_getline *line, int direction)
{
	t_u8_iterator	it;
	t_u8_iterator	end;

	it = it_new((t_string){line->input.buffer.str, line->input.buffer.len});
	it_next(&it);
	while (it.codepoint.len && it.byte_pos < line->cursor_index)
		it_next(&it);
	end = line->boundaries_fn(line, it, direction);
	line->cursor_index = end.byte_pos;
	line->input.cp_pos = SIZE_MAX;
	getline_redraw(line, 0);
}

void
	getline_input_move_at(t_getline *line, size_t pos)
{
	t_u8_iterator	it;

	it = it_new((t_string){line->input.buffer.str, line->input.buffer.len});
	it_next(&it);
	while (it.codepoint.len && it.byte_pos < pos)
		it_next(&it);
	line->cursor_index = it.byte_pos;
	line->input.cp_pos = SIZE_MAX;
	getline_redraw(line, 0);
}
