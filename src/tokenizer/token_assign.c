/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_assign.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgamba <linogamba@pundalik.org>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 11:59:40 by lgamba            #+#    #+#             */
/*   Updated: 2025/03/17 11:59:41 by lgamba           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "util/util.h"
#include <tokenizer/tokenizer.h>

int
	token_assign(t_token_list *list, t_u8_iterator *it)
{
	t_u8_iterator	cpy;

	if (list->size
			&& list->tokens[list->size - 1].type != TOK_SPACE
			&& list->tokens[list->size - 1].type != TOK_SEQUENCE)
		return (0);
	if (it->codepoint.len != 1 || !is_param_ident_start(it->codepoint.str[0]))
		return (0);
	cpy = *it;
	while (cpy.codepoint.len == 1 && cpy.codepoint.str[0] != '=')
	{
		if (!is_param_ident(cpy.codepoint.str[0]))
			return (0);
		it_next(&cpy);
	}
	if (cpy.codepoint.len != 1 || cpy.codepoint.str[0] != '=')
		return (0);
	it_next(&cpy);
	token_list_push(list, TOK_ASSIGN, it->byte_pos, cpy.byte_pos)->word
		= stringbuf_from_range(it->str.str + it->byte_pos,
			it->str.str + cpy.byte_pos - 1);
	*it = cpy;
	return (1);
}
