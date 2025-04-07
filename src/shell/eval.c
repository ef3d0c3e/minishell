/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   eval.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgamba <linogamba@pundalik.org>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 11:59:40 by lgamba            #+#    #+#             */
/*   Updated: 2025/03/17 11:59:41 by lgamba           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "eval.h"
#include "util/util.h"
#include <stdio.h>
#include <dirent.h>
#include <unistd.h>

char
	*find_in_dir(t_environ *env, const char *dirname, t_string name)
{
	char			*err;
	DIR				*dir;
	struct dirent	*ent;
	char			*result;

	dir = opendir(dirname);
	if (!dir)
	{
		asprintf(&err, "Failed to open directory '%s': %m", dirname);
		shell_error(env, err, __func__);
		return (NULL);
	}
	ent = readdir(dir);
	while (ent)
	{
		if (!str_cmp(name, ent->d_name))
		{
			asprintf(&result, "%s/%.256s", dirname, ent->d_name);
			break ;
		}
		ent = readdir(dir);
	}
	closedir(dir);
	if (!ent)
		return (NULL);
	return (result);
}

char
	*find_path(t_environ *env, t_string name)
{
	const char	*path = rb_find(&env->env, "PATH");
	const char	*prev = path;
	const char	*sep;
	char		*dirname;
	char		*found;

	if (!path)
		return (NULL);
	sep = ft_strchr(path, ':');
	while (sep)
	{
		dirname = ft_substr(prev, 0, sep - prev);
		found = find_in_dir(env, dirname, name);
		free(dirname);
		if (found)
			return (found);
		prev = sep + 1;
		sep = ft_strchr(prev, ':');
	}
	return (NULL);
}



void
	eval(t_environ *env, t_ast_node* program)
{
	if (program->type == NODE_COMMAND)
	{
		t_string_buffer atom = program->cmd.args[0].atom;
		char *p = find_path(env, (t_string){atom.str, atom.len});
		printf("Found: `%s`\n", p);
		free(p);
	}
}
