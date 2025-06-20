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

/** @brief Repl data stored by getline */
typedef struct s_repl_data
{
	/** @brief Current tokens */
	t_token_list	list;
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

/** @brief Data for path traversal */
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
 * @param items Items to populate
 * @param filter Search filter
 */
void
repl_complete_filename(t_complete_buf *items, const char *filter);

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
 * @param items Items to populate
 * @param filter Search filter
 */
void
repl_complete_cmd(t_shell *shell, t_complete_buf *items, const char *filter);
/**
 * @brief Populate completion items using the command's arguments (as defined
 * via the `complete` builtin)
 *
 * @param line Getline instance
 * @param items Items to populate
 * @param filter Search filter
 */
void
repl_complete_opts(t_getline *line, t_complete_buf *items, const char *filter);

#endif // REPL_H
