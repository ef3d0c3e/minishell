/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   eval.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgamba <linogamba@pundalik.org>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 11:59:40 by lgamba            #+#    #+#             */
/*   Updated: 2025/03/17 11:59:41 by lgamba           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#ifndef EVAL_H
# define EVAL_H

typedef struct s_shell	t_shell;

# include <parser/parser.h>

void
eval(t_shell *shell, t_ast_node* program);

/**
 * @brief Tokenizes and evaluate to stdout/stderr
 *
 * @param shell Shell session
 * @param input Prompt input
 *
 * @returns The return value of the evaluated command (-1 = parse/token error)
 */
int
repl(t_shell *shell, char *input);

/**
 * @brief Tokenizes and evaluate to a string
 *
 * @param shell Shell session
 * @param input Prompt input
 * @param buf Buffer to store stdout
 *
 * @returns The return value of the evaluated command
 */
int
repl_to_string(t_shell *shell, char *input, t_string_buffer *buf);

/**
 * @brief Evaluates command node
 *
 * @returns 1 On success, 0 if failed to resolve executable
 */
int
eval_cmd(t_shell *shell, t_ast_node* cmd);
/**
 * @brief Evaluates a function definition
 *
 * @param shell The shell session
 * @param definition The function definition node
 */
void
eval_function_definition(t_shell *shell, t_ast_node *definition);
/**
 * @brief Evaluates a function call
 *
 * @param shell The shell session
 * @param function The evaluated function node
 * @param argv Function arguments
 */
void
eval_function(t_shell *shell, t_ast_node *function, char **argv);
/**
 * @brief Evaluates a pipeline `|` or `|&`
 */
void
eval_pipeline(t_shell *shell, t_ast_node* pipeline);
/**
 * @brief Evaluates a sequence `||` or `&&`
 */
void
eval_sequence(t_shell *shell, t_ast_node* pipeline);
/**
 * @brief Evaluates a subshell `(...)`
 */
void
eval_subshell(t_shell *shell, t_ast_node* subshell);

/******************************************************************************/
/* Argument expansion                                                         */
/******************************************************************************/


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

#endif // EVAL_H
