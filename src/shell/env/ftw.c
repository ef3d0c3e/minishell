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
#include <shell/shell.h>

static char
	*get_path(const char *path, struct dirent *ent)
{
	t_pathbuf		buf;

	pathbuf_init(&buf, 24);
	if (ft_strcmp(path, "."))
		pathbuf_append(&buf, path, 0);
	pathbuf_append(&buf, ent->d_name, 0);
	return (pathbuf_cstr(&buf));
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
	while (dir)
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
			continue ;
		}
		fstatus = f->fn(fullpath, &f->sb, f->cookie);
		if (fstatus == -1)
		{
			free(fullpath);
			continue ;
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
