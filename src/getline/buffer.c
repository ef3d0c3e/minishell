/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   buffer.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgamba <linogamba@pundalik.org>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 11:59:40 by lgamba            #+#    #+#             */
/*   Updated: 2025/03/17 11:59:41 by lgamba           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include <shell/shell.h>

t_buffer
	getline_buffer_new(void)
{
	return (t_buffer){
		.buffer.str = NULL,
		.buffer.capacity = 0,
		.buffer.len = 0,
		.s_attrs.data = NULL,
		.s_attrs.capacity = 0,
		.s_attrs.size = 0,
	};
}

// TODO: Before returning the line, perform utf-8 check on the input for invalid codepoints
void
	getline_buffer_insert(t_getline *line, int c)
{
	char	buf[2];
	buf[0] = c;
	buf[1] = 0;
	stringbuf_replace(&line->buffer.buffer, line->cursor_index,
			line->cursor_index, buf);
	line->cursor_index += 1;
}
