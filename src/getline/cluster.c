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
#include "getline/getline.h"
#include <shell/shell.h>

int
measure_terminal_width(t_getline *line, const char *utf8, size_t byte_len);

static void
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

static void
	remove_cluster(t_getline *line, size_t i, size_t j)
{
	if (i >= j)
		return ;
	/*
	ft_memmove(line->buffer.s_clusters.data + i,
			line->buffer.s_clusters.data + j,
			sizeof(t_cluster) * (line->buffer.s_clusters.size - j));
	line->buffer.s_clusters.size -= (j - i);
	*/

	size_t total = line->buffer.s_clusters.size;
	size_t tail  = total - j;  
	t_cluster *A = line->buffer.s_clusters.data;

	// move the clusters at indices [j .. total) down to index i
	ft_memmove(&A[i],
			&A[j],
			sizeof *A * tail);

	line->buffer.s_clusters.size = total - (j - i);
}

/** @brief Checks if codepoint `cp` is a fuser according to UAX-29 */
static int	probe_fuser(uint32_t cp)
{
	static const struct s_codepoint_range fuse_ranges[] = {
	{0x200D, 0x200D}, {0x1F1E6, 0x1F1FF}, {0x1F3FB, 0x1F3FF}, {0x0300, 0x036F},
	{0x1AB0, 0x1AFF}, {0x1DC0, 0x1DFF}, {0x0903, 0x0903}, {0x093E, 0x0940},
	{0x0949, 0x094C}, {0x0600, 0x0605},
	};
	size_t	lo;
	size_t	hi;
	size_t	mid;

	lo = 0,
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


void getline_recluster(t_getline *line, t_u8_iterator it)
{
	// 1) insert dummy cluster so that cluster-array index == codepoint index
    if (it.codepoint.len == 0 || it.cp_pos > line->buffer.s_clusters.size)
        return ;
    insert_cluster(line, it.cp_pos, (t_cluster){it.codepoint.len, 0});

    // 2) recompute total codepoints now
    size_t total = line->buffer.s_clusters.size;

	u8_to_cp(it.codepoint);
    // 3) build recluster window [lo..hi)
    size_t k = it.cp_pos;  // index of our new codepoint
	size_t	lo;
	size_t	hi;

	lo = 0;
	if (it.cp_pos >= LINE_CLUSTER_MAX)
		lo = it.cp_pos - LINE_CLUSTER_MAX;
	hi = line->buffer.s_clusters.size;
	if (it.cp_pos + LINE_CLUSTER_MAX + 1 < hi)
		hi = it.cp_pos + LINE_CLUSTER_MAX + 1;

    // 4) now walk codepoints in [lo..hi) and update widths in place
    //    we need to know each cluster’s starting byte to measure spans:
    size_t byte_pos = 0;
    for (size_t i = 0; i < lo; i++)
        byte_pos += line->buffer.s_clusters.data[i].size;

    // track the start of the *current* visual cluster
    size_t cluster_start_byte = byte_pos;
    int    cluster_w = -1;

    // now loop i from lo to hi-1
    for (size_t i = lo; i < hi; i++)
	{
        t_cluster *c = &line->buffer.s_clusters.data[i];
        size_t this_start = byte_pos;
        size_t this_end   = byte_pos + c->size;
        size_t span_bytes = this_end - cluster_start_byte;

        // first codepoint of window: start new cluster
        if (i == lo)
		{
            cluster_w = measure_terminal_width(
                            line,
                            line->buffer.buffer.str + this_start,
                            c->size);
            c->width = cluster_w;
        }
		else
		{
            // measure the span from cluster_start_byte…this_end
            int span_w = measure_terminal_width(
                             line,
                             line->buffer.buffer.str + cluster_start_byte,
                             span_bytes);
            if (span_w == cluster_w) {
                // fused: width zero
                c->width = 0;
            } else {
                // new cluster begins here
                cluster_start_byte = this_start;
                cluster_w = measure_terminal_width(
                                line,
                                line->buffer.buffer.str + this_start,
                                c->size);
                c->width = cluster_w;
            }
        }

        byte_pos += c->size;
    }
}

void
	getline_cluster_print(t_getline *line)
{
	size_t	pos = 0;
	for (size_t i = 0; i < line->buffer.s_clusters.size; ++i)
	{
		t_cluster *cl = &line->buffer.s_clusters.data[i];
		ft_dprintf(2, "'%.*s'(%zu/%d) ", cl->size, line->buffer.buffer.str + pos, cl->size, cl->width);
		pos += cl->size;
	}
}
