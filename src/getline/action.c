/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   action.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgamba <linogamba@pundalik.org>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 11:59:40 by lgamba            #+#    #+#             */
/*   Updated: 2025/03/17 11:59:41 by lgamba           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include <getline/getline.h>
#include <shell/shell.h>

int
	getline_process_action(t_getline *line)
{
	if (line->state.action == ACT_QUIT)
	{
		line->state.action = ACT_NONE;
		return (line->input.buffer.len == 0);
	}
	if (line->state.action == ACT_CANCEL)
	{
		ft_dprintf(line->out_fd, "(C-c)\n\r");
		getline_buffer_free(&line->input);
		line->scrolled = 0;
		line->cursor_index = 0;
		getline_redraw(line, 1);
		line->shell->last_status = 130;
		line->state.action = ACT_NONE;
		return (0);
	}
	return (line->state.action != ACT_NONE);
}
