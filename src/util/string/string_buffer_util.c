/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   string_buffer_util.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgamba <linogamba@pundalik.org>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 11:59:40 by lgamba            #+#    #+#             */
/*   Updated: 2025/03/17 11:59:41 by lgamba           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "../util.h"

void
	stringbuf_replace(
	t_string_buffer *buf,
	size_t start,
	size_t end,
	const char *replace)
{
	const size_t	len = ft_strlen(replace);
	char			*result;

	buf->capacity = len + buf->len - (end - start);
	result = xmalloc(buf->capacity);
	ft_memcpy(result, buf->str, start);
	ft_memcpy(result + start, replace, len);
	ft_memcpy(result + start + len, buf->str + end, buf->len - end);
	free(buf->str);
	buf->len = buf->capacity;
	buf->str = result;
}

char
	*stringbuf_cstr(t_string_buffer *buf)
{
	stringbuf_reserve(buf, buf->len + 1);
	buf->str[buf->len] = 0;
	return (buf->str);
}

void
	stringbuf_reserve(t_string_buffer *buf, size_t new_capacity)
{
	if (buf->capacity >= new_capacity)
		return ;
	buf->str = ft_realloc(buf->str, buf->len, new_capacity);
	buf->capacity = new_capacity;
}
