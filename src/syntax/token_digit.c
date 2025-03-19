/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_digit.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgamba <linogamba@pundalik.org>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 11:59:40 by lgamba            #+#    #+#             */
/*   Updated: 2025/03/17 11:59:41 by lgamba           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "tokenizer.h"

int
	token_digit(t_token_list *list, t_u8_iterator *it)
{
	const size_t	start = it->byte_pos;
	int				p;
	int				digit;

	if (it->codepoint.str[0] < '0' || it->codepoint.str[0] > '9')
		return (0);
	digit = 0;
	p = 0;
	while (it->codepoint.str[0] >= '0' && it->codepoint.str[0] <= '9')
	{
		digit = digit * 10 + (it->codepoint.str[0] - '0');
		if (digit < p)
			p = -1;
		if (p >= 0)
			p = digit;
		it_next(it);
	}
	if (p < 0)
		token_error(list, start, it->byte_pos, "Number > 2**31 - 1");
	else
		token_list_push(list, (t_token){
			.type = TOK_DIGIT,
			.start = start,
			.end = it->byte_pos,
			.digit = {digit},
		});
	return (1);
}
