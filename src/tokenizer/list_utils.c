/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   list_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgamba <linogamba@pundalik.org>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 11:59:40 by lgamba            #+#    #+#             */
/*   Updated: 2025/03/17 11:59:41 by lgamba           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include <tokenizer/tokenizer.h>

void
	list_push_codepoint(t_token_list *list, const t_u8_iterator *it)
{
	if (list->size && list->tokens[list->size - 1].type == TOK_SINGLE_QUOTE)
	{
		list->tokens[list->size - 1].end += it->codepoint.len;
		stringbuf_append(&list->tokens[list->size - 1].word, it->codepoint);
	}
	else
	{
		token_list_push(list, (t_token){
			.type = TOK_SINGLE_QUOTE,
			.start = it->byte_pos,
			.end = it->byte_pos + it->codepoint.len,
			.word = stringbuf_from_range(it->codepoint.str,
				it->codepoint.str + it->codepoint.len)
		});
	}
}

void
	list_extend(t_token_list *result, t_token_list *from, size_t offset)
{
	size_t	i;

	i = 0;
	while (i < from->size)
	{
		from->tokens[i].start += offset;
		from->tokens[i].end += offset;
		token_list_push(result, from->tokens[i]);
		++i;
	}
	free(from->tokens);
}
