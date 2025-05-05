/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ftw.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgamba <linogamba@pundalik.org>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 11:59:40 by lgamba            #+#    #+#             */
/*   Updated: 2025/03/17 11:59:41 by lgamba           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "ft_printf.h"
#include "shell/env/env.h"
#include "util/util.h"
#include <dirent.h>
#include <shell/shell.h>
#include <stddef.h>
#include <stdio.h>
#include <sys/stat.h>

static char
	*get_path(const char *path, struct dirent *ent)
{
	char	*formatted;
	int		len;

	len = ft_strlen(path);
	while (len > 0 && path[len - 1] == '/')
		--len;
	if (len == 1 && path[0] == '.')
		len = 0;
	ft_asprintf(&formatted, "%.*s%s%s", len, path, &"/"[len == 0], ent->d_name);
	return (formatted);
}

static int
	file_tree_walk_impl(struct s_ftw *f, size_t depth, const char *path)
{
	DIR				*dir;
	struct dirent	*ent;
	char			*fullpath;
	int				status;
	int				fstatus;

	if (depth >= f->max_depth)
		return (0);
	dir = opendir(path);
	status = -1;
	while (1)
	{
		errno = 0;
		ent = readdir(dir);
		if (errno)
			break ;
		if (!ent)
		{
			status = 0;
			break ;
		}
		if (!ft_strcmp(ent->d_name, "..") || !ft_strcmp(ent->d_name, "."))
			continue ;
		fullpath = get_path(path, ent);
		if (lstat(fullpath, &f->sb) == -1)
		{
			free(fullpath);
			break ;
		}
		fstatus = f->fn(fullpath, &f->sb, f->cookie);
		if (fstatus == -1)
		{
			free(fullpath);
			break ;
		}
		if (fstatus != 1 && S_ISDIR(f->sb.st_mode))
		{
			status = file_tree_walk_impl(f, depth + 1, fullpath);
		}
		free(fullpath);
	}
	if (dir)
		closedir(dir);
	return (status);
}

int
	file_tree_walk(
	const char *path,
	size_t max_depth,
	t_ftw_fn fn,
	void *cookie)
{
	struct s_ftw	f;

	if (!max_depth)
		return (0);
	f.max_depth = max_depth;
	f.fn = fn;
	f.cookie = cookie;
	return (file_tree_walk_impl(&f, 0, path));
}
