/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_arith.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgamba <linogamba@pundalik.org>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 11:59:40 by lgamba            #+#    #+#             */
/*   Updated: 2025/03/17 11:59:41 by lgamba           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "tokenizer.h"

int
	token_arith(t_token_list *list, t_u8_iterator *it)
{
	size_t	end;

	if (str_cmp(it_substr(it, 3), "$(("))
		return (0);
	it_advance(it, 3);
	end = find_matching(it_substr(it, -1), "((", "))", 1);
	if (end == (size_t)-1)
	{
		token_error(list, it->byte_pos - 3, it->byte_pos, "Unterminated `$((` token");
		return (1);
	}
	token_list_push(list, (t_token){
		.type = TOK_ARITH,
		.start = it->byte_pos,
		.end = it->byte_pos + end,
	});
	it_advance(it, end + 2);
	return (1);
}
