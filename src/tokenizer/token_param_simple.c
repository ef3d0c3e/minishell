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

static inline int
	is_ident(t_string codepoint)
{
	return (codepoint.str[0] == '_' || codepoint.str[0] == '-'
		|| (codepoint.str[0] >= 'A' && codepoint.str[0] <= 'Z')
		|| (codepoint.str[0] >= 'a' && codepoint.str[0] <= 'z'));
}

int
	token_param_simple(t_token_list *list, t_u8_iterator *it)
{
	const size_t	start = it->byte_pos;
	t_u8_iterator	cpy;
	t_string_buffer	buf;


	if (it->codepoint.str[0] != '$')
		return (0);
	cpy = *it;
	it_advance(&cpy, 1);
	while (cpy.codepoint.len && is_ident(cpy.codepoint))
		it_next(&cpy);
	if (cpy.byte_pos == it->byte_pos + 1)
	{
		stringbuf_init(&buf, 8);
		stringbuf_append(&buf, it->codepoint);
		token_list_push(list, (t_token){
				.type = TOK_WORD,
				.start = start,
				.end = start + 1,
				.word = buf,
		});
	}
	else
		token_list_push(list, (t_token){
			.type = TOK_PARAM_SIMPLE,
			.start = start + 1,
			.end = cpy.byte_pos,
			.word = stringbuf_from_range(it->str.str + start + 1,
					it->str.str + cpy.byte_pos)
		});
	*it = cpy;
	return (1);
}

