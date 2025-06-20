/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   overfllow.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgamba <linogamba@pundalik.org>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 11:59:40 by lgamba            #+#    #+#             */
/*   Updated: 2025/03/17 11:59:41 by lgamba           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include <shell/shell.h>

void
	getline_handler_overflow(t_getline *line, t_drawline *dr, int right)
{
	(void)line;
	if (right)
		stringbuf_append_s(&dr->buf, ">");
	else
		stringbuf_append_s(&dr->buf, "<");
}
