/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgamba <linogamba@pundalik.org>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 11:59:40 by lgamba            #+#    #+#             */
/*   Updated: 2025/03/17 11:59:41 by lgamba           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#ifndef ENV_H
# define ENV_H

#include "util/util.h"
# include <parser/parser.h>
# include <sys/stat.h>

typedef struct s_shell t_shell;

/******************************************************************************/
/* Path buf utility                                                           */
/******************************************************************************/

typedef struct s_pathbuf
{
	t_string_buffer	buf;
}	t_pathbuf;

/**
 * @brief Initializes an empty @ref t_pathbuf with given default capacity
 *
 * @param buf Pathbuf to initialize
 * @param initial_capacity Pathbuf initial capacity
 */
void
pathbuf_init(t_pathbuf *buf, size_t initial_capacity);
/**
 * @brief Appends a component to the @ref t_pathbuf
 *
 * @param buf Pathbuf to append to
 * @param component component to append (may be NULL)
 * @param force_dir Force the componene to be treated as a directory, e.g append
 * a `/` after the component
 *
 * @returns 1 on success, 0 on failure or if `component == NULL`
 */
int
pathbuf_append(t_pathbuf *buf, const char *component, int force_dir);
/**
 * @brief Converts the @ref t_pathbuf to a NUL-terminated string
 *
 * @note This function does not clone the internal buffer, so you can free
 * the returned `char *` and not have to worry about calling @ref pathbuf_free.
 *
 * @param buf Buffer to convert
 *
 * @returns A null terminated string of the buffer
 */
char
*pathbuf_cstr(t_pathbuf *buf);
/**
 * @brief Frees a pathbuf
 *
 * @param buf Pathbufto free
 */
void
pathbuf_free(t_pathbuf *buf);

/******************************************************************************/
/* $PATH and environment handling                                             */
/******************************************************************************/

/**
 * @brief Holds data for environment tree traversal
 */
struct s_envp_traversal
{
	size_t	index;
	char	**envp;
};

/**
 * @brief Populates the shell's rb tree from a string of the form `VAR=VALUE`
 *
 * @param shell The shell session
 * @param var Assignment string `VAR=VALUE`
 */
void
envp_populate(t_shell *shell, const char *s);

/**
 * @brief Creates envp from the shell's environment
 *
 * @param shell The shell session
 *
 * @returns A null-terminated array of strings containing environment variables
 */
char
**environ_to_envp(t_shell *shell);

/**
 * @brief Populates program entry from the shell's `PATH`
 *
 * This can be used to repopulate the `PATH` when requested.
 *
 * @param shell Shell session
 */
void
path_populate(t_shell *shell);
/**
 * @brief Resolves function/builtin/executable from the shell
 *
 * @param shell The shell session
 * @param name Executable name
 * @param result Result of this function, on return value 0, this will contain
 * the full path to the executable (which must be freed)
 *
 * @returns
 *  - (-1) on error
 *  - (0) if `name` refers to an executable
 *  - (1) if `name` refers to a special builtin
 *  - (2) if `name` refers to a function
 *  - (3) if `name` refers to a builtin
 */
int
resolve_eval(t_shell *shell, const char *name, char **result);

/******************************************************************************/
/* Utilities for reading/parsing /etc/passwd                                  */
/******************************************************************************/

/**
 * @brief Entry from `/etc/passwd`
 */
