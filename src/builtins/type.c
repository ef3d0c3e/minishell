/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   type.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgamba <linogamba@pundalik.org>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 11:59:40 by lgamba            #+#    #+#             */
/*   Updated: 2025/03/17 11:59:41 by lgamba           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include <shell/shell.h>

static int
	type(t_shell *shell, int argc, char **argv)
{
	char	*resolved;
	int		result;

	if (argc != 2)
		return (0);
	resolved = NULL;
	result = resolve_eval(shell, argv[1], &resolved);
	if (result == 0)
		printf("%s is %s\n", argv[1], resolved);
	else if (result == 1 || result == 3)
		printf("%s is a shell builtin\n", argv[1]);
	else if (result == 2)
		printf("%s is a function\n", argv[1]);
	else
		ft_dprintf(2, "type: %s not found\n", argv[1]);
	free(resolved);
	return (0);
}

const t_builtin
	*builtin_type(void)
{
	static const t_builtin	builtin = (t_builtin){
		.name = "type",
		.desc = "The type builtin",
		.run = type,
		.init = NULL,
		.deinit = NULL,
	};

	return (&builtin);
}
