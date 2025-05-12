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
#include "getline/getline.h"
#include <shell/shell.h>

t_buffer
	getline_buffer_new(void)
{
	return (t_buffer){
		.buffer.str = NULL,
		.buffer.capacity = 0,
		.buffer.len = 0,
		.cp_len = 0,
		.cp_pos = SIZE_MAX,
		.s_attrs.data = NULL,
		.s_attrs.capacity = 0,
		.s_attrs.size = 0,
		.s_clusters.data = NULL,
		.s_clusters.size = 0,
		.s_clusters.capacity = 0,
	};
}

void
	getline_buffer_free(t_buffer *buf)
{
	stringbuf_free(&buf->buffer);
	free(buf->s_attrs.data);
	free(buf->s_clusters.data);
}

/** @brief Replaces invalid codepoint with 0xFFFD, this still allow unmapped
 * codepoints, but prevents unicode of malformed length */
static void
	check_utf8(t_getline *line, size_t start, size_t end)
{
	const size_t	len = u8_length(line->buffer.buffer.str[start]);

	if (end - start == len)
		return ;
	stringbuf_replace(&line->buffer.buffer, start, end, "\uFFFD");
}

void
	getline_buffer_insert(t_getline *line, int c)
{
	char	buf[2];

	buf[0] = c;
	buf[1] = 0;

	if (!line->buffer.cp_len)
	{
		if (line->buffer.cp_pos != SIZE_MAX)
			check_utf8(line, line->buffer.cp_pos, line->cursor_index);
		line->buffer.cp_pos = line->cursor_index;
		line->buffer.cp_len = u8_length(c);
	}
	stringbuf_replace(&line->buffer.buffer, line->cursor_index,
			line->cursor_index, buf);
	line->cursor_index += 1;
	--line->buffer.cp_len;
}
