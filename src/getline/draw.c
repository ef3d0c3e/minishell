/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgamba <linogamba@pundalik.org>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 11:59:40 by lgamba            #+#    #+#             */
/*   Updated: 2025/03/17 11:59:41 by lgamba           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include <shell/shell.h>

void
	getline_redraw(t_getline *l, int update)
{
	struct winsize	ws;

	if (update)
	{
		ioctl(l->out_fd, TIOCGWINSZ, &ws);
		l->display_width = ws.ws_col;
		l->display_height = ws.ws_row;
	}
	l->modes[l->mode].draw_mode_fn(l, update);
}
