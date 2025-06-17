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

static void
	complete_draw_items(t_getline *line)
{
	size_t					i;
	int						x;
	int						y;

	i = update_scroll_sel(line);
	x = 1;
	y = 0;
	while (i < line->state.comp.nitems)
	{
		line->comp_draw_item_fn(line, i, &line->state.comp.items[i]);
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
}

void
	getline_complete_draw(t_getline *line, int update)
{
	(void)update;
	if (line->state.comp.col_width < 5)
		return ;
	getline_cursor_set(line, 1, line->state.comp.start_row);
	complete_draw_items(line);
	ft_dprintf(line->out_fd, "\x1b[0J");
	getline_complete_row_indicator(line);
	getline_cursor_set(line, line->state.comp.cur_x, line->state.comp.cur_y);
	getline_input_draw(line, 0);
}
