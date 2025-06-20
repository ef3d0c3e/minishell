/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   complete_draw.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgamba <linogamba@pundalik.org>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 11:59:40 by lgamba            #+#    #+#             */
/*   Updated: 2025/03/17 11:59:41 by lgamba           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "util/util.h"
#include <shell/shell.h>

static size_t
	update_scroll_sel(t_getline *line)
{
	int		rows;
	int		ncols;
	int		y;

	if (line->state.comp.sel < 0)
		return (0);
	else if ((size_t)line->state.comp.sel > line->state.comp.nitems)
		line->state.comp.sel %= line->state.comp.nitems;
	rows = (line->state.comp.end_row - line->state.comp.start_row);
	if (rows < 0)
		rows *= -1;
	ncols = line->display_width / line->state.comp.col_width;
	if (!ncols)
		ncols = 1;
	y = line->state.comp.sel / ncols;
	if (y - line->state.comp.scrolled < 0)
		line->state.comp.scrolled = y;
	else if (y - line->state.comp.scrolled > rows)
		line->state.comp.scrolled = y - rows;
	return (line->state.comp.scrolled * (ncols));
}

static void
	getline_complete_row_indicator(t_getline *line, t_string_buffer *buf)
{
	if (line->state.comp.sel == -1)
	{
		stringbuf_append_s(buf, "\n\r\x1b[46m(");
		stringbuf_append_i(buf, line->state.comp.nitems);
		stringbuf_append_s(buf, " items)\x1b[m");
		return ;
	}
	stringbuf_append_s(buf, "\n\r\x1b[46m");
	stringbuf_append_i(buf, line->state.comp.sel + 1);
	stringbuf_append_s(buf, "/");
	stringbuf_append_i(buf, line->state.comp.nitems);
	stringbuf_append_s(buf, " items\x1b[m");
}

static void
	complete_draw_items(t_getline *line, t_string_buffer *buf)
{
	size_t					i;
	int						x;
	int						y;

	i = update_scroll_sel(line);
	x = 1;
	y = 0;
	while (i < line->state.comp.nitems)
	{
		line->comp_draw_item_fn(line, buf, i, &line->state.comp.items[i]);
		if (x + line->state.comp.col_width >= line->display_width)
		{
			x = 1;
			if (++y + line->state.comp.start_row > line->state.comp.end_row)
				break ;
			stringbuf_append_s(buf, "\n\r");
		}
		else
		{
			x += line->state.comp.col_width;
			stringbuf_append_s(buf, " ");
		}
		++i;
	}
}

void
	getline_complete_draw(t_getline *line, int update)
{
	t_string_buffer	buf;

	stringbuf_init(&buf, 4096);
	(void)update;
	if (line->state.comp.col_width < 5)
		return ;
	complete_draw_items(line, &buf);
	stringbuf_append_s(&buf, "\x1b[0J");
	getline_complete_row_indicator(line, &buf);
	getline_cursor_set(line, line->state.comp.cur_x, line->state.comp.cur_y);
	getline_input_draw(line, 0);
	getline_cursor_set(line, 1, line->state.comp.start_row);
	write(line->out_fd, buf.str, buf.len);
	stringbuf_free(&buf);
}
