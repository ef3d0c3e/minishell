/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   history_draw.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgamba <linogamba@pundalik.org>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 11:59:40 by lgamba            #+#    #+#             */
/*   Updated: 2025/03/17 11:59:41 by lgamba           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include <shell/shell.h>

void getline_history_draw(t_getline *line, int update)
{
	line->modes[LINE_INPUT].draw_mode_fn(line, 0);
}
