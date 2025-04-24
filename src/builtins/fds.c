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

static int
	fds(t_shell *shell, int argc, char **argv)
{
	(void)shell;
	(void)argc;
	(void)argv;
	return (0);
}

const t_builtin
	*builtin_fds(void)
{
	static const t_builtin	builtin = (t_builtin){
		.name = "fds",
		.desc = "List all used general purpose file descriptors",
		.fn = fds,
	};

	return (&builtin);
}
