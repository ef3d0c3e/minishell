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
	is_param_ident(char c)
{
	return (c == '_' || c == '-'
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
	t_u8_iterator	cpy;
	t_string_buffer	buf;


	if (it->codepoint.str[0] != '$')
		return (0);
	if (param_special(list, it))
		return (1);
	cpy = *it;
	it_advance(&cpy, 1);
	while (cpy.codepoint.len == 1 && is_param_ident(cpy.codepoint.str[0]))
		it_next(&cpy);
	if (cpy.byte_pos == it->byte_pos + 1)
	{
		stringbuf_init(&buf, 8);
		stringbuf_append(&buf, it->codepoint);
		token_list_push(list, TOK_WORD, start,
			start + 1)->word = buf;
	}
	else
		token_list_push(list, TOK_PARAM_SIMPLE, start, cpy.byte_pos)->word 
			= stringbuf_from_range(it->str.str + start + 1,
			it->str.str + cpy.byte_pos);
	*it = cpy;
	return (1);
}

