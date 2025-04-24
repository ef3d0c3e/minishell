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
#include <shell/eval.h>

static int
	echo(t_shell *shell, int argc, char **argv)
{
	int	i;
	int	start;
	int	newline;

	(void)env;
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
			ft_printf(" ");
		ft_printf("%s", argv[i]);
		++i;
	}
	if (newline)
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
