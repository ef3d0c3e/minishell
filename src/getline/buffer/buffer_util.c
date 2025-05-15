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
#include <shell/shell.h>

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
