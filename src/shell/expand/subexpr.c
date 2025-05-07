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
#include "ft_printf.h"
#include "shell/ctx/ctx.h"
#include "shell/eval/eval.h"
#include "util/util.h"
#include <shell/shell.h>

int
	expand_subexpr(
	t_shell *shell,
	t_fragment_list *list,
	struct s_atom *param,
	const char *ifs)
{
	char						*info;
	t_string_buffer				*buf;
	struct s_eval_string_result	result;

	(void)ifs;
	ft_asprintf(&info, "$(%s)", stringbuf_cstr(&param->text));
	result = ctx_eval_string(shell, ft_strdup(stringbuf_cstr(&param->text)), info);
	if (result.result.type != RES_NONE)
	{
		// An error happened
		stringbuf_free(&result.stdout);
		return (0);
	}
	buf = &result.stdout;
	while (buf->len && buf->str[buf->len - 1] == '\n')
		--buf->len;
	fraglist_push(list, result.stdout, param->flags);
	return (1);
}
