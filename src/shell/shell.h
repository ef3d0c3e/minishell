/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgamba <linogamba@pundalik.org>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 11:59:40 by lgamba            #+#    #+#             */
/*   Updated: 2025/03/17 11:59:41 by lgamba           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#ifndef SHELL_H
# define SHELL_H

# include <tokenizer/tokenizer.h>
# include <parser/parser.h>
# include <shell/env/env.h>
# include <builtins/builtin.h>

# include <ft_printf.h>
# include <gnl.h>

# include <sys/stat.h>
# include <sys/wait.h>
# include <errno.h>
# include <dirent.h>
# include <fcntl.h>

typedef struct s_error	t_error;

/** @brief Stores errors for the shell */
struct s_error_list
{
	t_error	*errors;
	size_t	size;
	size_t	capacity;
};

/**
 * @brief The shell session
 */
typedef struct s_shell
{
	/************\
	* Registries *
	\************/
	/** @brief The shell's environment */
	t_rbtree			reg_env;
	/** @brief Cache for regular files/symlinks in $PATH */
	t_rbtree			path_cache;
	/** @brief Builtins registry */
	t_rbtree			reg_builtins;
	/** @brief Shell options, modified by the `set` buitltin */
	t_rbtree			options;
	/** @brief File descriptor registry */
	t_rbtree			reg_fds;

	/**************\
	* Runtime data *
	\**************/
	/** @brief Current session errors */
	struct s_error_list	errors;

	/** @brief Stack of temporary values that need to be `free`d when the shell
	 * exits. The purpose of this is to avoid leaks when forking. View it as
	 * a per-process-unique pointers registry */
	t_rbtree			temporaries;
	/** @brief Current program prompt */
	char				*prompt;
	/** @brief Current token list (Non-owning) */
	t_token_list		*token_list;
	/** @brief Current parser (Non-owning) */
	t_parser			*parser;
	/** @brief Currently executed program (Non-owning) */
	t_ast_node			*program;

	/** @brief Set to 1 when the shell is not the parent shell. This value is
	 * set by @ref shell_fork */
	int					is_child;
	/** @brief Set to 1 when the shell is an interactive session. This is used
	 * to determine whether a program should expect user input */
	int					is_interactive;
	/** @brief Last program's exit status (initialized to 0) */
	int					last_status;
}	t_shell;

/******************************************************************************/
/* Shell utility functioons                                                   */
/******************************************************************************/

/**
 * @brief Creates a session from an `envp`
 *
 * @param envp The new session's environment
 *
 * @returns The newly created shell session
 */
t_shell
shell_new(const char **envp);
/**
 * @brief Destroys parsing-related data
 *
 * This function frees and sets to null the following members of `t_shell`
 *  - `prompt`
 *  - `token_list`
 *  - `parser`
 *  - `program`
 *
 * This function is called by @ref shell_free
 *
 * @param shell Session to delete parser-related data
 */
void
shell_parser_free(t_shell *shell);
/**
 * @brief Destroys a session
 *
 * @param shell Session to delete
 */
void
shell_free(t_shell *shell);

/******************************************************************************/
/* Shell libc wrappers                                                        */
/******************************************************************************/

/**
 * @brief Wrapper for `open`
 *
 * On success, this function will register the opened file descriptor
 * in the shell's fd registry.
 *
 * @warning In case the file descriptor is already registered, an error is
 * thrown
 *
 * @param shell The shell session
 * @param filename Filename to `open`
 * @param flags Open flags
 * @param mode Open mode
 *
 * @returns The opened file descriptor, -1 on error and set errno.
 */
int
shell_open(t_shell *shell, const char *filename, int flags, int mode);
/**
 * @brief Wrapper for `close`
 *
 * On success, this function will unregister the opened file descriptor
 * in the shell's fd registry.
 *
 * @warning In case the file descriptor was not previously registered, an error
 * is thrown.
 *
 * @param shell The shell session
 * @param fd File descriptor to `close`
 *
 * @returns 0 on success, -1 on error and set errno.
 */
int
shell_close(t_shell *shell, int fd);
/**
 * @brief Exits the shell with the given error status. Essentially a wrapper for
 * `exit`
 *
 * This function will clean up the memory used for the shell's session
 *
 * @param shell Shell to exit
 * @param status Shell exit status
 */
void
shell_exit(t_shell *shell, int status);
/**
 * @brief Creates a child session, a wrapper for `fork`
 *
 * The child session will have it's `is_child` attribute set to 1
 *
 * If forking fails and the session is a child session, this function does not
 * return and calls `shell_perror` to handle cleanup.
 *
 * @param shell The parent to create the child session from
 * @param function The source function
 */
pid_t
shell_fork(t_shell *shell, const char *function);

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
 * @param shell The session
 * @param msg The error's message
 * @param function The source of the error, should be populated by the
 * `__func__` macro
 */
void
shell_error(t_shell *shell, char *msg, const char *function);
/**
 * @brief Displays errors on stderr and clear the error queue
 *
 * @param shell The session
 *
 * @returns 1 If no errors were found, 0 otherwise
 */
int
shell_error_flush(t_shell *shell);
/**
 * @brief Exits the shell with the given error status and an error message
 *
 * This function will clean up the memory used for the shell's session
 *
 * @param shell Shell to exit
 * @param msg Error message
 * @param function The source function
 */
void
shell_fail(t_shell *shell, const char *msg, const char *function);
/**
 * @brief Exits the shell with the given error status and an error message
 * derived from `strerror(errno)`
 *
 * This function will clean up the memory used for the shell's session
 *
 * @param shell Shell to exit
 * @param msg Error message
 * @param function The source function
 */
void
shell_perror(t_shell *shell, const char *msg, const char *function);

/******************************************************************************/
/* Options handling for the `set` builtin                                     */
/******************************************************************************/

/** @brief Represents a configuration option for the shell. Options can be
 * configured using the `set` builtin */
struct s_option
{
	/** @brief Concise option description */
	const char	*desc;
	/** @brief Value of the option */
	int			value;
};

/**
 * @brief Initializes default options for the shell
 */
void
options_init(t_shell *shell);

/**
 * @brief Gets the value of an option
 *
 * This function will call to `shell_exit()` if the option doesn't exist
 *
 * @param shell The shell session
 * @param name Name of the option to get
 *
 * @returns The value of the option
 */
int
option_value(t_shell *shell, const char *name);

#endif // SHELL_H
