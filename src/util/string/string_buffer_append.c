/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   string_buffer_append.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgamba <linogamba@pundalik.org>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 11:59:40 by lgamba            #+#    #+#             */
/*   Updated: 2025/03/17 11:59:41 by lgamba           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include <util/util.h>

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
	stringbuf_append_s(t_string_buffer *s, const char *str)
{
	stringbuf_append(s, (t_string){str, ft_strlen(str)});
}

void
	stringbuf_append_i(t_string_buffer *s, int number)
{
	size_t	len;
	size_t	i;
	int		n;

	stringbuf_reserve(s, s->len + 11);
	len = 1;
	n = number;
	while (n <= -10 || n >= 10)
	{
		++len;
		n /= 10;
	}
	if (n < 0)
		s->str[s->len++] = '-';
	if (!n)
		s->str[s->len] = '0';
	i = 0;
	n = number;
	while (i++ < len)
	{
		s->str[s->len + len - i] = (n >= 0) * ((n % 10) + '0') + (n < 0)
			* (-(n % 10) + '0');
		n /= 10;
	}
	s->len += len + (n < 0);
}

void
	stringbuf_append_n(t_string_buffer *s, t_string str, size_t n)
{
	size_t	i;

	i = 0;
	while (i++ < n)
		stringbuf_append(s, str);
}
