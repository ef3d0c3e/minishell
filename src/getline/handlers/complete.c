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
#include "getline/getline.h"
#include <shell/shell.h>

/** @brief Frees completion items */
static void
	free_items(t_getline *line)
{
	size_t	i;

	i = 0;
	while (line->comp_state.items && line->comp_state.items[i].name)
	{
		free(line->comp_state.items[i].name);
		free(line->comp_state.items[i].desc);
		++i;
	}
	free(line->comp_state.items);
	line->comp_state.items = NULL;
	line->comp_state.nitems = 0;
}

/** @brief Populates the list of completion items */
static void
	populate_items(t_getline *line)
{
	if (!line->comp_provider_fn)
		return ;
	free_items(line);
	line->comp_state.items = line->comp_provider_fn(line);
	line->comp_state.nitems = 0;
	while (line->comp_state.items[line->comp_state.nitems].name)
		++line->comp_state.nitems;
}

void
	getline_complete_menu(t_getline *line)
{
	struct winsize	ws;

	populate_items(line);
	if (!line->comp_state.nitems)
	{
		free_items(line);
		return ;
	}
	ioctl(line->out_fd, TIOCGWINSZ, &ws);
	line->display_width = ws.ws_col;
	line->display_height = ws.ws_row;
	getline_cursor_pos(line, &line->comp_state.cur_x, &line->comp_state.cur_y);
	ft_dprintf(line->out_fd, "\x1b[?1049h");
	line->comp_state.start_row = 1;
	line->comp_state.end_row = ws.ws_row - 1;
	line->comp_state.col_width = line->display_width / 60;
	if (!line->comp_state.col_width)
		line->comp_state.col_width = line->display_width;
	else
		line->comp_state.col_width = line->display_width
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
	free_items(line);
	ft_dprintf(line->out_fd, "\x1b[?1049l");
	ft_dprintf(line->out_fd, "\x1b[%d;%dH", line->comp_state.cur_y,
			line->comp_state.cur_x);
	line->comp_state.shown = 0;
}
