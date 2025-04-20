/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   resolve.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgamba <linogamba@pundalik.org>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 11:59:40 by lgamba            #+#    #+#             */
/*   Updated: 2025/03/17 11:59:41 by lgamba           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "util/util.h"
#include <shell/eval.h>

/** @brief Gets the status of an executable
 *
 * @param pathname Executable pathname
 *
 * @returns
 *  - (-1) if the file is not executable/no permission/not found
 *  - (0) if the file is executable
 */
static int
	get_executable_status(t_environ *env, const char *pathname)
{
	struct stat	sb;
	char		*err;

	if (lstat(pathname, &sb) == -1)
	{
		if (errno == EACCES)
			ft_asprintf(&err, "Failed to execute `%s`: No permission", pathname);
		else
			ft_asprintf(&err, "lstat(`%s`) failed: %m", pathname);
		shell_error(env, err, __FUNCTION__);
		return (-1);
	}
	return (0);
}

/**
 * @brief Resolvevs an executable from the $PATH
 *
 * If the executable `name` is not found in $PATH, the path is updated once
 * then searched again. If no matches are found, an error is thrown and NULL
 * is returned.
 */
static char
	*resolve_from_path(t_environ *env, const char *name)
{
	char	*resolved;
	char	*msg;

	resolved = rb_find(&env->path_program, name);
	if (!resolved)
	{
		path_populate(env);
		resolved = rb_find(&env->path_program, name);
		if (!resolved)
		{
			ft_asprintf(&msg, "Failed to find executable `%s`", name);
			shell_error(env, msg, __func__);
			return (NULL);
		}
	}
	return (ft_strdup(resolved));
}

int
	resolve_executable(t_environ *env, const char *name, char **result)
{
	char		*cwd;
	char		*err;
	int			status;

	if (!ft_strchr(name, '/'))
	{
		// TODO: Function then builtin resolution
		if (rb_find(&env->builtins, name))
			return (2);
		*result = resolve_from_path(env, name);
		if (!*result)
			return (-1);
		status = get_executable_status(env, *result);
		return (status);
	}
	if (name[0] == '/')
	{
		*result = ft_strdup(name);
		return (get_executable_status(env, *result));
	}
	cwd = getcwd(NULL, 0);
	if (!cwd)
	{
		ft_asprintf(&err, "getcwd() failed: %m");
		shell_error(env, err, __FUNCTION__);
		return (-1);
	}
	ft_asprintf(result, "%s/%s", cwd, name);
	status = get_executable_status(env, *result);
	free(cwd);
	return (status);
}
