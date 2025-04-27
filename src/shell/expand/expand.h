/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgamba <linogamba@pundalik.org>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 11:59:40 by lgamba            #+#    #+#             */
/*   Updated: 2025/03/17 11:59:41 by lgamba           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#ifndef EXPAND_H
# define EXPAND_H

#include "util/util.h"
typedef struct s_shell	t_shell;

#include <parser/parser.h>

/** @brief A fragment */
typedef struct s_fragment
{
	t_string_buffer	word;
	int				flags;
	/** @brief Force this fragment to become the beginning of it's own
	 * argument */
	int				force_split;
}	t_fragment;

/** @brief A list of fragment */
typedef struct s_fragment_list
{
	t_fragment	*fragments;
	size_t		size;
	size_t		capacity;
}	t_fragment_list;

/**
 * @brief Pushes a word to the fragment list
 *
 * @param list List to push to
 * @param word Word to push
 * @param Original word flags
 */
void
fraglist_push(t_fragment_list *list, t_string_buffer word, int flags);

/**
 * @brief Resolves command arguments to an array of strings while performing
 * argument expansion
 *
 * @param shell The shell session
 * @param cmd Command to create an array of string from
 *
 * @returns A null-terminated array of strings to be passed to `execve`
 */
char
**arg_expansion(t_shell *shell, const struct s_cmd_node *cmd);

/**
 * @brief Resolves a parameter in the current context
 *
 * Resolution is done in the following order
 *  - Check for special parameter: $?, $# $@, etc.
 *  - Check for positional parameters: $0, $1, $2, etc. Positional parameters
 *  are look for in the current evaluation stack frame
 *  - Check for local variables in the current evaluation stack frame
 *  - Check for global variables in the environment
 *
 * @param shell The shell session
 * @param param Parameter to expand
 * @param argv Argument list to expand into
 * @param size Size of `argv`
 *
 * @returns 1 On success, 0 on failure. In `expfail` mode, the error should be
 * reported and evaluation should stop.
 */
int
expand_param(
	t_shell *shell,
	t_fragment_list *list,
	struct s_arg_item *param);

/**
 * @brief Expands sub expressions
 *
 * @param shell The shell session
 * @param list Fragment list to insert into
 * @param param Sub expression to expand
 *
 * @returns 1 On success, 0 on failure.
 */
int
expand_subexpr(
	t_shell *shell,
	t_fragment_list *list,
	struct s_arg_item *param);

#endif // EXPAND_H
