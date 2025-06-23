/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   repl.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgamba <linogamba@pundalik.org>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 11:59:40 by lgamba            #+#    #+#             */
/*   Updated: 2025/06/19 06:41:53 by thschnei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#ifndef REPL_H
# define REPL_H

# include <shell/regex/regex.h>
# include <getline/getline.h>
# include <tokenizer/tokenizer.h>

/******************************************************************************/
/* Completion                                                                 */
/******************************************************************************/

/** @brief Completion items */
typedef struct s_complete_buf
{
	/** @brief Completion items */
	t_complete_item	*data;
	/** @brief Number of items */
	size_t			size;
	/** @brief Items capacity */
	size_t			capacity;
}	t_complete_buf;

/**
 * @brief Adds a completion item to complete buffer
 *
 * @param buf Complete buffer
 * @param item Item to add
 */
void
complete_buf_push(t_complete_buf *buf, t_complete_item item);

/**
 * @brief The kind of completion items wanted
 */
enum e_completion_kind
{
	/** @brief Wants command completion (execs, functions, builtins) */
	COMP_CMD = (1<<0),
	/** @brief Wants files completion */
	COMP_FILES = (1<<1),
	/** @brief Wants options completion */
	COMP_OPTS = (1<<2),
	/** @brief Wants parameters completion */
	COMP_PARAM = (1<<3),
};

/** @brief Repl data stored by getline */
typedef struct s_repl_data
{
	/** @brief Current tokens */
	t_token_list			list;
	/** @brief Kind of completion wanted */
	enum e_completion_kind	kind;
	/** @brief Current command (NULL for none) */
	char					*cmd;
	/** @brief Whether the current word is the sequence's start */
	int						sequence_start;
	/** @brief Start token of the current word */
	size_t					tok_start;
	/** @brief End token of the current word */
	size_t					tok_end;
	/** @brief Completion filter (NULL for none) */
	char					*filter;
}	t_repl_data;

/** @brief Initializes empty REPL data for getline */
void
*repl_data_new(t_getline *line);
/** @brief Frees REPL data for getline */
void
repl_data_free(t_getline *line, void *data);

/**
 * @brief Highlighter for the getline prompt
 */
void
repl_highlighter(t_getline *line);

t_getline
repl_setup(t_shell *shell);

/******************************************************************************/
/* Completion                                                                 */
/******************************************************************************/

/**
 * @brief Completion provider for the getline prompt
 */
t_complete_item
*repl_completer(t_getline *line, size_t *word_start, size_t *word_end);

/** @brief Data for filename traversal */
typedef struct s_comp_file_tr
{
	/** @brief Completion items */
	t_complete_buf	*items;
	/** @brief Regex */
	t_regex			re;
	/** @brief Regex options */
	t_globopts		opts;
}	t_comp_file_tr;

/**
 * @brief Populate completion items using filenames
 *
 * @param shell The shell session
 * @param data REPL data
 * @param items Items to populate
 */
void
repl_complete_filename(
	t_shell *shell,
	const t_repl_data *data,
	t_complete_buf *items);

/** @brief Data for path traversal */
typedef struct s_comp_cmd_tr
{
	/** @brief Completion items */
	t_complete_buf	*items;
	/** @brief Search filter */
	const char		*filter;
}	t_comp_cmd_tr;

/**
 * @brief Populate completion items using commands (PATH executables, builtins
 * and functions)
 *
 * @param shell The shell session
 * @param data REPL data
 * @param items Items to populate
 */
void
repl_complete_cmd(
	t_shell *shell,
	const t_repl_data *data,
	t_complete_buf *items);
/**
 * @brief Populate completion items using the command's arguments (as defined
 * via the `complete` builtin)
 *
 * @param shell The shell session
 * @param data REPL data
 * @param items Items to populate
 */
void
repl_complete_opts(
	t_shell *shell,
	const t_repl_data *data,
	t_complete_buf *items);
/**
 * @brief Populate completion items using shell variables
 *
 * @param shell The shell session
 * @param data REPL data
 * @param items Items to populate
 */
void
repl_complete_params(
	t_shell *shell,
	const t_repl_data *data,
	t_complete_buf *items);
/**
 * @brief Checks if `name` matches against `filter`
 *
 * @param filter Filter to match against
 * @param name Name to match
 * @return 1 if name starts with `filter` (case insesitive)
 */
int
complete_match(const char *filter, const char *name);
/**
 * @brief Gets the content between two tokens
 *
 * @param list The token list
 * @param start Start token
 * @param end End token
 * @return The content between [start, end)
 */
char
*complete_token_content(const t_token_list *list, size_t start, size_t end);
/**
 * @brief Comparison function used to sort completion entries
 *
 * Entries are sorted by kind, then by name (case-insensitive)
 *
 * @param a First entry
 * @param b Second entry
 */
int
complete_sort(const void *a, const void *b);

#endif // REPL_H
