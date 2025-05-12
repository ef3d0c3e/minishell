/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cluster.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgamba <linogamba@pundalik.org>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 11:59:40 by lgamba            #+#    #+#             */
/*   Updated: 2025/03/17 11:59:41 by lgamba           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "ft_printf.h"
#include <shell/shell.h>

void
	getline_insert_cluster(t_buffer *buf, size_t at, t_cluster cluster)
{
	size_t		new_cap;

	if (buf->s_clusters.size + 1 >= buf->s_clusters.capacity)
	{
		new_cap = !buf->s_clusters.capacity
			+ 2 * buf->s_clusters.capacity;
		buf->s_clusters.data = ft_realloc(buf->s_clusters.data,
				sizeof(t_cluster) * buf->s_clusters.size,
				sizeof(t_cluster) * new_cap);
		buf->s_clusters.capacity = new_cap;
	}
	ft_memmove(buf->s_clusters.data + at + 1,
			buf->s_clusters.data + at,
			sizeof(t_cluster) * (buf->s_clusters.size - at));
	buf->s_clusters.data[at] = cluster;
	++buf->s_clusters.size;
}

void
	getline_remove_cluster(t_buffer *buf, size_t i, size_t j)
{
	const size_t	total = buf->s_clusters.size;
	t_cluster		*a;

	if (i >= j)
		return ;
	a = buf->s_clusters.data;
	ft_memmove(&a[i], &a[j], sizeof(a[0]) * (total - j));
	buf->s_clusters.size = total - (j - i);
}

void
	getline_cluster_print(t_getline *line)
{
	size_t		pos;
	size_t		i;
	t_string	cp;
	t_cluster	*cl;

	pos = 0;
	i = 0;
	while (i < line->buffer.s_clusters.size)
	{
		cl = &line->buffer.s_clusters.data[i];
		cp.str = line->buffer.buffer.str + pos;
		cp.len = cl->size;
		if (codepoint_width(u8_to_cp(cp)))
			ft_dprintf(2, "'%.*s'(%zu/%d) ", (int)cp.len, cp.str, cl->size,
					cl->width);
		else
			ft_dprintf(2, "<%x>(%zu/%d) ", u8_to_cp(cp), cl->size, cl->width);
		pos += cl->size;
		++i;
	}
}
