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
		ft_dprintf(line->out_fd, "(C-c)");
		getline_buffer_free(&line->input);
		line->scrolled = 0;
		line->cursor_index = 0;
		line->state.action = ACT_NONE;
		g_signal = SIGINT;
		return (1);
	}
	return (line->state.action != ACT_NONE);
}
