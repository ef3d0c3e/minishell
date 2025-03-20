/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_double_quote.c                               :+:      :+:    :+:   */
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
	token_double_quote(t_token_list *list, t_u8_iterator *it)
{
	size_t	end;

	if (it->codepoint.str[0] != '"')
		return (0);
	it_advance(it, 1);
	end = find_unsecaped(it_substr(it, -1), "\"");
	if (end == (size_t)-1)
	{
		token_error(list, it->byte_pos-1, it->byte_pos, "Unterminated `\"` token");
		return (1);
	}
	token_list_push(list, (t_token){
		.type = TOK_DOUBLE_QUOTE,
		.start = it->byte_pos,
		.end = it->byte_pos + end,
	});
	it_advance(it, end + 1);
	return (1);
}
