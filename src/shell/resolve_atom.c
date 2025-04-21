/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   resolve_atom.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgamba <linogamba@pundalik.org>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 11:59:40 by lgamba            #+#    #+#             */
/*   Updated: 2025/03/17 11:59:41 by lgamba           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include <shell/eval.h>
#include <stdio.h>

char
	*resolve_arg(t_environ *env, const struct s_node_compound *comp)
{
	t_string_buffer	buf;
	t_string		str;
	size_t			i;

	i = 0;
	stringbuf_init(&buf, 16);
	while (i < comp->nitems)
	{
		str.str = comp->items[i].atom.str;
		str.len = comp->items[i].atom.len;
		if (comp->items[i].type == NODE_ATOM)
			stringbuf_append(&buf, str);
		else if (comp->items[i].type == NODE_PARAMETER)
		{
			if (!str_cmp(str, "?"))
				stringbuf_itoa(&buf, env->last_status);
			else
				shell_fail(env, "Invalid parameter in lazy-expansion",
					SRC_LOCATION);
		}
		++i;
	}
	return (stringbuf_cstr(&buf));
}

/**
 * @brief Resolves command parameters to an array of strings
 *
 * @param env The shell session
 * @param cmd Command to create an array of string from
 *
 * @returns A null-terminated array of strings to be passed to `execve`
 */
char
	**resolve_command(t_environ *env, const struct s_node_cmd *cmd)
{
	char	**result;
	size_t	i;

	result = xmalloc(sizeof(char*) * (cmd->nargs + 1));
	i = 0;
	while (i < cmd->nargs)
	{
		result[i] = resolve_arg(env, &cmd->args[i]);
		++i;
	}
	result[i] = NULL;
	return (result);
}
