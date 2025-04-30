/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   source.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgamba <linogamba@pundalik.org>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 11:59:40 by lgamba            #+#    #+#             */
/*   Updated: 2025/03/17 11:59:41 by lgamba           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "shell/env/env.h"
#include <shell/shell.h>

static int
	unset(t_shell *shell, int argc, char **argv)
{
	if (argc != 2)
	{
		ft_dprintf(2, "USAGE: unset VARIABLE\n");
		return (1);
	}
	return (!unset_variable(shell, argv[1]));
}

const t_builtin
	*builtin_unset(void)
{
	static const t_builtin	builtin = (t_builtin){
		.name = "unset",
		.desc = "The unset builtin",
		.run = unset,
		.init = NULL,
		.deinit = NULL,
	};

	return (&builtin);
}
