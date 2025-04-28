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

/******************************************************************************/
/* Results for control flow                                                   */
/******************************************************************************/

enum e_result_type
{
	RES_NONE,
	RES_RETURN,
	RES_BREAK,
	RES_CONTINUE,
};

typedef struct s_eval_result
{
	/** @brief Result type */
	enum e_result_type	type;
	/** @brief Optional parameter, e.g for return or break */
	int					param;
}	t_eval_result;

/******************************************************************************/
/* Evaluators                                                                 */
/******************************************************************************/

t_eval_result
eval(t_shell *shell, t_ast_node* program);
/**
 * @brief Evaluates command node
 *
 * @param shell The shell session
 * @param The command node
 *
 * @returns 1 On success, 0 if failed to resolve executable
 */
t_eval_result
eval_cmd(t_shell *shell, t_ast_node* cmd);
/**
 * @brief Evaluates a function definition
 *
 * @param shell The shell session
 * @param definition The function definition node
 */
t_eval_result
eval_function_definition(t_shell *shell, t_ast_node *definition);
/**
 * @brief Evaluates a function call
 *
 * @param shell The shell session
 * @param function The evaluated function node
 * @param argv Function arguments
 */
t_eval_result
eval_function(t_shell *shell, t_ast_node *function, char **argv);
/**
 * @brief Evaluates special builtins
 *
 * @param shell The shell session
 * @param cmd The command node
 * @param argv Function arguments
 */
t_eval_result
eval_special(t_shell *shell, t_ast_node *cmd, char **argv);
/**
 * @brief Evaluates a pipeline `|` or `|&`
 */
t_eval_result
eval_pipeline(t_shell *shell, t_ast_node* pipeline);
/**
 * @brief Evaluates a sequence `||` or `&&`
 */
t_eval_result
eval_sequence(t_shell *shell, t_ast_node* pipeline);
/**
 * @brief Evaluates a subshell `(...)`
 */
t_eval_result
eval_subshell(t_shell *shell, t_ast_node* subshell);
/**
 * @brief Evaluates subexpressions `$(...)`
 */
t_eval_result
eval_subexpr(t_shell *shell, t_ast_node* subexpr);
/**
 * @brief Evaluates if cmd_list then cmd_list [elif cmd_list then cmd_list]* fi
 */
t_eval_result
eval_if(t_shell *shell, t_ast_node* node_if);
/**
 * @brief Evaluates if word in wordlist do cmd_list done 
 */
t_eval_result
eval_for(t_shell *shell, t_ast_node* node_if);

/******************************************************************************/
/* Arguments handling                                                         */
/******************************************************************************/

/** @brief Frees a NULL-terminated array of strings */
void
args_free(char **cmd);

#endif // EVAL_H
