/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_param_simple.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgamba <linogamba@pundalik.org>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 11:59:40 by lgamba            #+#    #+#             */
/*   Updated: 2025/03/17 11:59:41 by lgamba           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "tokenizer.h"
#include "util/util.h"

int
	is_param_ident_start(char c)
{
	return (c == '#' || c == '@' || c == '*' || c == '$' || c == '-'
		|| is_param_ident(c));
}

int
	is_param_ident(char c)
{
	return (c == '_'
		|| (c >= '0' && c <= '9')
		|| (c >= 'A' && c <= 'Z')
		|| (c >= 'a' && c <= 'z'));
}

/** @brief Handles special parameters that will be resolved at evaluation
 * time */
static int
	param_special(t_token_list *list, t_u8_iterator *it)
{
	static const char	*special[] = {
		"$?", NULL
	};
	const char			*kind = str_alternatives(it_substr(it, 2), special);

	if (kind)
	{
		token_list_push(list, TOK_PARAM_SIMPLE, it->byte_pos + 1,
			it->byte_pos + ft_strlen(kind))->word = stringbuf_from(kind + 1);
		it_advance(it, ft_strlen(kind));
		return (1);
	}
	return (0);
}

int
	token_param_simple(t_token_list *list, t_u8_iterator *it)
{
	const size_t	start = it->byte_pos;
	t_string		next;
	t_string_buffer	buf;


	if (it->codepoint.str[0] != '$')
		return (0);
	next = it_substr(it, 2);
	if (next.len != 2 || !is_param_ident_start(next.str[1]))
		return (0);
	it_advance(it, 2);
	stringbuf_init(&buf, 64);
	stringbuf_append(&buf, (t_string){next.str + 1, 1});
	while (it->codepoint.len == 1 && is_param_ident(it->codepoint.str[0]))
	{
		stringbuf_append(&buf, it->codepoint);
		it_next(it);
	}
	token_list_push(list, TOK_PARAM_SIMPLE, start, it->byte_pos)->word
		= buf;
	return (1);
}

