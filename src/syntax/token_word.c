/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_word.c                               :+:      :+:    :+:   */
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
	token_word(t_token_list *list, t_u8_iterator *it)
{
	t_string_buffer	buf;

	if (!list->size || list->tokens[list->size - 1].type != TOK_WORD)
	{
		stringbuf_init(&buf, 8);
		stringbuf_append(&buf, it->codepoint);
		token_list_push(list, (t_token){
			.type = TOK_WORD,
			.start = it->byte_pos,
			.end = it->byte_pos + it->codepoint.len,
			.word = buf
		});
	}
	else
	{
		stringbuf_append(&list->tokens[list->size - 1].word, it->codepoint);
		list->tokens[list->size - 1].end += it->codepoint.len;
	}
	it_advance(it, it->codepoint.len);
	return (1);
}
