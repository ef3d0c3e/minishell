/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thschnei <thschnei@student.42perpignan.fr  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/13 11:34:13 by thschnei          #+#    #+#             */
/*   Updated: 2025/06/13 12:36:18 by thschnei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include <shell/shell.h>

static int
	write_msg(const char *msg)
{
	const size_t	len = ft_strlen(msg);
	ssize_t			written;

	written = write(1, msg, len);
	if (written == -1 || (size_t)written != len)
	{
		if (errno != EINTR)
			ft_dprintf(2, "pwd: Write error");
		return (0);
	}
	return (1);
}

static int
	pwd(t_shell *shell, int argc, char **argv)
{
	char	*path;
	(void)shell;
	(void)argc;
	(void)argv;
	path = getcwd(NULL, 0);
	if (!path)
		ft_dprintf(2, "pwd: Alloc error");
	else
	{
		if (!write_msg(path))
		{
			free(path);
			return (errno == EINTR);
		}
		if (!write_msg("\n"))
			return (errno == EINTR);
	}
	free(path);
	return (0);
}

const t_builtin
	*builtin_pwd(void)
{
	static const t_builtin	builtin = (t_builtin){
		.name = "pwd",
		.desc = "The pwd builtin",
		.run = pwd,
		.init = NULL,
		.deinit = NULL,
	};

	return (&builtin);
}
