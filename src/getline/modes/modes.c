/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   modes.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgamba <linogamba@pundalik.org>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 11:59:40 by lgamba            #+#    #+#             */
/*   Updated: 2025/03/17 11:59:41 by lgamba           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include <shell/shell.h>

void
	getline_setup_modes(t_getline *line)
{
	getline_setup_input_mode(&line->modes[LINE_INPUT]);
	getline_setup_complete_mode(&line->modes[LINE_TAB]);
	getline_setup_history_mode(&line->modes[LINE_HISTSCROLL]);
}

void
	getline_change_mode(t_getline *line, int mode)
{
	if ((int)line->mode == mode)
		return ;
	if (line->modes[line->mode].disable_mode_fn)
		line->modes[line->mode].disable_mode_fn(line);
	line->mode = mode;
	if (line->modes[line->mode].enable_mode_fn)
		line->modes[line->mode].enable_mode_fn(line);
	getline_redraw(line, 1);
}
