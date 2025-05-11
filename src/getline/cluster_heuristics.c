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

int
measure_terminal_width(t_getline *line, const char *utf8, size_t byte_len);

/** @brief Checks if codepoint `cp` is a fuser according to UAX-29 */
static int
	probe_fuser(uint32_t cp)
{
	static const struct s_codepoint_range	fuse_ranges[] = {
	{0x0300, 0x036F},
	{0x0600, 0x0605},
	{0x0903, 0x0903},
	{0x093E, 0x0940},
	{0x0949, 0x094C},
	{0x1AB0, 0x1AFF},
	{0x1DC0, 0x1DFF},
	{0x200D, 0x200D},
	{0x1F1E6, 0x1F1FF},
	{0x1F3FB, 0x1F3FF},
	};
	size_t									lo;
	size_t									hi;
	size_t									mid;

	lo = 0;
	hi = sizeof(fuse_ranges) / sizeof(fuse_ranges[0]);
	while (lo < hi)
	{
		mid = (lo + hi) / 2;
		if (cp < fuse_ranges[mid].start)
			hi = mid;
		else if (cp > fuse_ranges[mid].end)
			lo = mid + 1;
		else
			return (1);
	}
	return (0);
}

/** @brief Performs expensive reclustering of all codepoints in [lo, hi-1] */
static void
	recluster_precise(t_getline *line, t_u8_iterator lo, t_u8_iterator hi)
{
	const char	*str = line->buffer.buffer.str;
	size_t		i[3];
	int			w;
	t_cluster	*c;

	c = &line->buffer.s_clusters.data[lo.cp_pos];
	w = measure_terminal_width(line, str + lo.byte_pos, c->size);
	c->width = w;
	i[0] = lo.cp_pos + 1;
	i[1] = lo.byte_pos + c->size;
	i[2] = lo.byte_pos;
	while (i[0] < hi.cp_pos)
	{
		c = &line->buffer.s_clusters.data[i[0]++];
		if (measure_terminal_width(line, str + i[2], i[1] + c->size
					- i[2]) == w)
			c->width = 0;
		else
		{
			i[2] = i[1];
			w = measure_terminal_width(line, str + i[1], c->size);
			c->width = w;
		}
		i[1] += c->size;
	}
}

void
	getline_recluster(t_getline *line, t_u8_iterator it)
{
	t_u8_iterator	prev;
	t_u8_iterator	next;
	size_t			i;

    if (it.codepoint.len == 0 || it.cp_pos > line->buffer.s_clusters.size)
        return ;
    insert_cluster(line, it.cp_pos, (t_cluster){it.codepoint.len, 0});
	prev = it;
	it_prev(&prev);
	next = it;
	it_next(&next);
	if (codepoint_width(u8_to_cp(prev.codepoint)) != 1
		|| codepoint_width(u8_to_cp(next.codepoint)) != 1
		|| codepoint_width(u8_to_cp(it.codepoint)) != 1
		|| probe_fuser(u8_to_cp(it.codepoint)))
	{
		i = 0;
		while (++i < LINE_CLUSTER_MAX)
		{
			it_prev(&prev);
			it_next(&next);
		}
		recluster_precise(line, prev, next);
	}
	else
		line->buffer.s_clusters.data[it.cp_pos].width
			= measure_terminal_width(line, it.codepoint.str, it.codepoint.len);
}
