/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   modes.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgamba <linogamba@pundalik.org>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 11:59:40 by lgamba            #+#    #+#             */
/*   Updated: 2025/03/17 11:59:41 by lgamba           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#ifndef MODES_H
# define MODES_H

# include <getline/buffer.h>
# include <util/util.h>

typedef struct s_getline	t_getline;

/******************************************************************************/
/* Input                                                                      */
/******************************************************************************/

enum e_input_action
{
	/** @brief No action */
	ACT_NONE,
	/** @brief Action triggered by `Enter` */
	ACT_ENTER,
	/** @brief Action triggered by `^C` */
	ACT_CANCEL,
	/** @brief Action triggered by `^D` (only if line is empty) */
	ACT_QUIT,
};

/** @brief Draws the input mode */
void
getline_input_draw(t_getline *l, int update);
/*-- Input actions --*/
/** @brief Perform action */
void
getline_input_action(t_getline *line, int action);
/** @brief Moves in input buffer by `offset` */
void
getline_input_move(t_getline *line, int offset);
/** @brief Moves at absolute byte position */
void
getline_input_move_at(t_getline *line, size_t pos);
/** @brief Moves in input buffer to the next/prvious word */
void
getline_input_move_word(t_getline *line, int direction);
/** @brief Deletes characters before or after cursor */
void
getline_input_delete(t_getline *line, int offset);
/** @brief Deletes next/previous word */
void
getline_input_delete_word(t_getline *line, int direction);

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
	/** @brief Item kind */
	int		kind;
	/** @brief Item name */
	char	*name;
	/** @brief Item description */
	char	*desc;
}	t_complete_item;

typedef struct s_complete_state
{
	/** @brief Whether the completion menu is active */
	int				shown;
	/** @brief Selected completion item */
	int				sel;
	/** @brief Number of scrolled rows */
	int				scrolled;
	/** @brief Saved cursor's x coordinate */
	int				cur_x;
	/** @brief Saved cursor's y coordinate */
	int				cur_y;

	/*-- Draw state --*/
	/** @brief Menu start row */
	int				start_row;
	/** @brief Menu end row */
	int				end_row;
	/** @brief Menu column width */
	int				col_width;
	char			*saved_lines;

	/*-- Complete state --*/
	/** @brief Completion items available */
	t_complete_item	*items;
	/** @brief Number of completion items */
	size_t			nitems;
	/** @brief Start position of the word to replace upon completion */
	size_t			word_start;
	/** @brief End position of the word to replace upon completion */
	size_t			word_end;
}	t_complete_state;

/** @brief Populates the list of completion items */
void
getline_complete_populate_items(t_getline *line);
/** @brief Frees the list of completion items */
void
getline_complete_free_items(t_getline *line);

/** @brief Enables completion mode */
void
getline_complete_enable(t_getline *line);
/** @brief Disables completion mode */
void
getline_complete_disable(t_getline *line);
/** @brief Draws the completion menu */
void
getline_complete_draw(t_getline *line, int update);
/*-- Tab-complete actions --*/
/** @brief Moves in the completion menu by columns */
void
getline_complete_move(t_getline *line, int offset);
/** @brief Moves in the completion menu by rows */
void
getline_complete_move_row(t_getline *line, int offset);
/** @brief Moves in the completion menu by pages */
void
getline_complete_move_page(t_getline *line, int offset);
/** @brief Selects the current completion item */
void
getline_complete_select(t_getline *line);

/******************************************************************************/
/* History                                                                    */
/******************************************************************************/

typedef struct s_history_state
{
	/** @brief Scroll index */
	int				scroll_index;
	/** @brief Pseudo reverse-i-search filter */
	char			*filter;
	/** @brief Saved input buffer */
	t_buffer		saved_input;
	/** @brief Saved cursor pos */
	size_t			saved_pos;
	/** @brief Saved line scroll */
	int				saved_scroll;
	/** @brief Accept current selection */
	int				accept;
}	t_history_state;

/** @brief Enables history mode */
void
getline_history_enable(t_getline *line);
/** @brief Disables history mode */
void
getline_history_disable(t_getline *line);
/** @brief Draws the history menu */
void
getline_history_draw(t_getline *line, int update);
/*-- History actions --*/
/** @brief Moves in the history scroll menu by `offset` */
void
getline_history_move(t_getline *line, int offset);
/** @brief Cancels current history selection */
void
getline_history_cancel(t_getline *line);

/******************************************************************************/
/* Modes                                                                      */
/******************************************************************************/

enum e_line_mode
{
	/** @brief Input line mode (default) */
	LINE_INPUT = 0,
	/** @brief Tab-complete mode */
	LINE_TAB,
	/** @brief History scroll mode */
	LINE_HISTSCROLL,
	/** @brief Reverse-i-search mode */
	LINE_RI_SEARCH,
	/** @brief Placeholder for the number of modes */
	LINE_MODE_SIZE,
};

/** @brief Mode for the line */
typedef struct s_line_mode
{
	/** @brief Active keybinds for this mode */
	t_rbtree	keybinds;
	/** @brief Enable mode function */
	void		(*enable_mode_fn)(t_getline *line);
	/** @brief Disable mode function */
	void		(*disable_mode_fn)(t_getline *line);
	/** @brief Mode draw function */
	void		(*draw_mode_fn)(t_getline *line, int update);
}	t_line_mode;

/** @brief State for modes */
typedef union s_line_mode_state
{
	/** @brief Action for input mode */
	enum e_input_action	action;
	/** @brief Tab-complete mode state */
	t_complete_state	comp;
	/** @brief History scroll mode state */
	t_history_state		hist;
}	t_linemode_state;

/**
 * @brief Sets up modes for the line
 *
 * @param line Getline instance
 */
void
getline_setup_modes(t_getline *line);
/**
 * @brief Sets the line's mode
 *
 * @param line Getline instance
 * @param mode Mode to change to
 */
void
getline_change_mode(t_getline *line, int mode);

/*-- Modes setups --*/
void
getline_setup_input_mode(t_line_mode *mode);
void
getline_setup_complete_mode(t_line_mode *mode);
void
getline_setup_history_mode(t_line_mode *mode);

#endif // MODES_H
