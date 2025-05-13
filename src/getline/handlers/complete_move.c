/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   complete_move.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgamba <linogamba@pundalik.org>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 11:59:40 by lgamba            #+#    #+#             */
/*   Updated: 2025/03/17 11:59:41 by lgamba           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "getline/getline.h"
#include <shell/shell.h>

void
	getline_complete_move(t_getline *l, int offset)
{
	//if (offset < 0 && (size_t)-offset > l->comp_state.sel)
	//	l->comp_state.sel = 0;
	//else
		l->comp_state.sel += offset;
	getline_redraw(l, 0);
}

void
	getline_complete_move_row(t_getline *l, int offset)
{
	const int	ncols = l->comp_state.width / l->comp_state.col_width;

	if (ncols)
		getline_complete_move(l, ncols * offset);
	else
		getline_complete_move(l, offset);
}
