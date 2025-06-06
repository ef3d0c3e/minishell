/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   path.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgamba <linogamba@pundalik.org>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 11:59:40 by lgamba            #+#    #+#             */
/*   Updated: 2025/03/17 11:59:41 by lgamba           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include <shell/shell.h>

/** @brief Check that a given program is executable */
static void
	path_check(t_shell *shell, const char *dirname, struct dirent *ent)
{
	struct stat	sb;
	char		*fullpath;
	char		*err;
	size_t		len;

	len = ft_strlen(dirname);
	while (len > 0 && dirname[len - 1] == '/')
		--len;
	ft_asprintf(&fullpath, "%.*s/%.256s", (int)len, dirname, ent->d_name);
	if (lstat(fullpath, &sb))
	{
		ft_asprintf(&err, "Failed to lstat(\"%s\"): %m", fullpath);
		shell_error(shell, err, SRC_LOCATION);
		free(fullpath);
		return ;
	}
	if (S_ISREG(sb.st_mode) || S_ISLNK(sb.st_mode))
		rb_insert(&shell->path_cache, ft_strdup(ent->d_name), fullpath);
	else
		free(fullpath);
}

/** @brief Populate from directory */
static void
	populate_path(t_shell *shell, const char *dirname)
{
	char			*str;
	DIR				*dir;
	struct dirent	*ent;

	dir = opendir(dirname);
	if (!dir)
	{
		ft_asprintf(&str, "Failed to opendir(\"%s\"): %m", dirname);
		shell_error(shell, str, SRC_LOCATION);
		return ;
	}
	while (dir)
	{
		errno = 0;
		ent = readdir(dir);
		if (errno)
		{
			ft_asprintf(&str, "Failed to readdir(\"%s\"): %m", dirname);
			shell_error(shell, str, SRC_LOCATION);
		}
		if (!ent)
			break ;
		path_check(shell, dirname, ent);
	}
	closedir(dir);
}

/** @brief Reads the `PATH` and populate the programs entries */
static void
	read_path(t_shell *shell)
{
	const char	*path = get_variable_value(shell, "PATH");
	const char	*prev = path;
	const char	*sep;
	char		*dirname;

	if (!path)
		return ;
	sep = ft_strchr(path, ':');
	while (sep || prev)
	{
		if (sep)
			dirname = ft_substr(prev, 0, sep - prev);
		else
			dirname = ft_substr(prev, 0, ft_strlen(prev));
		populate_path(shell, dirname);
		free(dirname);
		if (!sep)
			return ;
		prev = sep + 1;
		sep = ft_strchr(prev, ':');
	}
}

void
	path_populate(t_shell *shell)
{
	rb_free(&shell->path_cache);
	read_path(shell);
}
