/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   string_buffer_create.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgamba <linogamba@pundalik.org>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 11:59:40 by lgamba            #+#    #+#             */
/*   Updated: 2025/03/17 11:59:41 by lgamba           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "../util.h"

void
	stringbuf_init(t_string_buffer *buf, size_t initial_capacity)
{
	buf->str = xmalloc(initial_capacity);
	buf->len = 0;
	buf->capacity = initial_capacity;
}

t_string_buffer
	stringbuf_from(const char *msg)
{
	t_string_buffer	buffer;

	buffer.capacity = ft_strlen(msg);
	buffer.capacity += !buffer.capacity;
	buffer.len = buffer.capacity;
	buffer.str = ft_memcpy(xmalloc(buffer.capacity), msg, buffer.len);
	return (buffer);
}

t_string_buffer
	stringbuf_from_owned(char *msg)
{
	t_string_buffer	buffer;

	buffer.capacity = ft_strlen(msg) + 1;
	buffer.len = buffer.capacity - 1;
	buffer.str = msg;
	return (buffer);
}

t_string_buffer
	stringbuf_from_range(const char *start, const char *end)
{
	t_string_buffer	buf;

	stringbuf_init(&buf, end - start);
	buf.len = end - start;
	ft_memcpy(buf.str, start, buf.len);
	return (buf);
}
