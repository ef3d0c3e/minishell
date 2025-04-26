/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   funs.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgamba <linogamba@pundalik.org>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 11:59:40 by lgamba            #+#    #+#             */
/*   Updated: 2025/03/17 11:59:41 by lgamba           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#ifndef FUNS_H
# define FUNS_H

# include <parser/parser.h>

typedef struct s_shell	t_shell;

/** @brief Represents a function's stack frame */
typedef struct s_stack_frame
{
	/** @brief Function being evaluated */
	t_ast_node	*function;
	/** @brief Function's argument: `$1` `$2` etc... */
	char		**args;
	/** @brief Number of arguments */
	size_t		nargs;
	/** @brief Local variables defined in this stack */
	t_rbtree	locals;
}	t_stack_frame;

/** @brief Evaluation stack for the shell session */
typedef struct s_eval_stack
{
	/** @brief Current size of the stack */
	size_t			size;
	/** @brief Fixed stack capacity */
	size_t			capacity;
	/** @brief Stack frames */
	t_stack_frame	*frames;
}	t_eval_stack;

/**
 * @brief Initializes function related data for the shell
 *
 * @param shell The shell session
 */
void
funs_init(t_shell *shell);
/**
 * @brief Deinitializes function related data for the shell
 *
 * @param shell The shell session
 */
void
funs_deinit(t_shell *shell);
/**
 * @brief Push a function to the evaluation stack
 *
 * @param shell The shell session
 * @param function The evaluated function
 * @param argv Function arguments (NULL terminated)
 */
void
funs_stack_push(t_shell *shell, t_ast_node *function, char **argv);
/**
 * @brief Pops the last function from the avaluation stack
 */
void
funs_stack_pop(t_shell *shell);

#endif // FUNS_H
