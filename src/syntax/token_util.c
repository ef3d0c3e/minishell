/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_util.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgamba <linogamba@pundalik.org>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 11:59:40 by lgamba            #+#    #+#             */
/*   Updated: 2025/03/17 11:59:41 by lgamba           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "tokenizer.h"

size_t
	find_unsecaped(t_string input, const char *token)
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