struct s_passwd_ent
{
	/** @brief A user's username */
	char	*username;
	/** @brief A user's home directory */
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
 * @param shell The shell session
 * @param username Username to query from. On success this function takes
 * ownership of `username`.
 * @param ent The passwd entry to store the result
 *
 * @returns 0 on failure, 1 on success. On success the resulting `ent` should
 * be passed to `passwd_free`.
 */
int
passwd_query(t_shell *shell, char *username, struct s_passwd_ent *ent);

/******************************************************************************/
/* Variables handling                                                         */
/******************************************************************************/

/** @brief A shell variable */
typedef struct s_shell_var
{
	/** @brief The variable's name */
	const char	*name;
	/** @brief The variable's value */
	char		*value;
	/** @brief Whether this variable is exported */
	int			exported;
}	t_shell_var;

/**
 * @brief Sets a variable's value
 *
 * This function first looks if a variable named `name` exists in each stack
 * frames. Then it will search in the prefix stack.
 * If it is not found, the variable will be set in the global
 * environment.
 *
 * @param shell The shell session
 * @param name Variable name
 * @param value Variable value
 * @param export If set to 1, the value will be inserted in the environment and
 * be exported
 */
void
set_variable(t_shell *shell, const char *name, char *value, int export);
/**
 * @brief Gets a variable's value
 *
 * This function first looks if a variable named `name` exists in each stack
 * frames.
 * If it is not found, the variable will be searched in the global
 * environment.
 *
 * @param shell The shell session
 * @param name Variable name
 *
 * @returns The variable if found, NULL otherwise.
 */
char
*get_variable_value(t_shell *shell, const char *name);
/**
 * @brief Gets a variable
 *
 * This function first looks if a variable named `name` exists in each stack
 * frames.
 * If it is not found, the variable will be searched in the global
 * environment.
 *
 * @param shell The shell session
 * @param name Variable name
 * @param var Variable to populate
 *
 * @returns 1 if found, 0 otherwise
 */
int
get_variable(t_shell *shell, const char *name, t_shell_var *var);
/**
 * @brief Unsets a variable
 *
 * This function first looks if a variable named `name` exists in each stack
 * frames. Then it will search in the prefix stack.
 * If it is not found, the variable will be searched in the global
 * environment.
 *
 * @param shell The shell session
 * @param name Variable name
 *
 * @returns 1 if found, 0 otherwise
 */
int
unset_variable(t_shell *shell, const char *name);

/******************************************************************************/
/* Prefix assignment handling                                                 */
/******************************************************************************/

/** @brief Variable from prefix assignment */
typedef struct s_prefix_var
{
	/** @brief Previous value if a variable is shadowed */
	char		*saved_value;
	/** @brief Set to 1 if the variable was exported */
	int			exported;
}	t_prefix_var;

/** @brief Stack for prefix assignments */
typedef struct s_prefix_stack
{
	/** @brief Saved variables, e.g list of variable to reinstall when the stack
	 * is popped */
	t_rbtree	*variables;
	/** @brief Number of assignment stack */
	size_t		size;
	/** @brief Capacity of assignment stack */
	size_t		capacity;
}	t_prefix_stack;

/** @brief Initializes prefix stack data for the shell */
void
prefix_stack_init(t_shell *shell);
/** @brief Frees the prefix stack for the shell */
void
prefix_stack_deinit(t_shell *shell);
/**
 * @brief Pushes a new assignments frame in the stack
 *
 * @param shell The shell session
 * @param assigns Assignments to push
 * @param size Number of assignments
 */
void
prefix_stack_push(
	t_shell *shell,
	struct s_assignment *assigns,
	size_t size);
/** @brief Pops a frame in the prefix assignments stack */
void
prefix_stack_pop(t_shell *shell);

/******************************************************************************/
/* Utilities                                                                  */
/******************************************************************************/

/**
 * @brief Evaluator function for @ref file_tree_walk
 *
 * @param path Path to the file, relative to search start
 * @param sb Returned deta by `lstat`
 * @param cookie Optional parameter given to @ref file_tree_walk
 *
 * @returns This function can return the following values:
 *  * -1 to stop searching
 *  * 0 to continue searching
 *  * 1 to prevent searching recursively over this directory (only used if `sb`
 *  refers to a directory)
 */
typedef int
(*t_ftw_fn)(
	char *path,
	const struct stat *sb,
	void *cookie);
/**
 * @brief Recursively walk a directory and it's content
 *
 * @param path Original path
 * @param max_depth Maximum search depth
 * @param fn The file tree walk callback
 * @param cookie Optional data passed to `fn`
 *
 * @returns -1 on error, 0 on success
 */
int
file_tree_walk(const char *path, size_t max_depth, t_ftw_fn fn, void *cookie);

/** @brief Data for file tree walk */
struct s_ftw
{
	size_t		max_depth;
	char		*fullpath;
	struct		stat sb;
	t_ftw_fn	fn;
	void		*cookie;
};

/******************************************************************************/
/* Profile management                                                         */
/******************************************************************************/

/**
 * @brief Attempts to source the default profile
 *
 * This function will first look in `~/.hshrc` if not found, it will look into
 * `$XDG_CONFIG_HOME/hsh/profile.sh`. If it's still not found a default profile
 * will be used and a message will be displayed.
 *
 * @param shell The shell session
 */
void
profile_source(t_shell *shell);

#endif // ENV_H
