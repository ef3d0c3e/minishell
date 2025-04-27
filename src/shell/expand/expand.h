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

#include "parser/parser.h"
typedef struct s_shell	t_shell;

/**
 * @brief Resolves command parameters to an array of strings while performing
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
 * @brief Resolvevs a parameter in the current context
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
	struct s_arg_param *param,
	char ***argv,
	size_t *size);

#endif // EXPAND_H
