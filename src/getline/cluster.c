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
#include "getline/getline.h"
#include "util/util.h"
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

/// Ensure that clusters never span across byte_pos.
/// If a cluster covers byte_pos strictly inside, split it into two.
static void
split_cluster_at_byte(t_getline *line, size_t byte_pos)
{
    size_t i = 0;
    size_t pos = 0;
    size_t n = line->buffer.s_clusters.size;

    // 1) find the cluster index i that covers byte_pos
    //    while tracking the byte-offset pos of its start
    while (i < n)
    {
        size_t sz = line->buffer.s_clusters.data[i].size;
        if (byte_pos < pos + sz)
            break;
        pos += sz;
        i++;
    }

    // if byte_pos is exactly on a boundary (pos == byte_pos), nothing to split
    if (pos == byte_pos || i >= n)
        return;

    // now pos < byte_pos < pos + cluster.size, so split cluster[i]
    t_cluster old = line->buffer.s_clusters.data[i];
    size_t left_size  = byte_pos - pos;
    size_t right_size = old.size - left_size;

    // shrink the existing entry to the left piece
    line->buffer.s_clusters.data[i].size  = left_size;
    line->buffer.s_clusters.data[i].width = old.width;

    // insert the right piece immediately after
    insert_cluster(line,
                   i + 1,
                   (t_cluster){ .size  = right_size,
                                .width = old.width });
}

void recluster_around(t_getline *line, size_t k);

/// Insert a new visual cluster covering the byte‐range [start, end)
/// with the given width.  Splits any existing clusters at the boundaries,
/// removes any clusters fully inside the range, and inserts the new one.
size_t
getline_cluster_insert(t_getline *line,
		size_t start,
		size_t end,
		int width)
{
	// 1) Split any cluster that straddles the start boundary
	split_cluster_at_byte(line, start);
	// 2) Split any cluster that straddles the end boundary
	split_cluster_at_byte(line, end);

	// 3) Find index i of the first cluster at or after `start`
	size_t pos = 0;
	size_t i   = 0;
	while (i < line->buffer.s_clusters.size
			&& pos + line->buffer.s_clusters.data[i].size <= start)
	{
		pos += line->buffer.s_clusters.data[i].size;
		i++;
	}

	// 4) Find index j of the first cluster strictly after `end`
	size_t pos2 = pos;
	size_t j    = i;
	while (j < line->buffer.s_clusters.size
			&& pos2 < end)
	{
		pos2 += line->buffer.s_clusters.data[j].size;
		j++;
	}

	// 5) Remove all clusters in [i .. j-1]
	remove_cluster(line, i, j);

	// 6) Insert the new cluster at index i
	insert_cluster(line,
			i,
			(t_cluster){
			.size  = end - start,
			.width = width
			});
	return (i);
}

