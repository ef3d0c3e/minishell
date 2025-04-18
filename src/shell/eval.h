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
# include <shell/opts.h>

# include <ft_printf.h>
# include <gnl.h>

# include <sys/stat.h>
# include <sys/wait.h>
# include <errno.h>
# include <dirent.h>
# include <fcntl.h>

typedef struct s_error	t_error;

/** @brief The shell session */
typedef struct s_environ
{
	/** @brief Stores all environment variables */
	t_rbtree	env;

	/**
	 * @brief Stores all shell options
	 */
	t_rbtree	opts;

	/** @brief All programs in the path */
	t_rbtree	path_program;

	/** @brief Errors list */
	t_error		*errors;
	/** @brief Capacity of errors list */
	size_t		errors_capacity;
	/** @brief Number of errors */
	size_t		errors_size;

	/** @brief Exit status of the last command */
	int			last_status;

	/** @brief Set to 1 when the session is a child session of another shell */
	int			is_child;

	/** @brief Currently executed program */
	t_ast_node	*program;
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

/**
 * @brief Evaluates command node
 *
 * @returns 1 On success, 0 if failed to resolve executable
 */
int
eval_cmd(t_environ *env, t_ast_node* cmd);
/**
 * @brief Evaluates a pipeline `|` or `|&`
 */
void
eval_pipeline(t_environ *env, t_ast_node* pipeline);
/**
 * @brief Evaluates a sequence `||` or `&&`
 */
void
eval_sequence(t_environ *env, t_ast_node* pipeline);

/**
 * @brief Holds data for environment tree traversal
 */
struct s_envp_traversal
{
	size_t	index;
	char	**envp;
};

/******************************************************************************/
/* Environment handling                                                       */
/******************************************************************************/

/**
 * @brief Populates program entry from the shell's `PATH`
 *
 * This can be used to repopulate the `PATH` when requested.
 *
 * @param env Shell session
 */
void
path_populate(t_environ *env);

/**
 * @brief Entry from `/etc/passwd`
 */
struct s_passwd_ent
{
	char	*username;
	char	*homedir;
};

/**
 * @brief Frees a passwd entry
 *
 * @param ent Entry to free
 */
void
passwd_free(struct s_passwd_ent *ent);

/**
 * @brief Query from `/etc/passwd`
 *
 * @param username Username to query from. On success this function takes
 * ownership of `username`.
 * @param ent The passwd entry to store the result
 *
 * @returns 0 on failure, 1 on success. On success the resulting `ent` should
 * be passed to `passwd_free`.
 */
int
passwd_query(t_environ *env, char *username, struct s_passwd_ent *ent);

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
 *
 * @returns 1 If no errors were found, 0 otherwise
 */
int
shell_error_flush(t_environ *env);
/**
 * @brief Exits the shell with the given error status
 *
 * This function will clean up the memory used for the shell's session
 *
 * @param env Shell to exit
 * @param status Shell exit status
 */
void
shell_exit(t_environ *env, int status);
/**
 * @brief Exits the shell with the given error status and an error message
 *
 * This function will clean up the memory used for the shell's session
 *
 * @param env Shell to exit
 * @param status Shell exit status
 * @param msg Error message
 * @param function The source function
 */
void
shell_perror(t_environ *env, int status, const char *msg, const char *function);

/**
 * @brief Creates a child session
 *
 * The child session will have it's `is_child` attribute set to 1
 *
 * If forking fails and the session is a child session, this function does not
 * return and calls `shell_perror` to handle cleanup.
 *
 * @param env The parent to create the child session from
 * @param function The source function
 */
pid_t
shell_fork(t_environ *env, const char *function);

/**
 * @brief Resolves function/builtin/executable from the shell
 *
 * @param env The shell session
 * @param name Executable name
 * @param result Result of this function, will need to be freed on success
 *
 * @returns
 *  - (-1) on error
 *  - (0) if `name` refers to an executable
 *  - (1) if `name` refers to a function
 *  - (2) if `name` refers to a builtin
 */
int
resolve_executable(t_environ *env, const char *name, char **result);

#endif // EVAL_H
