/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   move.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgamba <linogamba@pundalik.org>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 11:59:40 by lgamba            #+#    #+#             */
/*   Updated: 2025/03/17 11:59:41 by lgamba           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "getline/getline.h"
#include "util/util.h"
#include <shell/shell.h>

static void
	move_left(t_getline *line, int num)
{
	t_u8_iterator	it;

	it = it_new((t_string){line->buffer.buffer.str, line->buffer.buffer.len});
	it_next(&it);
	while (it.codepoint.len && it.byte_next < line->cursor_index)
		it_next(&it);
	while (!line->buffer.s_clusters.data[it.cp_pos].width)
		it_prev(&it);
	while (--num)
	{
		while (!line->buffer.s_clusters.data[it.cp_pos].width)
			it_prev(&it);
		if (num)
			it_prev(&it);
	}
	line->cursor_index = it.byte_pos;
	line->buffer.cp_pos = SIZE_MAX;
}

static void
	move_right(t_getline *line, int num)
{
	t_u8_iterator	it;

	it = it_new((t_string){line->buffer.buffer.str, line->buffer.buffer.len});
	it_next(&it);
	while (it.codepoint.len && it.byte_pos <= line->cursor_index)
		it_next(&it);
	while (it.cp_pos < line->buffer.s_clusters.size
			&& !line->buffer.s_clusters.data[it.cp_pos].width)
		it_next(&it);
	while (--num)
	{
		while (it.cp_pos < line->buffer.s_clusters.size
				&& !line->buffer.s_clusters.data[it.cp_pos].width)
			it_next(&it);
		if (num)
			it_next(&it);
	}
	line->cursor_index = it.byte_pos;
	line->buffer.cp_pos = SIZE_MAX;
}

void
	getline_move(t_getline *line, int offset)
{
	if (line->buffer.cp_len)
		return ;
	if (line->cursor_index != 0 && offset < 0)
		move_left(line, -offset);
	else if (offset > 0)
		move_right(line, offset);
	else if (line->render.scrolled)
		--line->render.scrolled;
	getline_redraw(line, 0);
}
