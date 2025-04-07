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
#include "ft_printf.h"
#include <fcntl.h>
#include <shell/eval.h>

void
	passwd_free(struct s_passwd_ent *ent)
{
	free(ent->username);
	free(ent->group);
	free(ent->homedir);
}


int
	passwd_query(t_environ *env, char *username, struct s_passwd_ent *ent)
{
	char	*err;
	int		fd;

	fd = open("/etc/passwd", O_RDONLY);
	if (fd == -1)
	{
		ft_asprintf(&err, "Failed to open(\"/etc/passwd\"): %m");
		shell_error(env, err, __func__);
		return (0);
	}
	close(fd);
	return (1);
}
