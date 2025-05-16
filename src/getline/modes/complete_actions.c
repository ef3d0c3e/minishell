/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   complete_actions.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgamba <linogamba@pundalik.org>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 11:59:40 by lgamba            #+#    #+#             */
/*   Updated: 2025/03/17 11:59:41 by lgamba           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "ft_printf.h"
#include "getline/buffer/buffer.h"
#include "getline/modes/modes.h"
#include <shell/shell.h>

static void
	update_line(t_getline *line)
{
	const t_complete_item	*item = &line->state.comp.items
		[line->state.comp.sel];
	//ft_dprintf(2, "start=%zu, end=%zu\n\r", line->state.comp.word_start, line->state.comp.word_end);
	getline_buffer_replace(&line->input, line->state.comp.word_start, line->state.comp.word_end, item->name);
	line->state.comp.word_end = line->state.comp.word_start + ft_strlen(item->name);
}

void
	getline_complete_move(t_getline *line, int offset)
{
	if (line->state.comp.mini_mode)
	{
		line->state.comp.mini_mode = 0;
		line->state.comp.cur_y = 1;
		line->state.comp.start_row = 2;
		ft_dprintf(line->out_fd, "\x1b[2J");
	}
	line->state.comp.sel += offset;
	if (line->state.comp.nitems)
	{
		if (line->state.comp.sel < 0)
			line->state.comp.sel = line->state.comp.nitems - 1;
		else if ((size_t)line->state.comp.sel >= line->state.comp.nitems)
			line->state.comp.sel = 0;
	}
	update_line(line);
}

void
	getline_complete_move_row(t_getline *line, int offset)
{
	const int	ncols = line->display_width / line->state.comp.col_width;

	if (ncols)
		getline_complete_move(line, -ncols * offset);
	else
		getline_complete_move(line, -offset);
}

void
	getline_complete_move_page(t_getline *line, int offset)
{
	const int	ncols = line->display_width / line->state.comp.col_width;
	int			nrows;

	nrows = line->state.comp.end_row - line->state.comp.start_row;
	if (nrows < 0)
		nrows *= -1;
	if (ncols)
		getline_complete_move(line, -nrows * ncols * offset);
	else
		getline_complete_move(line, -nrows * offset);
}

void
	getline_complete_select(t_getline *line)
{
	// TODO
	getline_buffer_recluster(line, &line->input);
	getline_change_mode(line, LINE_INPUT);
}
