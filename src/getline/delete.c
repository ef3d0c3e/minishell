/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   delete.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgamba <linogamba@pundalik.org>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 11:59:40 by lgamba            #+#    #+#             */
/*   Updated: 2025/03/17 11:59:41 by lgamba           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "util/util.h"
#include <shell/shell.h>

static void
	getline_delete_left(t_getline *line, int offset)
{
	t_u8_iterator	hi;
	t_u8_iterator	lo;
	int				i;

	if (!line->cursor_index)
		return ;
	hi = it_new((t_string){line->buffer.buffer.str, line->buffer.buffer.len});
	it_next(&hi);
	while (hi.codepoint.len && hi.byte_next < line->cursor_index)
		it_next(&hi);
	lo = hi;
	i = 1;
	while (i++ != offset)
		it_prev(&lo);
	getline_remove_cluster(&line->buffer, lo.cp_pos, hi.cp_pos + 1);
	stringbuf_replace(&line->buffer.buffer, lo.byte_pos, hi.byte_next, "");
	line->cursor_index = hi.byte_pos;
}

static void
	getline_delete_right(t_getline *line, int offset)
{
	t_u8_iterator	lo;
	t_u8_iterator	hi;
	int				i;

	if (line->cursor_index >= line->buffer.buffer.len)
		return ;
	lo = it_new((t_string){line->buffer.buffer.str, line->buffer.buffer.len});
	it_next(&lo);
	while (lo.codepoint.len && lo.byte_pos < line->cursor_index)
		it_next(&lo);
	hi = lo;
	i = 1;
	while (i++ != offset)
		it_next(&hi);
	getline_remove_cluster(&line->buffer, lo.cp_pos, hi.cp_pos + 1);
	stringbuf_replace(&line->buffer.buffer, lo.byte_pos, hi.byte_next, "");
	line->cursor_index = lo.byte_pos;
	line->buffer.cp_pos = SIZE_MAX;
	getline_redraw(line, 1);
}

void
	getline_delete(t_getline *line, int offset)
{
	if (offset < 0)
		getline_delete_left(line, -offset);
	else if (offset > 0)
		getline_delete_right(line, offset);
	line->buffer.cp_pos = SIZE_MAX;
	getline_redraw(line, 1);
}
