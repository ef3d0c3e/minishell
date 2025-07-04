/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgamba <linogamba@pundalik.org>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 11:59:40 by lgamba            #+#    #+#             */
/*   Updated: 2025/06/19 17:39:22 by thschnei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#ifndef SHELL_H
# define SHELL_H

# include <tokenizer/tokenizer.h>
# include <parser/parser.h>
# include <builtins/builtin.h>
# include <builtins/complete/complete.h>
# include <term/capability.h>
# include <shell/env/env.h>
# include <shell/eval/eval.h>
# include <shell/redir/redir.h>
# include <shell/fds/fds.h>
# include <shell/funs/funs.h>
# include <shell/regex/regex.h>
# include <shell/expand/expand.h>
# include <shell/ctx/ctx.h>
# include <shell/repl/repl.h>

# include <getline/getline.h>
# include <ft_printf.h>
# include <gnl.h>
# include <libopts.h>

# include <sys/stat.h>
# include <sys/wait.h>
# include <errno.h>
# include <dirent.h>
# include <fcntl.h>

/** @brief Value of the last received signal */
extern volatile sig_atomic_t	g_signal;

typedef struct s_error			t_error;

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
	/** @brief Registry for defined functions */
	t_rbtree			reg_fns;
	/** @brief Shell options, modified by the `set` buitltin */
	t_rbtree			options;
	/** @brief File descriptor registry */
	t_rbtree			reg_fds;

	/**************\
	* Runtime data *
	\**************/
	/** @brief Current session errors */
	struct s_error_list	errors;

	/** @brief Tree of temporary values that need to be `free`d when the shell
	 * exits. The purpose of this is to avoid leaks when forking. View it as
	 * a per-process-unique pointers registry.
	 *
	 * It's similar to glibc's atexit, excepts that values can be specified and
	 * removed any any given point.
	 *
	 * If the data member for a node is set, it will be used as the cleanup
	 * function instead of `free`. */
	t_rbtree			atexit;

	/** @brief Chain of active contexts */
	t_ctx				*context;

	/** @brief Prefix assignment stack */
	t_prefix_stack		prefix_stack;

	/** @brief Evaluation stack for proceduces */
	t_eval_stack		eval_stack;

	/** @brief Set to 1 when the shell is not the parent shell. This value is
	 * set by @ref shell_fork */
	int					is_child;
	/** @brief Set to 1 when the shell is an interactive session. This is used
	 * to determine whether a program should expect user input */
	int					is_interactive;
	/** @brief Last program's exit status (initialized to 0) */
	int					last_status;
	/** @brief Current terminal capabilities */
	t_termcap			cap;

	/** @brief Command options completion tree */
	t_rbtree			cmd_completion;
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
 * @brief Destroys a session
 *
 * @param shell Session to delete
 */
void
shell_free(t_shell *shell);

/******************************************************************************/
/* Temporaries handling                                                       */
/******************************************************************************/

/**
 * @brief Initializes the temporaries registry
 *
 * @param shell The shell session
 */
void
temporaries_init(t_shell *shell);
/**
 * @brief Celanup all temporaries
 *
 * @param shell The shell session
 */
void
temporaries_cleanup(t_shell *shell);

/******************************************************************************/
/* Shell libc wrappers                                                        */
/******************************************************************************/

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
/* Options handling for the `set` and `shopt` builtin                         */
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
 * @brief Insert option into the registry
 *
 * @param shell The shell session
 * @param name The option's name
 * @param desc The option's description
 * @param value The option's initial value [0 or 1].
 */
void
option_insert(t_shell *shell, const char *name, const char *desc, int value);
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
/**
 * @brief Sets the value of an option
 *
 * @param shell The shell session
 * @param name Name of the option to set
 * @param value Option value: 0 or 1
 *
 * @returns 1 On success, 0 on failure
 */
int
option_set(t_shell *shell, const char *name, int value);

/******************************************************************************/
/* Signal handling                                                            */
/******************************************************************************/

/**
 * @brief Install or uninstall signals handlers for the shell
 *
 * @param shell The shell session
 * @param uninstall Set to 1 to uninstall the signal handlers
 */
void
signal_install(t_shell *shell, int uninstall);

/******************************************************************************/
/* Command line options                                                       */
/******************************************************************************/

/** @brief Command line options for the shell */
typedef struct s_program_opts
{
	/** @brief Libopts handle */
	void		*opts;
	/** @brief Command to run in headless mode */
	const char	*headless;
	/** @brief Run in clean mode (do not source profile) */
	int			clean_mode;
}	t_program_opts;

/**
 * @brief Initializes the command line argument parser
 *
 * @param handle t_program_opts pointer
 */
void
*shell_args_init(void *handle);

/******************************************************************************/
/* Misc utilities                                                             */
/******************************************************************************/

/**
 * @brief Reads incoming data from file descriptor `fd` to string `buf`
 *
 * @param shell The shell session
 * @param fd File descriptor to read from
 * @param buf Buffer to write to (must be initialized with capacity >= 1024)
 */
void
read_fd_to_string(t_shell *shell, int fd, t_string_buffer *buf);

#endif // SHELL_H
