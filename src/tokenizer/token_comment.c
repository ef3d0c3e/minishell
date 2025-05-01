/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_comment.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgamba <linogamba@pundalik.org>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 11:59:40 by lgamba            #+#    #+#             */
/*   Updated: 2025/03/17 11:59:41 by lgamba           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include <tokenizer/tokenizer.h>

int
	token_comment(t_token_list *list, t_u8_iterator *it)
{
	const size_t	start = it->byte_pos;

	if (list->size && token_isword(list->tokens[list->size - 1].type))
		return (0);
	if (it->codepoint.str[0] != '#')
		return (0);
	while (it->codepoint.len && it->codepoint.str[0] != '\n')
		it_next(it);
	token_list_push(list, TOK_COMMENT, start,
		it->byte_pos);
	return (1);
}
