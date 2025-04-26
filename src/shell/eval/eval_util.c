/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   eval_util.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgamba <linogamba@pundalik.org>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 11:59:40 by lgamba            #+#    #+#             */
/*   Updated: 2025/03/17 11:59:41 by lgamba           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "parser/parser.h"
#include <shell/shell.h>

/** @brief Translates single command argument (Compound) to string, optionnaly
 * performing lazy-expansion */
static char
	*resolve_arg(t_shell *shell, const struct s_argument *arg)
{
	t_string_buffer	buf;
	t_string		str;
	size_t			i;

	i = 0;
	stringbuf_init(&buf, 16);
	while (i < arg->nitems)
	{
		str.str = arg->items[i].data.str;
		str.len = arg->items[i].data.len;
		if (arg->items[i].type == ARG_LITERAL)
			stringbuf_append(&buf, str);
		else if (arg->items[i].type == ARG_PARAMETER)
		{
			if (!str_cmp(str, "?"))
				stringbuf_itoa(&buf, shell->last_status);
			else
				shell_fail(shell, "Invalid parameter in lazy-expansion",
					SRC_LOCATION);
		}
		++i;
	}
	return (stringbuf_cstr(&buf));
}


char
	**command_to_argv(t_shell *shell, const struct s_cmd_node *cmd)
{
	char	**result;
	size_t	i;

	result = xmalloc(sizeof(char*) * (cmd->nargs + 1));
	i = 0;
	while (i < cmd->nargs)
	{
		result[i] = resolve_arg(shell, &cmd->args[i]);
		++i;
	}
	result[i] = NULL;
	return (result);
}
