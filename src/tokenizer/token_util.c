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
		stringbuf_itoa(buf, tok->digit);
		return (1);
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
