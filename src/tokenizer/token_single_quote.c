/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_single_quote.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgamba <linogamba@pundalik.org>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 11:59:40 by lgamba            #+#    #+#             */
/*   Updated: 2025/06/18 08:39:18 by thschnei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "tokenizer.h"

int
	token_single_quote(t_token_list *list, t_u8_iterator *it)
{
	t_string_buffer	buf;
	const size_t	start = it->byte_pos;

	if (it->codepoint.str[0] != '\'')
		return (0);
	it_advance(it, 1);
	while (it->codepoint.len)
	{
		if (it->codepoint.str[0] == '\'')
		{
			stringbuf_init(&buf, it->byte_pos - start);
			stringbuf_append(&buf, (t_string){.str = it->str.str + start + 1,
				.len = it->byte_pos - start - 1});
			token_list_push(list,
				TOK_SINGLE_QUOTE, start + 1, it->byte_pos)->word = buf;
			list->tokens[list->size - 1].flags |= FL_SQUOTED;
			it_advance(it, 1);
			return (1);
		}
		it_next(it);
	}
	token_error(list, start, it->byte_pos, "Unterminated `'` token");
	return (1);
}
