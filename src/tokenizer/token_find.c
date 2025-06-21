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
#include <tokenizer/tokenizer.h>

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

static int
	find_matching_loop(
	int match_open,
	int match_close,
	size_t *escape,
	size_t *balance)
{
	if (match_open)
	{
		if (*escape % 2 == 1)
			*escape = 0;
		else
			++*balance;
		return (1);
	}
	else if (match_close)
	{
		if (*escape % 2 == 1)
			*escape = 0;
		else
		{
			--*balance;
			if (!*balance)
				return (-1);
		}
		return (1);
	}
	return (0);
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
	size_t			balance;
	t_u8_iterator	it;
	int				status;

	escape = 0;
	it = it_new(input);
	it_next(&it);
	balance = 1;
	while (it.codepoint.len)
	{
		status = find_matching_loop(!str_cmp(it_substr(&it, lens[0]), opening),
				!str_cmp(it_substr(&it, lens[1]), closing), &escape, &balance);
		if (status == -1)
			return (it.byte_pos);
		if (status == 0 && escaped && it.codepoint.str[0] == '\\')
			++escape;
		else if (status == 0)
			escape = 0;
		it_next(&it);
	}
	return ((size_t)-1);
}
