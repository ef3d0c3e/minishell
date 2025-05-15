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
#include "getline/buffer.h"
#include <shell/shell.h>

void
	getline_history_move(t_getline *line, int offset)
{
	int	sc;

	sc = line->state.hist.scroll_index + offset;
	if (line->history.num_entries - sc < 0)
		sc = 0;
	else if (sc < 0)
	{
		getline_change_mode(line, LINE_INPUT);
		return ;
	}
	line->state.hist.scroll_index = sc;
	getline_buffer_set_content(&line->input,
		line->history.entries[line->history.num_entries
		- line->state.hist.scroll_index - 1].input);
}
