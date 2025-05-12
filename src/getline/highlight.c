/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   highlight.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgamba <linogamba@pundalik.org>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 11:59:40 by lgamba            #+#    #+#             */
/*   Updated: 2025/03/17 11:59:41 by lgamba           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include <shell/shell.h>

static size_t
	bfind(t_buffer *buf, size_t start)
{
	size_t									lo;
	size_t									hi;
	size_t									mid;

	lo = 0;
	hi = buf->s_attrs.size;
	while (lo < hi)
	{
		mid = (lo + hi) / 2;
		if (start < buf->s_attrs.data[mid].start)
			hi = mid;
		else if (start >= buf->s_attrs.data[mid].end)
			lo = mid + 1;
		else
			return (lo);
	}
	return (SIZE_MAX);
}

void
	getline_highlight_add(t_buffer *buf, t_buffer_attr attr)
{
	size_t	pos;
	size_t	new_cap;

	if (buf->s_attrs.size <= buf->s_attrs.capacity)
	{
		new_cap = buf->s_attrs.capacity * 2 + !buf->s_attrs.capacity * 4;
		buf->s_attrs.data = ft_realloc(buf->s_attrs.data,
				sizeof(t_buffer_attr) * buf->s_attrs.size,
				sizeof(t_buffer_attr) * new_cap);
		buf->s_attrs.capacity = new_cap;
	}
	pos = bfind(buf, attr.start);
	if (pos == SIZE_MAX)
		pos = buf->s_attrs.size;
	ft_memmove(buf->s_attrs.data + pos + 1, buf->s_attrs.data + pos,
			buf->s_attrs.size - pos);
	buf->s_attrs.data[pos] = attr;
	++buf->s_attrs.size;
}

t_buffer_attr*
	getline_highlight_get(t_buffer *buf, size_t pos)
{
	const size_t	p = bfind(buf, pos);

	if (p == SIZE_MAX)
		return (NULL);
	return (&buf->s_attrs.data[p]);
}

void
	getline_highlight_display(t_getline *line, const t_buffer_attr *attr)
{
	if (!attr)
	{
		ft_dprintf(line->out_fd, "\033[m");
		return ;
	}
	if (attr->color != -1)
		ft_dprintf(line->out_fd, "\033[38;2;%d;%d;%dm",
				((attr->color >> 16) & 0xFF),
				((attr->color >> 8) & 0xFF),
				attr->color & 0xFF);
}
