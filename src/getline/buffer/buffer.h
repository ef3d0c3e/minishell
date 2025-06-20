/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   buffer.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgamba <linogamba@pundalik.org>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 11:59:40 by lgamba            #+#    #+#             */
/*   Updated: 2025/03/17 11:59:41 by lgamba           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#ifndef BUFFER_H
# define BUFFER_H

# include <util/util.h>

typedef struct s_getline	t_getline;
typedef struct s_buffer		t_buffer;

/******************************************************************************/
/* Unicode grapheme handling                                                  */
/******************************************************************************/

enum
{
	/** @brief Maximum number of codepoints to recluster around */
	LINE_CLUSTER_MAX = 10,
};
/**
 * @brief Represents a single grapheme cluster according to the terminal.
 *
 * It is impossible to perform clustering programmatically since different
 * terminals + font stack will handle unicode differently. So we nicely ask the
 * terminal about the width of our codepoint sequences and trust it.
 *
 * Hence this is called `cluster` because `grapheme` would be incorrect.
 * Even though this is what visually appears on the terminal screen.
 */
typedef struct s_cluster
{
	/** @brief Size in bytes of the cluster */
	size_t	size;
	/** @brief Visual width (measured) of the cluster */
	int		width;
}	t_cluster;

/**
 * @brief Inserts cluster at guven position
 *
 * @param buf Buffer to insert to
 * @param at Position to insert at
 * @param cluster Cluster to insert
 */
void
getline_insert_cluster(t_buffer *buf, size_t at, t_cluster cluster);
/**
 * @brief Removes clusters in a range
 *
 * Removes clusters within [i, j-1].
 *
 * @param buf Buffer to remove from
 * @param i Range start
 * @param j Range end
 */
void
getline_remove_cluster(t_buffer *buf, size_t i, size_t j);
/**
 * @brief Prints clustering data to stderr
 */
void
getline_cluster_print(t_getline *line);
/**
 * @brief Updates the cluster data around `it`
 *
 * @param line Getline instance
 * @param buf Buffer to recluster
 * @param it Iterator to recluster around
 */
void
getline_recluster(t_getline *line, t_buffer *buf, t_u8_iterator it);

/******************************************************************************/
/* Buffer, holding attributes and clustering data                             */
/******************************************************************************/

/** @brief Style attributes */
typedef struct s_buffer_attr
{
	/* @brief Style start range */
	size_t	start;
	/* @brief Style end range */
	size_t	end;
	/** @brief Color, set to -1 for none */
	int		color;
	/** @brief Bold text */
	int		bold:1;
	/** @brief Italic text */
	int		italic:1;
	/** @brief Underline text */
	int		underline:1;
}	t_buffer_attr;

/** @brief Adds highlighting to the buffer */
void
getline_highlight_add(t_buffer *buf, t_buffer_attr attr);
/** @brief Gets highlight at byte position */
t_buffer_attr
*getline_highlight_get(t_buffer *buf, size_t pos);
/** @brief Displays the given highlights, resets if `attr == NULL` */
void
getline_highlight_display(
	t_getline *line,
	t_string_buffer *buf,
	const t_buffer_attr *attr);

typedef struct s_buffer
{
	/** @brief The line's buffer */
	t_string_buffer		buffer;
	/** @brief Bytes required to terminate the current codepoint */
	size_t				cp_len;
	/** @brief Start byte position of the current codepoint */
	size_t				cp_pos;
	/** @brief Attributes buffer */
	struct
	{
		t_buffer_attr	*data;
		size_t			capacity;
		size_t			size;
	}	s_attrs;
	/** @brief Clustering data */
	struct
	{
		t_cluster		*data;
		size_t			size;
		size_t			capacity;
	}	s_clusters;
}	t_buffer;

/**
 * @brief Initializes a new empty buffer
 */
t_buffer
getline_buffer_new(void);
/**
 * @brief Creates a buffer from another buffer
 */
t_buffer
getline_buffer_clone(const t_buffer *orig);
/**
 * @brief Frees a buffer
 *
 * @param buf Buffer to free
 */
void
getline_buffer_free(t_buffer *buf);
/** @brief Sets the buffer while performing easy-reclustering
 * (meant for preview) */
void
getline_buffer_set_content(t_buffer *buf, const char *str);
/** @brief Reclusters the entire buffer (expensive) */
void
getline_buffer_recluster(t_getline *line, t_buffer *buf);
/** @brief Inserts byte `c` into the line buffer */
void
getline_buffer_insert(t_getline *line, int c);
/**
 * @brief Replaces byte range in buffer with content
 *
 * This function will dellete highlight and clustering data for the buffer.
 * Clustering data will be repopulated using `wcwidth`, so it will require
 * further reclustering before editing the buffer.
 *
 * @param buf Buffer to replace into
 * @param start Start byte pos
 * @param end End byte pos
 * @param content Content to replace [start..end) with
 */
void
getline_buffer_replace(
	t_buffer *buf,
	size_t start,
	size_t end,
	const char *content);

#endif // BUFFER_H
