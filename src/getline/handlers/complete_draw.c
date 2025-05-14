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
#include "ft_printf.h"
#include "getline/getline.h"
#include "util/util.h"
#include <shell/shell.h>
#include <stdlib.h>

static int
	draw_bounded(t_getline *line, const char *str, int maxwidth, const char *tr)
{
	int				w;
	int				len;
	t_u8_iterator	it;

	it = it_new((t_string){str, ft_strlen(str)});
	it_next(&it);
	w = 0;
	while (it.codepoint.len)
	{
		len = codepoint_width(u8_to_cp(it.codepoint));
		if (w + len > maxwidth)
		{
			write(line->out_fd, tr, ft_strlen(tr));
			break ;
		}
		write(line->out_fd, it.codepoint.str, it.codepoint.len);
		w += len;
		it_next(&it);
	}
	return (w);
}

void
	getline_handler_comp_draw_item(
	t_getline *line,
	size_t i,
	const t_complete_item *item)
{
	const int		desc_len = string_width(item->desc);
	int				w;

	if ((size_t)line->comp_state.sel == i)
		ft_dprintf(line->out_fd, "\x1b[7m");
	ft_dprintf(line->out_fd, "\x1b[37m");
	w = draw_bounded(line, item->name, line->comp_state.col_width - 4, "…");
	while (desc_len + w++ < line->comp_state.col_width - 4)
		ft_dprintf(line->out_fd, " ");
	if (item->desc)
	{
		ft_dprintf(line->out_fd, "\x1b[33m");
		ft_dprintf(line->out_fd, "(");
		w += 3;
		draw_bounded(line, item->desc, line->comp_state.col_width - w, "…");
		ft_dprintf(line->out_fd, ")");
	}
	ft_dprintf(line->out_fd, "\x1b[m");
}

static size_t
	update_scroll(t_getline *line)
{
	int		rows;
	int		ncols;
	int		y;

	rows = (line->comp_state.end_row - line->comp_state.start_row);
	if (rows < 0)
		rows *= -1;
	ncols = line->display_width / line->comp_state.col_width;
	if (!ncols)
		ncols = 1;
	y = line->comp_state.sel / ncols;
	if (y - line->comp_state.scrolled < 0)
		line->comp_state.scrolled = y;
	else if (y - line->comp_state.scrolled >= rows)
		line->comp_state.scrolled = y - rows + 1;
	return (line->comp_state.scrolled * (ncols));
}

void
	getline_complete_redraw(t_getline *line)
{
	size_t					i;
	int						x;
	int						y;
	const t_complete_item	*item;

	if (line->comp_state.col_width < 5)
		return ;
	if (line->comp_state.sel < 0)
	{
		line->comp_state.sel %= line->comp_state.nitems;
		line->comp_state.sel *= -1;
	}
	else if ((size_t)line->comp_state.sel > line->comp_state.nitems)
		line->comp_state.sel %= line->comp_state.nitems;
	i = update_scroll(line);
	x = 1;
	y = 0;
	getline_cursor_set(line, 1, line->comp_state.start_row);
	while (i < line->comp_state.nitems)
	{
		item = &line->comp_state.items[i];
		line->comp_draw_item_fn(line, i, item);
		if (x + line->comp_state.col_width >= line->comp_state.width)
		{
			x = 1;
			++y;
			ft_dprintf(line->out_fd, "\n\r")	;
			if (y + line->comp_state.start_row + 1 > line->comp_state.end_row)
				break ;
		}
		else
		{
			x += line->comp_state.col_width;
			ft_dprintf(line->out_fd, " ");
		}
		++i;
	}
	getline_cursor_set(line, line->comp_state.cur_x, line->comp_state.cur_y);
}
