/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input_actions.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgamba <linogamba@pundalik.org>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 11:59:40 by lgamba            #+#    #+#             */
/*   Updated: 2025/03/17 11:59:41 by lgamba           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include <shell/shell.h>

void
	getline_input_action(t_getline *line, int action)
{
	if (action == ACT_CLEAR_SCREEN)
	{
		if (line->shell->cap.strs.clear)
		{
			ft_dprintf(line->out_fd, "%s", line->shell->cap.strs.clear);
			getline_cursor_set(line, 1, 1);
		}
		return ;
	}
	line->state.action = action;
}
