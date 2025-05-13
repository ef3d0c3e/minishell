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
#include <shell/shell.h>

/*
static void
	draw(t_getline *line, t_complete_draw *dr, comp_fn fn)
{
	int						i;
	const t_complete_item	*item;

	i = 0;
	while (1)
	{
		item = fn(line, i);
		if (!item)
			break ;
		ft_dprintf(line->out_fd, "\x1b[%d;%dH", dr->start_row + i, 1);
		ft_dprintf(line->out_fd, "%s (%s)", item->name, item->desc);
		++i;
	}
}
*/

void
	getline_handler_draw_comp_item(
	t_getline *line,
	const t_complete_item *item)
{

}

static const t_complete_item
	*get_item(t_getline *line, size_t i)
{
	static const t_complete_item items[] = {
		{COMPLETE_WORD, "test123", "this is a test"},
		{COMPLETE_FILE, "a.out", "file"},
		{COMPLETE_FILE, "b.out", "1"},
		{COMPLETE_FILE, "c.out", "2"},
	};

	if (i >= sizeof(items) / sizeof(items[0]))
		return (NULL);
	return (&items[i]);
}

void
	getline_complete_menu(t_getline *line)
{
	int				cur_y;
	int				cur_x;
	struct winsize	ws;

	// decide where to draw
	ioctl(line->out_fd, TIOCGWINSZ, &ws);
	line->comp_state.width = ws.ws_col;
	line->comp_state.height = ws.ws_row;
	getline_cursor_pos(line, &cur_x, &cur_y);
	
	if (cur_y >= ws.ws_row / 2)
	{
		line->comp_state.start_row = 1;
		line->comp_state.end_row = cur_y - 1;
	}
	else
	{
		line->comp_state.start_row = cur_y + 1;
		line->comp_state.end_row = ws.ws_row;
	}
	line->comp_state.col_width = line->comp_state.width / 60;
	if (!line->comp_state.col_width)
		line->comp_state.col_width = line->comp_state.width;
	//draw(line, &dr, get_item);
	line->comp_state.shown = 1;
	//ft_dprintf(line->out_fd, "\x1b[%d;%dH", cur_y, cur_x);
}
