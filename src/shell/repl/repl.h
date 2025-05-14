/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   repl.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgamba <linogamba@pundalik.org>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 11:59:40 by lgamba            #+#    #+#             */
/*   Updated: 2025/03/17 11:59:41 by lgamba           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#ifndef REPL_H
# define REPL_H

#include <getline/getline.h>

/******************************************************************************/
/* Completion                                                                 */
/******************************************************************************/

typedef struct s_path_tr
{
	size_t			index;
	t_complete_item	*items;
}	t_path_tr;

/**
 * @brief Highlighter for the getline prompt
 */
t_complete_item
*repl_completer(t_getline *line);

/**
 * @brief Highlighter for the getline prompt
 */
void
repl_highlighter(t_getline *line);

t_getline
repl_setup(t_shell *shell);

#endif // REPL_H
