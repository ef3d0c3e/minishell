/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   subexpr.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgamba <linogamba@pundalik.org>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 11:59:40 by lgamba            #+#    #+#             */
/*   Updated: 2025/06/19 07:34:15 by thschnei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include <shell/shell.h>

int
	expand_subexpr(
	t_shell *shell,
	t_fragment_list *list,
	struct s_atom *param)
{
	char						*info;
	t_string_buffer				*buf;
	struct s_eval_string_result	result;

	ft_asprintf(&info, "$(%s)", stringbuf_cstr(&param->text));
	result
		= ctx_eval_string(shell, ft_strdup(stringbuf_cstr(&param->text)), info);
	if (shell->last_status || result.result.type != RES_NONE)
	{
		stringbuf_free(&result.content);
		return (0);
	}
	buf = &result.content;
	while (buf->len && buf->str[buf->len - 1] == '\n')
		--buf->len;
	fraglist_push(list, result.content, param->flags);
	return (1);
}
