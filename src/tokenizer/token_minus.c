/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_minus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgamba <linogamba@pundalik.org>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 11:59:40 by lgamba            #+#    #+#             */
/*   Updated: 2025/03/17 11:59:41 by lgamba           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include <tokenizer/tokenizer.h>

int
	token_minus(t_token_list *list, t_u8_iterator *it)
{
	if (it->codepoint.str[0] != '-')
		return (0);
	token_list_push(list, (t_token){
		.type = TOK_MINUS,
		.start = it->byte_pos,
		.end = it->byte_pos + 1,
		.reserved_word = "-"
	});
	it_next(it);
	return (1);
}
