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
#include "ft_printf.h"
#include <shell/shell.h>

void
	getline_complete_menu(t_getline *line)
{
	struct winsize	ws;

	ioctl(line->out_fd, TIOCGWINSZ, &ws);
	line->comp_state.width = ws.ws_col;
	line->comp_state.height = ws.ws_row;
	getline_cursor_pos(line, &line->comp_state.cur_x, &line->comp_state.cur_y);
	if (line->comp_state.cur_y >= ws.ws_row / 2)
	{
		line->comp_state.start_row = 1;
		line->comp_state.end_row = line->comp_state.cur_y - 1;
	}
	else
	{
		line->comp_state.start_row = line->comp_state.cur_y + 1;
		line->comp_state.end_row = ws.ws_row;
	}
	line->comp_state.col_width = line->comp_state.width / 60;
	if (!line->comp_state.col_width)
		line->comp_state.col_width = line->comp_state.width;
	else
		line->comp_state.col_width = line->comp_state.width
			/ line->comp_state.col_width;
	line->comp_state.shown = 1;
	line->comp_state.sel = 0;
	line->comp_state.scrolled = 0;
	getline_redraw(line, 1);
}

void
	getline_complete_menu_hide(t_getline *line)
{
	if (!line->comp_state.shown)
		return ;
	ft_dprintf(line->out_fd, "\x1b[%d;%dH", line->comp_state.cur_y,
			line->comp_state.cur_x);
	line->comp_state.shown = 0;
}
