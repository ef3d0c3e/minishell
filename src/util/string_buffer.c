/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   string_buffer.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgamba <linogamba@pundalik.org>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 11:59:40 by lgamba            #+#    #+#             */
/*   Updated: 2025/03/17 11:59:41 by lgamba           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "util.h"
#include <uchar.h>

void
	stringbuf_init(t_string_buffer *buf, size_t initial_capacity)
{
	buf->str = xmalloc(initial_capacity);
	buf->capacity = initial_capacity;
}

void
	stringbuf_free(t_string_buffer *s)
{
	free(s->str);
}

void
	stringbuf_append(t_string_buffer *s, t_string str)
{
	size_t	new_cap;

	new_cap = s->capacity + !s->capacity * 256;
	while (new_cap < s->len + str.len)
		new_cap *= 2;
	s->str = ft_realloc(s->str, s->capacity, new_cap);
	s->capacity = new_cap;
	ft_memcpy(s->str + s->len, str.str, str.len);
	s->len += str.len;
}
