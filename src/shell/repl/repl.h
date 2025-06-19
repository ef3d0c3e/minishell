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

# include "getline/buffer/buffer.h"
# include "tokenizer/tokenizer.h"
# include <getline/getline.h>

/******************************************************************************/
/* Completion                                                                 */
/******************************************************************************/

/** @brief Data for path traversal */
typedef struct s_path_tr
{
	size_t			index;
	t_complete_item	*items;
	char			*filter;
}	t_path_tr;

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
 * @brief Completion provider for the getline prompt
 */
t_complete_item
*repl_completer(t_getline *line, size_t *word_start, size_t *word_end);
/**
 * @brief Highlighter for the getline prompt
 */
void
repl_highlighter(t_getline *line);

t_getline
repl_setup(t_shell *shell);

#endif // REPL_H
