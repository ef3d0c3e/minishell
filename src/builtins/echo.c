/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgamba <linogamba@pundalik.org>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 11:59:40 by lgamba            #+#    #+#             */
/*   Updated: 2025/03/17 11:59:41 by lgamba           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include <shell/shell.h>

static void
	write_msg(const char *msg)
{
	const size_t	len = ft_strlen(msg);
	ssize_t			written;

	written = write(1, msg, len);
	if (written == -1 || (size_t)written != len)
	{
		ft_dprintf(2, "echo: Write error");
		exit(EXIT_FAILURE);
	}
}

static int
	echo(t_shell *shell, int argc, char **argv)
{
	int	i;
	int	start;
	int	newline;

	(void)shell;
	newline = 1;
	start = 1;
	if (start < argc && !ft_strcmp(argv[start], "-n"))
	{
		newline = 0;
		++start;
	}
	i = start;
	while (i < argc)
	{
		if (i != start)
			write_msg(" ");
		write_msg(argv[i]);
		++i;
	}
	if (newline)
		write_msg("\n");
	return (0);
}

const t_builtin
	*builtin_echo(void)
{
	static const t_builtin	builtin = (t_builtin){
		.name = "echo",
		.desc = "The echo builtin",
		.run = echo,
		.init = NULL,
		.deinit = NULL,
	};

	return (&builtin);
}
