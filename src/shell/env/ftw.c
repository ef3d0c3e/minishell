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
	const size_t	len = ft_strlen(path);
	int				force_dir;

	force_dir = 1;
	if (len && path[len - 1] == '/')
		force_dir = 0;
	pathbuf_init(&buf, 24);
	if (ft_strcmp(path, "."))
		pathbuf_append(&buf, path, force_dir);
	pathbuf_append(&buf, ent->d_name, 0);
	return (pathbuf_cstr(&buf));
}

static int
	file_tree_walk_impl(struct s_ftw *f, size_t depth, const char *path);

static int
	ftw_call(
	struct s_ftw *f,
	const char *path,
	DIR *dir,
	size_t depth)
{
	char			*fullpath;
	struct dirent	*ent;
	int				status;
	int				fstatus;

	errno = 0;
	status = 0;
	ent = readdir(dir);
	if (errno)
		return (-1);
	if (!ent)
		return (-2);
	if (!ft_strcmp(ent->d_name, "..") || !ft_strcmp(ent->d_name, "."))
		return (0);
	fullpath = get_path(path, ent);
	if (lstat(fullpath, &f->sb) == -1)
		return (free(fullpath), 0);
	fstatus = f->fn(fullpath, &f->sb, f->cookie);
	if (fstatus == -1)
		return (free(fullpath), 0);
	if (fstatus != 1 && S_ISDIR(f->sb.st_mode))
		status = file_tree_walk_impl(f, depth + 1, fullpath);
	return (free(fullpath), status);
}

static int
	file_tree_walk_impl(struct s_ftw *f, size_t depth, const char *path)
{
	DIR				*dir;
	int				status;

	if (depth >= f->max_depth)
		return (0);
	dir = opendir(path);
	while (dir)
	{
		status = ftw_call(f, path, dir, depth);
		if (status < 0)
		{
			if (status == -2)
				status = 0;
			break ;
		}
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
