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
#include <shell/shell.h>

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

	if ((size_t)line->state.comp.sel == i)
		ft_dprintf(line->out_fd, "\x1b[7m");
	ft_dprintf(line->out_fd, "\x1b[37m");
	w = draw_bounded(line, item->name, line->state.comp.col_width - 4, "…");
	while (desc_len + w++ < line->state.comp.col_width - 4)
		ft_dprintf(line->out_fd, " ");
	if (item->desc)
	{
		ft_dprintf(line->out_fd, "\x1b[33m");
		ft_dprintf(line->out_fd, "(");
		w += 3;
		draw_bounded(line, item->desc, line->state.comp.col_width - w, "…");
		ft_dprintf(line->out_fd, ")");
	}
	ft_dprintf(line->out_fd, "\x1b[m");
}

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
	getline_complete_row_indicator(t_getline *line)
{
	int	ncols;
	int	nrows;
	
	ncols = line->display_width / line->state.comp.col_width;
	if (!ncols)
		ncols = 0;
	nrows = line->state.comp.nitems / ncols;
	if (line->state.comp.nitems % ncols)
		++nrows;
	if (line->state.comp.sel == -1)
	{
		ft_dprintf(line->out_fd, "\n\r\x1b[46mrows %d to %d of %d\x1b[m",
			line->state.comp.scrolled + 1,
			line->state.comp.scrolled + (line->state.comp.end_row
			- line->state.comp.start_row) + 1,
			nrows);
		return ;
	}
	ft_dprintf(line->out_fd, "\n\r\x1b[46m%d/%d (rows %d to %d of %d)\x1b[m",
			line->state.comp.sel + 1,
			line->state.comp.nitems,
			line->state.comp.scrolled + 1,
			line->state.comp.scrolled + (line->state.comp.end_row
			- line->state.comp.start_row) + 1,
			nrows);
}

void
	getline_complete_draw(t_getline *line, int update)
{
	size_t					i;
	int						x;
	int						y;
	const t_complete_item	*item;

	if (line->state.comp.col_width < 5)
		return ;
	i = update_scroll_sel(line);
	x = 1;
	y = 0;
	getline_cursor_set(line, 1, line->state.comp.start_row);
	while (i < line->state.comp.nitems)
	{
		item = &line->state.comp.items[i];
		line->comp_draw_item_fn(line, i, item);
		if (x + line->state.comp.col_width >= line->display_width)
		{
			x = 1;
			if (++y + line->state.comp.start_row > line->state.comp.end_row)
				break ;
			ft_dprintf(line->out_fd, "\n\r");
		}
		else
		{
			x += line->state.comp.col_width;
			ft_dprintf(line->out_fd, " ");
		}
		++i;
	}
	ft_dprintf(line->out_fd, "\x1b[0J");
	getline_complete_row_indicator(line);
	getline_cursor_set(line, line->state.comp.cur_x, line->state.comp.cur_y);
	getline_input_draw(line, 0);
}
