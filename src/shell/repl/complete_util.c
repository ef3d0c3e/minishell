/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   complete_util.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgamba <linogamba@pundalik.org>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 11:59:40 by lgamba            #+#    #+#             */
/*   Updated: 2025/06/19 06:48:54 by thschnei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include <shell/shell.h>

void
	complete_buf_push(t_complete_buf *buf, t_complete_item item)
{
	size_t	new_cap;

	if (buf->capacity <= buf->size)
	{
		new_cap = (buf->capacity * 2) + !buf->capacity * 16;
		buf->data = ft_realloc(buf->data,
				sizeof(t_complete_item) * buf->capacity,
				sizeof(t_complete_item) * new_cap);
		buf->capacity = new_cap;
	}
	buf->data[buf->size++] = item;
}

int
	complete_match(const char *filter, const char *name)
{
	size_t	i;
	char	x;
	char	y;

	i = 0;
	while (filter[i] && name[i])
	{
		x = filter[i];
		y = name[i];
		if (x >= 'a' && x <= 'z')
			x -= 32;
		if (y >= 'a' && y <= 'z')
			y -= 32;
		if (x != y)
			return (0);
		++i;
	}
	return (!filter[i] || name[i]);
}

char
	*complete_token_content(const t_token_list *list, size_t start, size_t end)
{
	t_string_buffer		buf;

	stringbuf_init(&buf, 24);
	while (start < end)
		token_wordcontent(&buf, &list->tokens[start++]);
	if (!buf.len)
		return (stringbuf_free(&buf), NULL);
	return (stringbuf_cstr(&buf));
}