/// line->buffer.data is your UTF‑8 buffer (char*).  
/// line->buffer.s_clusters.data is t_cluster[].
void recluster_around(t_getline *line, size_t k)
{
	enum { MAX_CLUSTER_LEN = 5 };
    t_cluster *C = line->buffer.s_clusters.data;
    size_t     N = line->buffer.s_clusters.size;

    // 1) If the cluster at k has zero width, auto‑merge it into the left
    if (C[k].width == 0 && k > 0) {
        // absorb cluster k into k-1
        size_t mergedSize  = C[k-1].size + C[k].size;
        int    mergedWidth = C[k-1].width;  
        // remove k…k+1
        remove_cluster(line, k-1, k+1);
        // insert merged
        insert_cluster(line, k-1, (t_cluster){ .size = mergedSize,
                                                .width = mergedWidth });
        return;
    }

    // 2) Build prefix‑sum of widths
    int *prefix = malloc((N+1)*sizeof(int));
    prefix[0] = 0;
    size_t pi = 0;
    while (pi < N) {
        prefix[pi+1] = prefix[pi] + C[pi].width;
        pi++;
    }

    // 3) Find best span [i..j) containing k that maximizes (sumWidth - measuredWidth)
    size_t best_i = 0, best_j = 0;
    int    best_diff = 0;
    int    best_w    = 0;

    // We'll iterate i from max(0, k-(M-1)) to k
    size_t i = (k < (MAX_CLUSTER_LEN-1) ? 0 : k - (MAX_CLUSTER_LEN-1));
    while (i <= k) {
        size_t j = i + 1;
        while (j <= N && j - i <= MAX_CLUSTER_LEN) {
            if (k < j) {
                // sum of this span
                int sumW = prefix[j] - prefix[i];

                // byte-range for [i..j)
                size_t byteStart = 0, idx = 0;
                while (idx < i) {
                    byteStart += C[idx].size;
                    idx++;
                }
                size_t byteLen = 0;
                idx = i;
                while (idx < j) {
                    byteLen += C[idx].size;
                    idx++;
                }

                int w = measure_terminal_width(line,
                              line->buffer.buffer.str + byteStart,
                              byteLen);

                // if terminal treats it as one, w < sumW or (w==sumW but span>1)
                // we consider both cases:
                int diff = sumW - w;
                if (diff > best_diff || (diff == 0 && (j-i) > 1 && best_diff == 0)) {
                    // prefer any multi‑codepoint span even if no numeric shrink
                    best_diff = diff;
                    best_i    = i;
                    best_j    = j;
                    best_w    = w;
                }
            }
            j++;
        }
        i++;
    }

    free(prefix);

    // 4) If we found a multi‑codepoint span to merge (best_j–best_i > 1)
    if (best_j - best_i > 1) {
        // compute total bytes
        size_t totalB = 0, x = best_i;
        while (x < best_j) {
            totalB += C[x].size;
            x++;
        }
        // remove old clusters
        remove_cluster(line, best_i, best_j);
        // insert the merged cluster
        insert_cluster(line, best_i, (t_cluster){
            .size  = totalB,
            .width = best_w
        });
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

/*
static void
	split_cluster_at_byte(t_getline *line, size_t byte_pos)
{
	size_t pos = 0;
	size_t n = line->buffer.s_clusters.size;

	for (size_t i = 0; i < n; ++i)
	{
		t_cluster c = line->buffer.s_clusters.data[i];
		size_t start = pos;
		size_t end   = pos + c.size;

		// If byte_pos is strictly inside this cluster, split it
		if (byte_pos > start && byte_pos < end)
		{
			size_t left_size  = byte_pos - start;
			size_t right_size = end - byte_pos;
			int    orig_width = c.width;

			// shrink the existing cluster to the left piece
			line->buffer.s_clusters.data[i].size  = left_size;
			line->buffer.s_clusters.data[i].width = orig_width;

			// insert the right piece immediately after
			insert_cluster(line,
					i + 1,
					(t_cluster){ .size = right_size,
					.width = orig_width });
			return;
		}

		// if byte_pos falls exactly on this cluster's end, we're aligned
		if (byte_pos == end) {
			return;
		}

		pos = end;
	}
	// if byte_pos == 0 or beyond all clusters, nothing to split
}

/// Find the index of the cluster containing byte_pos, and also
/// return the byte‐offset at which that cluster starts.
	static size_t
find_cluster_index_and_offset(t_getline *line,
		size_t byte_pos,
		size_t *out_cluster_start)
{
	size_t pos = 0;
	for (size_t i = 0; i < line->buffer.s_clusters.size; ++i) {
		size_t sz = line->buffer.s_clusters.data[i].size;
		if (byte_pos < pos + sz) {
			// cluster i covers byte_pos
			if (out_cluster_start) *out_cluster_start = pos;
			return i;
		}
		pos += sz;
	}
	// if byte_pos == total bytes, return “one past last”
	if (out_cluster_start) *out_cluster_start = pos;
	return line->buffer.s_clusters.size;
}

void
	getline_cluster_insert(t_getline *line, size_t start, size_t end, int width)
{
	
	//size_t	i;
	//size_t	pos;

	//i = 0;
	//pos = 0;
	//while (i < line->buffer.s_clusters.size)
	//{
	//	if (pos + line->buffer.s_clusters.data[i].size > start)
	//		break ;
	//	pos += line->buffer.s_clusters.data[i].size;
	//	++i;
	//}
	//ft_dprintf(2, "Insert %zu/%zu %zu/%zu\n\r", start, end, i, pos);
	//if (pos != start)
	//{
	//	// TODO: Edge-case where we have to break a cluster in two?
	//	shell_fail(line->shell, "Invalid clustering start", SRC_LOCATION);
	//}
	//insert(line, i, (t_cluster){end - start, width});
	//// TODO
	

	size_t cluster_start;
	size_t i = find_cluster_index_and_offset(line, byte_pos, &cluster_start);

	// If byte_pos is exactly on a boundary (== cluster_start), nothing to do
	if (cluster_start == byte_pos)
		return;

	// Must be strictly inside cluster i
	t_cluster old = line->buffer.s_clusters.data[i];
	size_t left_size  = byte_pos - cluster_start;
	size_t right_size = old.size - left_size;

	// Shrink existing to left half
	line->buffer.s_clusters.data[i].size  = left_size;
	line->buffer.s_clusters.data[i].width = old.width;

	// Insert right half immediately after
	insert_cluster(line, i + 1, (t_cluster){
			.size  = right_size,
			.width = old.width
			});
}

*/
