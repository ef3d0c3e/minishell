/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   passwd.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgamba <linogamba@pundalik.org>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 11:59:40 by lgamba            #+#    #+#             */
/*   Updated: 2025/03/17 11:59:41 by lgamba           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "util/util.h"
#include <shell/eval.h>
#include <stddef.h>

void
	passwd_free(struct s_passwd_ent *ent)
{
	free(ent->username);
	free(ent->homedir);
}

/** @brief Parse single entry */
static inline void
	parse_entry(const char *line, char *username, struct s_passwd_ent *ent)
{
	const char	*sep = ft_strchr(line, ':');
	const char	*prev = sep;
	size_t		count;

	ent->username = username;
	count = 0;
	while (sep || prev)
	{
		if (count == 4 && sep)
			ent->homedir = ft_substr(prev, 0, sep - prev);
		else if (count == 4 && !sep)
			ent->homedir = ft_strdup(prev);
		++count;
		prev = sep + 1;
		sep = ft_strchr(prev, ':');
	}
}

/** @brief Read /etc/passwd file line by line */
static inline int
	read_entry(int fd, char *username, struct s_passwd_ent *ent)
{
	const size_t	len = ft_strlen(username);
	const char	*line;
	int			found;

	found = 0;
	ent->homedir = NULL;
	ent->username = NULL;
	line = get_next_line(fd);
	while (line)
	{
		if (!ft_strcmp(line, username) && line[len] == ':')
		{
			parse_entry(line + len + 1, username, ent);
			found = 1;
		}
		line = get_next_line(fd);
	}
	return (found);
}

int
	passwd_query(t_environ *env, char *username, struct s_passwd_ent *ent)
{
	char	*err;
	int		fd;
	int		ret;

	fd = open("/etc/passwd", O_RDONLY);
	if (fd == -1)
	{
		ft_asprintf(&err, "Failed to open(\"/etc/passwd\"): %m");
		shell_error(env, err, __func__);
		return (0);
	}
	ret = read_entry(fd, username, ent);
	close(fd);
	return (ret);
}
