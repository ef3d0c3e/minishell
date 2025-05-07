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

/** @brief Types of results */
enum e_result_type
{
	/** @brief Normal return code, continue execution */
	RES_NONE,
	/** @brief Returns from current function (optional exit code, defaults
	 * to 0) */
	RES_RETURN,
	/** @brief Exits from current loop (if/while/until) with an optional numeric
	 * parameter to exit nested loops (defaults to 1) */
	RES_BREAK,
	/** @brief Skip until next iteration of loop (if/while/until) with an 
	 * optional numeric parameter to continue nested loops (defaults to 1) */
	RES_CONTINUE,
	/** @brief Exits current shell, with optional numeric argument for exit
	 * code, defaults to 0 */
	RES_EXIT,
	/** @brief Stops the current command from being evaluated, set when a signal
	 * is caught */
	RES_STOP,
};

typedef struct s_eval_result
{
	/** @brief Result type */
	enum e_result_type	type;
	/** @brief Optional parameter, e.g for return, exit, continue or break */
	int					param;
}	t_eval_result;

/******************************************************************************/
/* Evaluators                                                                 */
/******************************************************************************/

/**
 * @brief Evaluates node `program`
 *
 * This function will look at the node's type to dispatch to the correct
 * evaluator function below.
 *
 * @param shell The shell session
 * @param program The ast node to evaluate
 */
t_eval_result
eval(t_shell *shell, t_ast_node* program);
/**
 * @brief Evaluates a list of commands, separated by `;` `\n` and `&`
 *
 * @param shell The shell session
 * @param The command list node
 */
t_eval_result
eval_list(t_shell *shell, t_ast_node* cmd);
/**
 * @brief Evaluates command node
 *
 * @param shell The shell session
 * @param The command node
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
 * @brief Evaluates a pipeline operator `|` or `|&`
 */
t_eval_result
eval_pipeline(t_shell *shell, t_ast_node* pipeline);
/**
 * @brief Evaluates a binary operator `||` or `&&`
 */
t_eval_result
eval_binary(t_shell *shell, t_ast_node* pipeline);
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
 * @brief Evaluates a while or until loopp
 */
t_eval_result
eval_loop(t_shell *shell, t_ast_node* node_loop);
/**
 * @brief Evaluates if word in wordlist do cmd_list done 
 */
t_eval_result
eval_for(t_shell *shell, t_ast_node* node_if);

/******************************************************************************/
/* Commands evaluators                                                        */
/******************************************************************************/

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
 * @brief Evaluates a function call
 *
 * @param shell The shell session
 * @param function The evaluated function node
 * @param argv Function arguments
 */
t_eval_result
eval_function(t_shell *shell, t_ast_node *function, char **argv);
/**
 * @brief Evaluates a builtin command
 *
 * @param shell The shell session
 * @param cmd The command node
 * @param argv Function arguments
 */
t_eval_result
eval_builtin(t_shell *shell, t_ast_node *cmd, char **argv);
/**
 * @brief Evaluates an executable
 *
 * @param shell The shell session
 * @param cmd The command node
 * @param path Path to the executable
 * @param argv Function arguments
 */
t_eval_result
eval_exec(t_shell *shell, t_ast_node *cmd, char *path, char **argv);

/******************************************************************************/
/* Arguments handling                                                         */
/******************************************************************************/

/** @brief Frees a NULL-terminated array of strings */
void
args_free(char **cmd);

#endif // EVAL_H
