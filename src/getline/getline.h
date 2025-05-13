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
# include <sys/ioctl.h>

typedef struct s_shell		t_shell;
typedef struct s_getline	t_getline;
typedef struct s_buffer		t_buffer;

/******************************************************************************/
/* Input handling                                                             */
/******************************************************************************/

/** @brief The default `getc` function */
int
getline_getc(t_getline *line);
/** @brief Reads a single char, using either `getc` or by recycling from the
 * queue */
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

/** @brief Signature for the key handler function */
enum e_keyhandler_sig
{
	SIG_NONE = 0,	
	SIGMASK_P0 = 0x3,

	SIG_I = 0x1,	
	SIG_Z = 0x2,	
};

typedef struct s_key_handler
{
	/** @brief Key sequence for this handler */
	const char				*keyseq;
	/** @brief Function pointer */
	void					*function;
	/** @brief Function signature */
	enum e_keyhandler_sig	sig;
	/* @brief First parameter */
	union
	{
		int					i0;
		size_t				z0;
	};
}	t_key_handler;

int
getline_handle_key(t_getline *line, int c);
/** @brief Moves in input buffer by `offset` */
void
getline_move(t_getline *line, int offset);
/** @brief Moves at absolute byte position */
void
getline_move_at(t_getline *line, size_t pos);
/** @brief Moves in input buffer to the next/prvious word */
void
getline_move_word(t_getline *line, int direction);
/** @brief Deletes characters before or after cursor */
void
getline_delete(t_getline *line, int offset);
/** @brief Deletes next/previous word */
void
getline_delete_word(t_getline *line, int direction);

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
getline_highlight_display(t_getline *line, const t_buffer_attr *attr);

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
/* Completion                                                                 */
/******************************************************************************/

/** @brief Default completion item kinds */
enum e_complete_item_kind
{
	COMPLETE_WORD,
	COMPLETE_OPTION,
	COMPLETE_FILE,
};

typedef struct s_complete_item
{
	int		kind;
	char	*name;
	char	*desc;
}	t_complete_item;

/** @brief Function returning the next completion item */
typedef const t_complete_item*(*t_comp_fn)(t_getline *l, size_t i);

typedef struct s_complete_state
{
	/** @brief Whether the completion menu is active */
	int		shown;
	/** @brief Selected completion item */
	size_t	sel;
	/** @brief Draws a single completion item */
	void	(*comp_draw_item_fn)(t_getline *, const t_complete_item *item);

	/*-- Draw state --*/
	/** @brief Menu start row */
	int start_row;
	/** @brief Menu end row */
	int end_row;
	/** @brief Menu column width */
	int col_width;
	/** @brief Window width */
	int width;
	/** @brief Window height */
	int height;

}	t_complete_state;

void
getline_complete_menu(t_getline *line);

/** @brief Moves in the completion menu by columns */
void
getline_complete_move(t_getline *l, int offset);
/** @brief Moves in the completion menu by rows */
void
getline_complete_move_row(t_getline *l, int offset);

/******************************************************************************/
/* Rendering                                                                  */
/******************************************************************************/

/** @brief Render state for the line */
typedef struct s_drawline
{
	int	cursor_pos;
	int	column_pos;
	int	input_w;
	int	prompt_w;
	int	left_indicator;
	int	right_indicator;
	int	printed;
}	t_drawline;

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
getline_redraw(t_getline *line, int update);

/** @brief Sets the prompt text */
void
getline_set_prompt(t_getline *line, const char *text);

typedef struct s_getline
{
	/** @brief Associated shell session */
	t_shell				*shell;

	/*-- Input/Output stack --*/
	/** @brief Input FD */
	int					in_fd;
	/** @brief Output FD */
	int					out_fd;
	/** @brief Getchar function */
	int					(*getc_fn)(t_getline *);
	/** @brief Queed input */
	t_string_buffer		input_queue;

	/*-- Rendering --*/
	/** @brief Highlighter function */
	void				(*highlighter_fn)(t_getline *);
	/** @brief Overflow indicator function */
	void				(*overflow_fn)(t_getline *, int);
	/** @brief Line prompt */
	t_buffer			prompt;
	/** @brief Input buffer */
	t_buffer			input;
	/** @brief Line render data */
	t_render_data		render;

	/*-- Cursor & Input handling --*/
	/** @brief Cursor's byte position in the prompt */
	size_t				cursor_index;
	/** @brief Key sequence sliding window */
	unsigned char		sequence[16];
	/** @brief Length of `sequence` */
	size_t				sequence_len;
	/** @brief List of key bindings */
	t_rbtree			keybinds;
	/** @brief Word boundaries function */
	t_u8_iterator		(*boundaries_fn)(t_getline *line, t_u8_iterator, int);

	/*-- Completion --*/
	/** @brief State of the completion menu */
	t_complete_state	comp_state;
	/** @brief Key binds for completion mode */
	t_rbtree			comp_keybinds;

	/** @brief Terminal handling */
	struct termios		tio;
}	t_getline;

t_getline
getline_setup(t_shell *shell);
void
getline_cleanup(t_getline *line);
/** @brief Entry point function */
char
*getline_read(t_getline *line, const char *prompt);

/******************************************************************************/
/* Default handlers                                                           */
/******************************************************************************/

/** @brief Registers default handlers */
void
getline_setup_handlers(t_getline *line);
/** @brief `overflow_fn`, draws overflow indicators */
void
getline_handler_overflow(t_getline *line, int right);
/** @brief Finds word boundaries */
t_u8_iterator
getline_handler_word_boundaries(t_getline *line, t_u8_iterator it, int direction);
/** @brief Default completion item draw function */
void
getline_handler_draw_comp_item(
	t_getline *line,
	const t_complete_item *item);

/******************************************************************************/
/* Utilities                                                                  */
/******************************************************************************/

/** @brief Gets the cursor position */
int
getline_cursor_pos(t_getline *line, int *x, int *y);
/** @brief Asks the terminal to measure the width of text */
int
getline_text_width(t_getline *line, const char *utf8, size_t byte_len);

#endif // GETLINE_H
