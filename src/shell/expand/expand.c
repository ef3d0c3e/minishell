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
#include "shell/expand/expand.h"
#include "ft_printf.h"
#include "parser/parser.h"
#include "util/util.h"
#include <shell/shell.h>
#include <stddef.h>

void
	argv_push(char ***argv, size_t len, char *str)
{
	printf("PUSH %zu %s %p\n", len, str, *argv);
	*argv = ft_realloc(*argv, sizeof(char **) * len, sizeof(char **) * (len + 1));
	(*argv)[len] = str;
}

void
	argv_append(char **argv, const char *app)
{
	char			*new;
	char			*av;

	if (*argv)
	{
		ft_asprintf(&new, "%s%s", *argv, app);
		free(*argv);
		*argv = new;
	}
	else
		*argv = ft_strdup(app);
}

void
	expand_arg(t_shell *shell, struct s_argument *arg, char ***argv, size_t *len)
{
	size_t	i;

	argv_push(argv, *len, NULL);
	i = 0;
	while (i < arg->nitems)
	{
		if (arg->items[i].type == ARG_LITERAL)
		{
			argv_append(&(*argv)[*len], stringbuf_cstr(&arg->items[i].text));
		}
		else if (arg->items[i].type == ARG_SUBEXPR)
		{
			// TODO
		}
		else if (arg->items[i].type == ARG_PARAMETER)
			expand_param(shell, &arg->items[i].param, argv, len);
		++i;
	}
	(*len)++;
}

char
	**arg_expansion(t_shell *shell, const struct s_cmd_node *cmd)
{
	char	**argv;
	size_t	size;
	size_t	i;

	size = 0;
	argv = NULL;
	i = 0;
	while (i < cmd->nargs)
	{
		expand_arg(shell, &cmd->args[i], &argv, &size);
		++i;
	}
	argv_push(&argv, size, NULL);
	return (argv);
}
