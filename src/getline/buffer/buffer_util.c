/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   buffer_util.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgamba <linogamba@pundalik.org>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 11:59:40 by lgamba            #+#    #+#             */
/*   Updated: 2025/03/17 11:59:41 by lgamba           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "ft_printf.h"
#include <shell/shell.h>

/** @brief Replaces invalid codepoint with 0xFFFD, this still allow unmapped
 * codepoints, but prevents unicode of malformed length */
static void
	check_utf8(t_getline *line, size_t start, size_t end)
{
	const size_t	len = u8_length(line->input.buffer.str[start]);

	if (end - start == len)
		return ;
	asm("int $3");
	ft_dprintf(2, "start=%zu end=%zu\n\r", start, end);
	exit(1);
	stringbuf_replace(&line->input.buffer, start, end, "\uFFFD");
}

void
	getline_buffer_set_content(t_buffer *buf, const char *str)
{
	t_u8_iterator	it;

	getline_buffer_free(buf);
	it = it_new((t_string){str, ft_strlen(str)});
	it_next(&it);
	while (it.codepoint.len)
	{
		getline_insert_cluster(buf, it.cp_pos, (t_cluster){
			.size = it.codepoint.len,
			.width = codepoint_width(u8_to_cp(it.codepoint))
		});
		it_next(&it);
	}
	buf->buffer = stringbuf_from(str);
}

void
	getline_buffer_insert(t_getline *line, int c)
{
	char	buf[2];

	buf[0] = c;
	buf[1] = 0;
	if (!line->input.cp_len)
	{
		if (line->input.cp_pos != SIZE_MAX)
			check_utf8(line, line->input.cp_pos, line->cursor_index);
		line->input.cp_pos = line->cursor_index;
		line->input.cp_len = u8_length(c);
	}
	stringbuf_replace(&line->input.buffer, line->cursor_index,
		line->cursor_index, buf);
	line->cursor_index += 1;
	--line->input.cp_len;
}

void
	getline_buffer_recluster(t_getline *line, t_buffer *buf)
{
	t_string_buffer	content;
	t_u8_iterator	it;
	t_u8_iterator	it2;

	content = buf->buffer;
	stringbuf_init(&buf->buffer, content.len);
	free(buf->s_clusters.data);
	buf->s_clusters.data = NULL;
	buf->s_clusters.size = 0;
	buf->s_clusters.capacity = 0;
	it = it_new((t_string){content.str, content.len});
	it_next(&it);
	while (it.codepoint.len)
	{
		stringbuf_append(&buf->buffer, it.codepoint);
		it2 = it_new((t_string){buf->buffer.str,
				buf->buffer.len});
		it_next(&it2);
		it_advance(&it2, it.byte_pos);
		getline_recluster(line, buf, it2);
		it_next(&it);
	}
	stringbuf_free(&content);
}

void
	getline_buffer_replace(
	t_buffer *buf,
	size_t start,
	size_t end,
	const char *content)
{
	t_u8_iterator	it;

	free(buf->s_attrs.data);
	buf->s_attrs.data = NULL;
	buf->s_attrs.size = 0;
	buf->s_attrs.capacity = 0;
	free(buf->s_clusters.data);
	buf->s_clusters.data = NULL;
	buf->s_clusters.size = 0;
	buf->s_clusters.capacity = 0;
	stringbuf_replace(&buf->buffer, start, end, content);
	it = it_new((t_string){buf->buffer.str, buf->buffer.len});
	it_next(&it);
	while (it.codepoint.len)
	{
		getline_insert_cluster(buf, it.cp_pos, (t_cluster){
			.size = it.codepoint.len,
			.width = codepoint_width(u8_to_cp(it.codepoint))
		});
		it_next(&it);
	}
}
