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
	return ((t_buffer){
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
	});
}

t_buffer
	getline_buffer_clone(const t_buffer *orig)
{
	t_buffer	buf;

	buf = *orig;
	buf.buffer = stringbuf_from_range(orig->buffer.str,
			orig->buffer.str + orig->buffer.len);
	buf.s_attrs.data = xmalloc(sizeof(t_buffer_attr) * buf.s_attrs.capacity);
	ft_memcpy(buf.s_attrs.data, orig->s_attrs.data,
		sizeof(t_buffer_attr) * buf.s_attrs.size);
	buf.s_clusters.data = xmalloc(sizeof(t_cluster) * buf.s_clusters.capacity);
	ft_memcpy(buf.s_clusters.data, orig->s_clusters.data,
		sizeof(t_cluster) * buf.s_clusters.size);
	return (buf);
}

void
	getline_buffer_free(t_buffer *buf)
{
	stringbuf_free(&buf->buffer);
	free(buf->s_attrs.data);
	free(buf->s_clusters.data);
	*buf = getline_buffer_new();
}
