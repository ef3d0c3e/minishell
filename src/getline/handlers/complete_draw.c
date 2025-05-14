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

	if ((size_t)line->comp_state.sel == i)
		ft_dprintf(line->out_fd, "\x1b[7m");
	ft_dprintf(line->out_fd, "\x1b[37m");
	w = draw_bounded(line, item->name, line->comp_state.col_width - 4, "…");
	while (desc_len + w++ < line->comp_state.col_width - 4)
		ft_dprintf(line->out_fd, " ");
	ft_dprintf(line->out_fd, "\x1b[33m");
	ft_dprintf(line->out_fd, "(");
	w += 3;
	draw_bounded(line, item->desc, line->comp_state.col_width - w, "…");
	ft_dprintf(line->out_fd, ")\x1b[m");
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

	i = 0;
	x = 1;
	y = 0;
	while (i < line->comp_state.nitems)
	{
		item = &line->comp_state.items[i];
		if (line->comp_state.start_row >= line->comp_state.cur_y)
			getline_cursor_set(line, x, line->comp_state.start_row + y);
		else
			getline_cursor_set(line, x, line->comp_state.end_row - y);
		line->comp_draw_item_fn(line, i, item);
		if (x + line->comp_state.col_width >= line->comp_state.width)
		{
			x = 1;
			++y;
		}
		else
			x += line->comp_state.col_width;
		++i;
	}
}
