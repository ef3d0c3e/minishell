/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgamba <linogamba@pundalik.org>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 11:59:40 by lgamba            #+#    #+#             */
/*   Updated: 2025/03/17 11:59:41 by lgamba           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "ft_printf.h"
#include "shell/eval.h"
#include "tokenizer/tokenizer.h"
#include "util/util.h"
#include <expansion/expansion.h>

/** @brief Removes trailing newlines, according to bash's rules */
void
remove_trailing(t_string_buffer *buf)
{
	while (buf->len && buf->str[buf->len - 1] == '\n')
		--buf->len;
}

int
	expand_cmdsub(t_environ *env, t_token *token, t_token_list *result)
{
	t_string_buffer	buf;
	char			*err;
	int				status;

	if (token->type != TOK_CMD_SUB)
		return (0);

	status = repl_to_string(env, stringbuf_cstr(&token->word), &buf);
	if (status)
	{
		ft_asprintf(&err, "Command substitution for `%.*s` failed: return "
			"value=%d", (int)token->word.len, token->word.str, status);
		token_free(token);
		shell_error(env, err, __func__);
		return (1);
	}
	remove_trailing(&buf);
	// TODO: Perform word splitting
	token_list_push(result, (t_token){
		.type = TOK_SINGLE_QUOTE,
		.start = token->start,
		.end = token->end,
		.word = buf
	});
	token_free(token);
	return (1);
}

