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
#include <shell/shell.h>

void
	insert_cluster(t_getline *line, size_t at, t_cluster cluster)
{
	size_t		new_cap;

	if (line->buffer.s_clusters.size >= line->buffer.s_clusters.capacity)
	{
		new_cap = !line->buffer.s_clusters.capacity
			+ 2 * line->buffer.s_clusters.capacity;
		line->buffer.s_clusters.data = ft_realloc(line->buffer.s_clusters.data,
				sizeof(t_cluster) * line->buffer.s_clusters.size,
				sizeof(t_cluster) * new_cap);
		line->buffer.s_clusters.capacity = new_cap;
	}
	ft_memmove(line->buffer.s_clusters.data + at + 1,
			line->buffer.s_clusters.data + at,
			sizeof(t_cluster) * (line->buffer.s_clusters.size - at));
	line->buffer.s_clusters.data[at] = cluster;
	++line->buffer.s_clusters.size;
}

void
	remove_cluster(t_getline *line, size_t i, size_t j)
{
	const size_t	total = line->buffer.s_clusters.size;
	t_cluster		*a;

	if (i >= j)
		return ;
	a = line->buffer.s_clusters.data;
	ft_memmove(&a[i], &a[j], sizeof(a[0]) * (total - j));
	line->buffer.s_clusters.size = total - (j - i);
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
