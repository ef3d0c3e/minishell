/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgamba <linogamba@pundalik.org>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 11:59:40 by lgamba            #+#    #+#             */
/*   Updated: 2025/03/17 11:59:41 by lgamba           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "util/util.h"
#include <shell/shell.h>
#include <stddef.h>
#include <stdlib.h>

static void
	argv_push(char **argv, size_t len, char *str)
{
	argv = ft_realloc(argv, sizeof(char **) * len, sizeof(char **) * (len + 1));
	argv[len] = str;
}

char
	**arg_expansion(t_shell *shell, const struct s_cmd_node *cmd)
{
	char	**argv;
	size_t	size;
	size_t	i;

	size = 0;
	argv = NULL;
	while (cmd->nargs)
	{
	}
}
