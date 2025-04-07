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

# include <parser/parser.h>

# include <ft_printf.h>

# include <errno.h>
# include <dirent.h>
# include <sys/stat.h>

typedef struct s_error	t_error;

/** @brief The shell session */
typedef struct s_environ
{
	/** @brief Stores all environment variables */
	t_rbtree	env;

	/** @brief All programs in the path */
	t_rbtree	path_program;

	/** @brief Errors list */
	t_error		*errors;
	/** @brief Capacity of errors list */
	size_t		errors_capacity;
	/** @brief Number of errors */
	size_t		errors_size;
}	t_environ;

/**
 * @brief Creates a session from an `envp`
 *
 * @param envp The new session's environment
 */
t_environ
env_new(char **envp);
/**
 * @brief Destroyes a session
 *
 * @param env Session to delete
 */
void
env_free(t_environ *env);

void
eval(t_environ *env, t_ast_node* program);

/******************************************************************************/
/* Path handling                                                              */
/******************************************************************************/

/**
 * @brief Populates program entry from the shell's `PATH`
 *
 * @param env Shell session
 */
void
path_populate(t_environ *env);

/******************************************************************************/
/* Error handling                                                             */
/******************************************************************************/

typedef struct s_error
{
	/** @brief Error message */
	char		*msg;
	/** @brief Error function */
	const char	*function;
}	t_error;

/**
 * @brief Adds an error to the session
 *
 * @param env The session
 * @param msg The error's message
 * @param function The source of the error, should be populated by the
 * `__func__` macro
 */
void
shell_error(t_environ *env, char *msg, const char *function);
/**
 * @brief Displays errors on stderr and clear the error queue
 *
 * @param env The session
 */
void
shell_error_flush(t_environ *env);


#endif // EVAL_H
