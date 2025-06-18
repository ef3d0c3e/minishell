/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_digit.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgamba <linogamba@pundalik.org>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 11:59:40 by lgamba            #+#    #+#             */
/*   Updated: 2025/06/18 08:51:09 by thschnei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include <tokenizer/tokenizer.h>

int
	token_digit(t_token_list *list, t_u8_iterator *it)
{
	const size_t	start = it->byte_pos;
	t_string		peek;
	t_string_buffer	digit;

	if (it->codepoint.str[0] == '+' || it->codepoint.str[0] == '-')
	{
		peek = it_substr(it, 2);
		if (peek.len < 2 || (peek.str[1] < '0' || peek.str[1] > '9'))
			return (0);
		stringbuf_init(&digit, 16);
		stringbuf_append(&digit, it->codepoint);
		it_advance(it, 1);
	}
	else if (it->codepoint.str[0] < '0' || it->codepoint.str[0] > '9')
		return (0);
	else
		stringbuf_init(&digit, 16);
	while (it->codepoint.len && it->codepoint.str[0] >= '0'
		&& it->codepoint.str[0] <= '9')
	{
		stringbuf_append(&digit, it->codepoint);
		it_next(it);
	}
	token_list_push(list, TOK_DIGIT, start, it->byte_pos)->word = digit;
	return (1);
}
