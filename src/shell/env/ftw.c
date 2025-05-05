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
#include <sys/stat.h>

static char
	*get_path(const char *path, struct dirent *ent)
{
	char	*formatted;
	int		len;

	len = ft_strlen(path);
	while (len > 0 && path[len - 1] == '/')
		--len;
	ft_asprintf(&formatted, "%.*s/%s", len, path, ent->d_name);
	return (formatted);
}

int
	file_tree_walk(
	const char *path,
	size_t max_depth,
	t_ftw_fn fn,
	void *cookie)
{
	DIR				*dir;
	struct dirent	*ent;
	struct stat		sb;
	int				status;
	char			*fullpath;

	if (!max_depth)
		return (0);
	dir = opendir(path);
	status = 1;
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
		if (lstat(ent->d_name, &sb) == -1)
			break ;
		fullpath = get_path(path, ent);
		status = fn(ent->d_name, &sb, cookie);
		free(fullpath);
		if (status == -1)
			break ;
		if (status != 1 && S_ISDIR(sb.st_mode))
			if (file_tree_walk(path, max_depth - 1, fn, cookie) == 1)
			{
				status = 0;
				break ;
			}
	}
	if (dir)
		closedir(dir);
	return (status);
}
