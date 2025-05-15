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
#include <shell/shell.h>

void
	getline_complete_move(t_getline *l, int offset)
{
	l->state.comp.sel += offset;
	if (l->state.comp.nitems)
	{
		if (l->state.comp.sel < 0)
			l->state.comp.sel = l->state.comp.nitems - 1;
		else if ((size_t)l->state.comp.sel >= l->state.comp.nitems)
			l->state.comp.sel = 0;
	}
}

void
	getline_complete_move_row(t_getline *l, int offset)
{
	const int	ncols = l->display_width / l->state.comp.col_width;

	if (ncols)
		getline_complete_move(l, -ncols * offset);
	else
		getline_complete_move(l, -offset);
}

void
	getline_complete_move_page(t_getline *l, int offset)
{
	const int	ncols = l->display_width / l->state.comp.col_width;
	int			nrows;

	nrows = l->state.comp.end_row - l->state.comp.start_row;
	if (nrows < 0)
		nrows *= -1;
	if (ncols)
		getline_complete_move(l, -nrows * ncols * offset);
	else
		getline_complete_move(l, -nrows * offset);
}

void
	getline_complete_select(t_getline *line)
{
	// TODO
	getline_change_mode(line, LINE_INPUT);
}
