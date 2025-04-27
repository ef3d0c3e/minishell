/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_meta.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgamba <linogamba@pundalik.org>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 11:59:40 by lgamba            #+#    #+#             */
/*   Updated: 2025/03/17 11:59:41 by lgamba           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include <tokenizer/tokenizer.h>

// https://www.gnu.org/software/bash/manual/bash.html#index-metacharacter

int
	token_space(t_token_list *list, t_u8_iterator *it)
{
	if (it->codepoint.str[0] != ' ' && it->codepoint.str[0] != '\t')
		return (0);
	if (!list->size || list->tokens[list->size - 1].type != TOK_SPACE)
	{
		token_list_push(list, TOK_SPACE, it->byte_pos,
			it->byte_pos + it->codepoint.len);
	}
	else
		list->tokens[list->size - 1].end += it->codepoint.len;
	it_advance(it, it->codepoint.len);
	return (1);
}
