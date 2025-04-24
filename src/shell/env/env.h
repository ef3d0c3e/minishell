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

# include <util/util.h>

typedef struct s_shell t_shell;

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
 *  - (1) if `name` refers to a function
 *  - (2) if `name` refers to a builtin
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
passwd_query(t_shell *shell, char *username, struct s_passwd_ent *ent);

#endif // ENV_H
