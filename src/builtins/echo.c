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
#include "ft_printf.h"
#include <shell/eval.h>

static int
	echo(t_environ *env, int argc, char **argv)
{
	size_t	i;

	i = 1;
	while (i < argc)
	{
		ft_printf("%s", argv[i]);
		++i;
	}
	ft_printf("\n");
	return (0);
}

const t_builtin
	*builtin_echo(void)
{
	static const t_builtin	builtin = (t_builtin){
		.name = "echo",
		.desc = "The echo builtin",
		.fn = echo,
	};

	return (&builtin);
}
