/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_find.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgamba <linogamba@pundalik.org>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 11:59:40 by lgamba            #+#    #+#             */
/*   Updated: 2025/03/17 11:59:41 by lgamba           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "tokenizer.h"

size_t
	find_unescaped(t_string input, const char *token)
{
	const size_t	tok_len = ft_strlen(token);
	size_t			escape;
	t_u8_iterator	it;

	escape = 0;
	it = it_new(input);
	it_next(&it);
	while (it.codepoint.len)
	{
		if (!str_cmp(it_substr(&it, tok_len), token))
		{
			if (escape % 2 == 1)
				escape = 0;
			else
				return (it.byte_pos);
		}
		else if (it.codepoint.str[0] == '\\')
			++escape;
		else
			escape = 0;
		it_next(&it);
	}
	return ((size_t)-1);
}

size_t
	find_matching(
	t_string input,
	const char *opening,
	const char *closing,
	int escaped)
{
	const size_t	lens[2] = {ft_strlen(opening), ft_strlen(closing)};
	size_t			escape;
	t_u8_iterator	it;
	size_t			balance;

	escape = 0;
	it = it_new(input);
	it_next(&it);
	balance = 1;
	while (it.codepoint.len)
	{
		if (!str_cmp(it_substr(&it, lens[0]), opening))
		{
			if (escape % 2 == 1)
				escape = 0;
			else
				++balance;
		}
		else if (!str_cmp(it_substr(&it, lens[1]), closing))
		{
			if (escape % 2 == 1)
				escape = 0;
			else
			{
				--balance;
				if (!balance)
					return (it.byte_pos);
			}
		}
		else if (escaped && it.codepoint.str[0] == '\\')
			++escape;
		else
			escape = 0;
		it_next(&it);
	}
	return ((size_t)-1);
}
