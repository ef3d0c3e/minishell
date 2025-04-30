/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   source.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgamba <linogamba@pundalik.org>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 11:59:40 by lgamba            #+#    #+#             */
/*   Updated: 2025/03/17 11:59:41 by lgamba           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "ft_printf.h"
#include "util/util.h"
#include <shell/shell.h>
#include <unistd.h>

static int
	cd_var(t_shell *shell, const char *varname)
{
	char	*path;

	path = get_variable_value(shell, varname);
	if (!path)
	{
		ft_dprintf(2, "cd: %s not set\n", varname);
		return (1);
	}
	if (chdir(path) == -1)
	{
		ft_dprintf(2, "cd: chdir('%s') failed: %m\n", path);
		return (1);
	}
	return (0);
}

static int
	cd(t_shell *shell, int argc, char **argv)
{
	char	*path;
	char	*cwd;

	if (argc > 2)
	{
		ft_dprintf(2, "USAGE: cd PATH\n");
		return (1);
	}
	if (argc == 1 || !argv[1][0])
		return (cd_var(shell, "HOME"));
	else if (!ft_strcmp(argv[1], "-"))
		return (cd_var(shell, "OLDPWD"));
	if (argv[1][0] == '/')
		path = ft_strdup(argv[1]);
	else
	{
		cwd = getcwd(NULL, 0);
		if (!cwd)
			return (ft_dprintf(2, "cd: getcwd() failed: %m\n"), 1);
		ft_asprintf(&path, "%s/%s", cwd, argv[1]);
	}
	if (argv[1][0] != '/')
		free(cwd);
	if (chdir(path) == -1)
	{
		ft_dprintf(2, "cd: chdir('%s') failed: %m\n", path);
		free(path);
		return (0);
	}
	free(path);
	return (0);
}

const t_builtin
	*builtin_cd(void)
{
	static const t_builtin	builtin = (t_builtin){
		.name = "cd",
		.desc = "The cd builtin",
		.run = cd,
		.init = NULL,
		.deinit = NULL,
	};

	return (&builtin);
}
