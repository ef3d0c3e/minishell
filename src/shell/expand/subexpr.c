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

/** @brief Trim the result of a subexpr using the shell's IFS */
static void
trim_subexpr(t_string_buffer *buf)
{
	const char *ifs = " \t\n";
	size_t start, end;

	for (start = 0; start < buf->len && ft_strchr(ifs, buf->str[start]);
		++start)
		;
	for (end = buf->len; end > start && ft_strchr(ifs, buf->str[end - 1]);
		--end)
		;
	ft_memmove(buf->str, buf->str + start, end - start);
	buf->len = end - start;
}

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
	trim_subexpr(buf);
	if (buf->len)
		fraglist_push(list, result.content, param->flags);
	else
		stringbuf_free(buf);
	return (1);
}
