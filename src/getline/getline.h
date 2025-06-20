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

# include <getline/modes/modes.h>
# include <getline/wcwidth/wcwidth.h>
# include <getline/handlers/handlers.h>

# include <sys/types.h>
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
getline_handlers_getc(t_getline *line);
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
	/** @brief `int` */
	SIG_I = 0x1,	
	/** @brief `size_t` */
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

/******************************************************************************/
/* History                                                                    */
/******************************************************************************/

typedef struct s_history_ent
{
	/** @brief Input */
	char	*input;
	/** @brief Set to 1 if this history line has been saved */
	int		saved;
}	t_history_ent;

/** @brief A structure to store command history */
typedef struct s_history
{
	/** @brief History entries */
	t_history_ent	*entries;
	/** @brief Number of history entries, when full, half the entries get
	 * written */
	size_t			num_entries;
	/** @brief Maximum history entries */
	size_t			max_entries;
	/** @brief File to store history at, will discord if not possible */
	char			*histfile;
}	t_history;

/**
 * @brief Initializes an empty history
 *
 * @param max_entries Maximum number of entries the history can hold
 *
 * @returns The newly created history
 */
t_history
getline_history_init(size_t max_entries);
/**
 * @brief Frees and synchronize the history
 *
 * @param line Getline instance
 */
void
getline_history_free(t_getline *line);
/**
 * @brief Sets the history save file for the history structure
 *
 * @param line Getline instance
 * @param histfile History file path
 * @param source Source lines from history if required
 */
void
getline_history_set_file(t_getline *line, char *histfile, int source);
/**
 * @brief Adds entry to the history
 *
 * @param line Getline instance
 * @param entry Entry to add
 */
void
getline_history_add(t_getline *line, char *entry, int saved);

/******************************************************************************/
/* Rendering                                                                  */
/******************************************************************************/

/** @brief Render state for the line */
typedef struct s_drawline
{
	t_string_buffer	buf;
	int				cursor_pos;
	int				column_pos;
	int				input_w;
	int				prompt_w;
	int				left_indicator;
	int				right_indicator;
	int				printed;
}	t_drawline;

/** @brief Redraws ui */
void
getline_redraw(t_getline *line, int update);

/******************************************************************************/
/* User-defined data														  */
/******************************************************************************/

/** @brief Data create function */
typedef void				*(*t_data_new_fn)(t_getline *line);
/** @brief Data delete function */
typedef void				(*t_data_free_fn)(t_getline *line, void *data);

/******************************************************************************/
/* Line state                                                                 */
/******************************************************************************/

/** @brief Sets the prompt text */
void
getline_set_prompt(t_getline *line, const char *text);

typedef struct s_getline
{
	/** @brief Associated shell session */
	t_shell				*shell;
	/** @brief Terminal handling */
	struct termios		tio;

	/*-- Line mode state handling --*/
	/** @brief Available line modes */
	t_line_mode			modes[LINE_MODE_SIZE];
	/** @brief Current line mode */
	enum e_line_mode	mode;
	/** @brief Line mode state */
	t_linemode_state	state;

	/*-- History --*/
	/** @brief The line's history */
	t_history			history;

	/*-- Input/Output stack --*/
	/** @brief Input FD */
	int					in_fd;
	/** @brief Output FD */
	int					out_fd;
	/** @brief Getchar function */
	t_getc_fn			getc_fn;
	/** @brief Queed inputs */
	t_string_buffer		input_queue;

	/*-- Rendering --*/
	/** @brief Highlighter function */
	t_highlighter_fn	highlighter_fn;
	/** @brief Overflow indicator function */
	t_overflow_fn		overflow_fn;
	/** @brief Line prompt */
	t_buffer			prompt;
	/** @brief Input buffer */
	t_buffer			input;
	/** @brief Line scrolled amount */
	int					scrolled;
	/** @brief Display width in cells */
	int					display_width;
	/** @brief Display height in cells */
	int					display_height;

	/*-- Cursor & Input handling --*/
	/** @brief Cursor's byte position in the prompt */
	size_t				cursor_index;
	/** @brief Key sequence sliding window */
	unsigned char		sequence[16];
	/** @brief Length of `sequence` */
	size_t				sequence_len;
	/** @brief Word boundaries function */
	t_boundaries_fn		boundaries_fn;

	/*-- Completion --*/
	/** @brief Draws a single completion item */
	t_comp_draw_item_fn	comp_draw_item_fn;
	/** @brief Returns a list of completion items */
	t_comp_provider_fn	comp_provider_fn;

	/*-- User-defined --*/
	t_data_free_fn		data_free_fn;
	void				*data;
}	t_getline;

/** @brief Sets up getline */
t_getline
getline_setup(t_shell *shell, t_data_new_fn new_fn, t_data_free_fn free_fn);
/** @brief Cleans up getline */
void
getline_cleanup(t_getline *line);
/** @brief Entry point function */
char
*getline_read(t_getline *line, const char *prompt);
/** @brief Process special key actions */
int
getline_process_action(t_getline *line);

/******************************************************************************/
/* Utilities                                                                  */
/******************************************************************************/

/** @brief Gets the cursor position */
int
getline_cursor_pos(t_getline *line, int *x, int *y);
/** @brief Sets the cursor position */
void
getline_cursor_set(t_getline *line, int col, int row);
/** @brief Sets the cursor's visibility */
void
getline_cursor_visible(t_getline *line, int visible);
/** @brief Asks the terminal to measure the width of text */
int
getline_text_width(t_getline *line, const char *utf8, size_t byte_len);
/** @brief Enables or disable raw mode */
void
getline_raw_mode(t_getline *line, int set);
/** @brief Gets the length of an escape sequence */
size_t
getline_escape_len(const char *s, size_t len);

#endif // GETLINE_H
