/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cluster_heuristics.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgamba <linogamba@pundalik.org>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 11:59:40 by lgamba            #+#    #+#             */
/*   Updated: 2025/03/17 11:59:41 by lgamba           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include <shell/shell.h>

/** @brief Performs expensive reclustering of all codepoints in [lo, hi-1] */
static void
	recluster_precise(t_getline *line, t_buffer *buf, t_u8_iterator lo, t_u8_iterator hi)
{
	const char	*str = buf->buffer.str;
	size_t		i[3];
	int			w;
	t_cluster	*c;

	c = &buf->s_clusters.data[lo.cp_pos];
	w = getline_text_width(line, str + lo.byte_pos, c->size);
	c->width = w;
	i[0] = lo.cp_pos + 1;
	i[1] = lo.byte_pos + c->size;
	i[2] = lo.byte_pos;
	while (i[0] < hi.cp_pos)
	{
		c = &buf->s_clusters.data[i[0]++];
		if (getline_text_width(line, str + i[2], i[1] + c->size
					- i[2]) == w)
			c->width = 0;
		else
		{
			i[2] = i[1];
			w = getline_text_width(line, str + i[1], c->size);
			c->width = w;
		}
		i[1] += c->size;
	}
}

void
	getline_recluster(t_getline *line, t_buffer *buf, t_u8_iterator it)
{
	t_u8_iterator	prev;
	t_u8_iterator	next;
	size_t			i;

    if (it.codepoint.len == 0 || it.cp_pos > buf->s_clusters.size)
        return ;
    getline_insert_cluster(buf, it.cp_pos, (t_cluster){it.codepoint.len, 0});
	prev = it;
	it_prev(&prev);
	next = it;
	it_next(&next);
	if (codepoint_width(u8_to_cp(prev.codepoint)) != 1
		|| codepoint_width(u8_to_cp(next.codepoint)) != 1
		|| codepoint_width(u8_to_cp(it.codepoint)) != 1
		|| codepoint_is_fuser(u8_to_cp(it.codepoint)))
	{
		i = 0;
		while (++i < LINE_CLUSTER_MAX)
		{
			if (prev.codepoint.len && !buf->s_clusters.data[prev.cp_pos].width)
				it_prev(&prev);
			if (next.codepoint.len && !buf->s_clusters.data[next.cp_pos].width)
				it_next(&next);
		}
		recluster_precise(line, buf, prev, next);
	}
	else
		buf->s_clusters.data[it.cp_pos].width
			= codepoint_width(u8_to_cp(it.codepoint));
}
