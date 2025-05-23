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
#include <stdlib.h>
#include <string.h>

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
	memcpy(result, buf->str, start);
	memcpy(result + start, replace, len);
	memcpy(result + start + len, buf->str + end, buf->len - end);
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

void
	stringbuf_itoa(t_string_buffer *buf, int number)
{
	size_t	len;
	size_t	i;
	int		n;

	stringbuf_reserve(buf, buf->len + 11);
	len = 1;
	n = number;
	while (n <= -10 || n >= 10)
	{
		++len;
		n /= 10;
	}
	if (n < 0)
		buf->str[buf->len++] = '-';
	if (!n)
		buf->str[buf->len] = '0';
	i = 0;
	n = number;
	while (i++ < len)
	{
		buf->str[buf->len + len - i] = (n % 10) + '0';
		n /= 10;
	}
	buf->len += len + (n < 0);
}
