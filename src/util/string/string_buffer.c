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
#include "../util.h"

void
	stringbuf_free(t_string_buffer *s)
{
	free(s->str);
}

t_string_buffer
	stringbuf_copy(const t_string_buffer *orig)
{
	t_string_buffer	copy;

	copy = *orig;
	copy.str = ft_memcpy(xmalloc(copy.len), orig->str, copy.len);
	return (copy);
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

void
	stringbuf_append_n(t_string_buffer *s, t_string str, size_t n)
{
	size_t	i;

	i = 0;
	while (i++ < n)
		stringbuf_append(s, str);
}

t_string_buffer
	stringbuf_substr(t_string str, size_t start, size_t end)
{
	t_string_buffer	sub;

	sub.len = end - start;
	sub.capacity = sub.len;
	sub.str = xmalloc(sub.len);
	ft_memcpy(sub.str, str.str + start, sub.len);
	return (sub);
}
