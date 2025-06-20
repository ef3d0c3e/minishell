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

static int
	hist_filter_next(t_getline *line, int sc, int direction)
{
	const int			saved = sc;
	const t_history_ent	*ent;

	if (sc == -2)
		return (sc);
	while (direction > 0 && ++sc < (int)line->history.num_entries)
	{
		ent = &line->history.entries[line->history.num_entries - sc - 1];
		if (str_starts_with((t_string){ent->input, ft_strlen(ent->input)},
			line->state.hist.filter))
			break ;
	}
	while (direction < 0 && --sc > 0)
	{
		ent = &line->history.entries[line->history.num_entries - sc - 1];
		if (str_starts_with((t_string){ent->input, ft_strlen(ent->input)},
			line->state.hist.filter))
			break ;
	}
	if (sc == (int)line->history.num_entries && saved == -1)
		return (-1);
	if (sc == (int)line->history.num_entries)
		return (-2);
	return (sc);
}

static int
	next_hist_index(t_getline *line, int offset)
{
	int	sc;

	sc = line->state.hist.scroll_index;
	if (!line->state.hist.filter)
		return (sc + offset);
	while (offset)
	{
		sc = hist_filter_next(line, sc, offset);
		if (sc < 0)
			break ;
		if (offset > 0)
			--offset;
		else
			++offset;
	}
	return (sc);
}

static void
	hist_highlight(t_getline *l)
{
	t_buffer_attr	attrs;
	const size_t	len = ft_strnlen(l->state.hist.filter,
			!!l->state.hist.filter * SIZE_MAX);

	free(l->input.s_attrs.data);
	l->input.s_attrs.data = NULL;
	l->input.s_attrs.size = 0;
	l->input.s_attrs.capacity = 0;
	ft_memset(&attrs, 0, sizeof(t_buffer_attr));
	if (len)
	{
		attrs.underline = -1;
		attrs.color = 0xff7f10;
		attrs.start = 0;
		attrs.end = len;
		getline_highlight_add(&l->input, attrs);
	}
	attrs.underline = 0;
	attrs.color = 0x7f7f7f;
	attrs.start = len;
	attrs.end = l->input.buffer.len;
	getline_highlight_add(&l->input, attrs);
}

void
	getline_history_move(t_getline *line, int offset)
{
	int	sc;

	if (!line->history.num_entries)
		return ;
	sc = next_hist_index(line, offset);
	if ((int)line->history.num_entries - sc < 0)
		sc = 0;
	else if (sc < 0)
	{
		if (sc == -1)
			getline_history_cancel(line);
		return ;
	}
	line->state.hist.scroll_index = sc;
	getline_buffer_set_content(&line->input,
		line->history.entries[line->history.num_entries
		- line->state.hist.scroll_index - 1].input);
	hist_highlight(line);
	line->scrolled = 0;
	line->cursor_index = line->input.buffer.len;
}

void
	getline_history_cancel(t_getline *line)
{
	line->state.hist.accept = 0;
	getline_change_mode(line, LINE_INPUT);
}
