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
#include "util/util.h"
#include <stddef.h>
#include <stdlib.h>

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

int
	token_isword(enum e_token_type type)
{
	// TODO
	return (type == TOK_WORD
		|| type == TOK_SINGLE_QUOTE
		|| type == TOK_DOUBLE_QUOTE
		|| type == TOK_DIGIT
		|| type == TOK_KEYWORD);
}

size_t
	find_matching(t_string input, const char *opening, const char *closing)
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
		else if (it.codepoint.str[0] == '\\')
			++escape;
		else
			escape = 0;
		it_next(&it);
	}
	return ((size_t)-1);
}

int
	token_wordcontent(t_string_buffer *buf, const t_token *tok)
{
	// TODO
	if (tok->type == TOK_WORD
		|| tok->type == TOK_SINGLE_QUOTE
		|| tok->type == TOK_DOUBLE_QUOTE)
	{
		stringbuf_append(buf, (t_string){.str = tok->word.str,
				.len = tok->word.len });
		return (1);
	}
	else if (tok->type == TOK_DIGIT)
	{
		// TODO
	}
	else if (tok->type == TOK_KEYWORD)
	{
		stringbuf_append(buf, (t_string){.str = tok->reserved_word,
				.len = ft_strlen(tok->reserved_word) });
		return (1);
	}
	return 0;
}

int
	token_precedence(const t_token *tok)
{
	if (tok->type == TOK_PIPELINE)
		return (0);
	else if (tok->type == TOK_SEQUENCE)
	{
		if (!ft_strcmp("&&", tok->reserved_word)
			|| !ft_strcmp("||", tok->reserved_word))
			return (2);
		return (1);
	}
	else if (tok->type == TOK_CMD_SUB || tok->type == TOK_ARITH)
		return (3);
	return (-1);
}
