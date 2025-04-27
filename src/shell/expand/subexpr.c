/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   subexpr.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgamba <linogamba@pundalik.org>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 11:59:40 by lgamba            #+#    #+#             */
/*   Updated: 2025/03/17 11:59:41 by lgamba           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include <shell/shell.h>

int
	expand_subexpr(
	t_shell *shell,
	t_fragment_list *list,
	struct s_arg_item *param)
{
	char			*info;
	t_string_buffer	result;

	ft_asprintf(&info, "$(%s)", stringbuf_cstr(&param->text));
	// Right here, the program will be forked then the fork will exit, so every
	// local heap-allocated variables are going to be the source of memory leaks.
	// This preventable, either via extensive bookkeeping or by using execve tricks
	result = ctx_eval_string(shell, ft_strdup(stringbuf_cstr(&param->text)), info);
	if (shell->last_status != 0)
	{
		// An error happened
		stringbuf_free(&result);
		return (0);
	}
	while (result.len && result.str[result.len - 1] == '\n')
		--result.len;
	fraglist_push(list, result, param->flags);
	return (1);
}
