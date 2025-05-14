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
	l->comp_state.sel += offset;
	if (l->comp_state.nitems)
	{
		if (l->comp_state.sel < 0)
			l->comp_state.sel = l->comp_state.nitems - 1;
		else if ((size_t)l->comp_state.sel >= l->comp_state.nitems)
			l->comp_state.sel = 0;
	}
	getline_redraw(l, 0);
}

void
	getline_complete_move_row(t_getline *l, int offset)
{
	const int	ncols = l->display_width / l->comp_state.col_width;

	if (ncols)
		getline_complete_move(l, -ncols * offset);
	else
		getline_complete_move(l, -offset);
}

void
	getline_complete_move_page(t_getline *l, int offset)
{
	const int	ncols = l->display_width / l->comp_state.col_width;
	int			nrows;

	nrows = l->comp_state.end_row - l->comp_state.start_row;
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
	getline_complete_menu_hide(line);
}
