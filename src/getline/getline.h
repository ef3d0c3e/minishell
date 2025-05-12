/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   getline.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgamba <linogamba@pundalik.org>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 11:59:40 by lgamba            #+#    #+#             */
/*   Updated: 2025/03/17 11:59:41 by lgamba           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#ifndef GETLINE_H
# define GETLINE_H

# include <util/util.h>
# include <termios.h>

typedef struct s_shell		t_shell;
typedef struct s_getline	t_getline;
typedef struct s_buffer		t_buffer;

/******************************************************************************/
/* Input handling                                                             */
/******************************************************************************/

/** @brief The default `getc` function */
int
getline_getc(t_getline *line);
int
getline_read_char(t_getline *line);
/** @brief Appends input to the input queue */
void
getline_recycle_input(t_getline *line, const char *input, size_t len);

/******************************************************************************/
/* Key handling                                                               */
/******************************************************************************/

/** @brief Keybinding function */
typedef void				(*t_keybind_fn)(t_getline *);

void
getline_setup_keys(t_getline *line);
int
getline_handle_key(t_getline *line, int c);

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
/* Buffer management                                                          */
/******************************************************************************/

typedef struct s_buffer_attr
{
	size_t	start;
	size_t	end;
	int		color;
	int		bold:1;
	int		italic:1;
	int		underline:1;
}	t_buffer_attr;

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
 * @brief Frees a buffer
 *
 * @param buf Buffer to free
 */
void
getline_buffer_free(t_buffer *buf);

/** @brief Inserts byte `c` into the line buffer */
void
getline_buffer_insert(t_getline *line, int c);

/******************************************************************************/
/* Rendering                                                                  */
/******************************************************************************/

typedef struct s_render_data
{
	/** @brief Width in cells of the display */
	int		display_width;
	/** @brief Scrolled bytes */
	int		scrolled;
	/** @brief Safe magin in 1/100th the width */
	int		safe_margin;

}	t_render_data;

/** @brief Initializes render data */
t_render_data
getline_render_new(void);
void
getline_redraw(t_getline *line);

/** @brief Sets the prompt text */
void
getline_set_prompt(t_getline *line, const char *text);

typedef struct s_getline
{
	/** @brief Associated shell session */
	t_shell			*shell;

	/*-- Input/Output stack --*/
	/** @brief Input FD */
	int				in_fd;
	/** @brief Output FD */
	int				out_fd;
	/** @brief Getchar function */
	int				(*getc)(t_getline *line);
	/** @brief Queed input */
	t_string_buffer	input_queue;

	/** @brief List of key bindings */
	t_rbtree		keybinds;

	/*-- Rendering --*/
	/** @brief Line prompt */
	t_buffer		prompt;
	/** @brief Line render data */
	t_render_data	render;

	/** @brief Input buffer */
	t_buffer		buffer;
	/** @brief Cursor's byte position in the prompt */
	size_t			cursor_index;
	/** @brief Key sequence sliding window */
	unsigned char	sequence[16];
	size_t			sequence_len;

	/** @brief Terminal handling */
	struct termios	tio;
}	t_getline;

t_getline
getline_setup(t_shell *shell);
void
getline_cleanup(t_getline *line);

char
*getline_read(t_getline *line, const char *prompt);

/******************************************************************************/
/* Utilities                                                                  */
/******************************************************************************/

int
getline_cursor_pos(t_getline *line, int *x, int *y);
int
getline_text_width(t_getline *line, const char *utf8, size_t byte_len);

#endif // GETLINE_H
