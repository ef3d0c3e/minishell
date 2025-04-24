/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_util.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgamba <linogamba@pundalik.org>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 11:59:40 by lgamba            #+#    #+#             */
/*   Updated: 2025/03/17 11:59:41 by lgamba           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include <shell/shell.h>

size_t
	parser_next_operator(
		t_parser *parser,
		size_t start,
		size_t end,
		int min_prec)
{
	const t_token	*tok;
	size_t			i;
	size_t			balance[2];

	i = start;
	balance[0] = 0;
	balance[1] = 0;
	// TODO: `{}` balance
	while (i < end)
	{
		tok = &parser->list.tokens[i];
		if (tok->type == TOK_GROUPING && tok->reserved_word[0] == '(')
			++balance[0];
		else if (tok->type == TOK_GROUPING && tok->reserved_word[0] == ')')
			--balance[0];
		if ((tok->type == TOK_PIPELINE || tok->type == TOK_SEQUENCE
			|| tok->type == TOK_NEWLINE) && balance[0] == 0)
		{
			if (token_precedence(tok) == min_prec)
				return (i);
		}
		++i;
	}
	return ((size_t)-1);
}

int
	token_atoi(t_parser *parser, size_t start, int *value)
{
	const t_token	*tok = &parser->list.tokens[start];
	size_t			pos;
	int				sign;

	if (tok->type != TOK_DIGIT)
		return (parser_error(parser, ft_strdup("Expected number"), start, start),
			0);
	pos = 0;
	sign = 1;
	if (tok->word.str[pos] == '+' && ++pos)
		sign = 1;
	else if (tok->word.str[pos] == '-' && ++pos)
		sign = -1;
	*value = 0;
	while (pos < tok->word.len && tok->word.str[pos] >= '0'
		&& tok->word.str[pos] <= '9')
	{
		if (muladd_10s_overflow(*value, sign, tok->word.str[pos] - '0'))
			return (parser_error(parser, ft_strdup("Integer has overflown"),
				start, start), 0);
		*value = *value * 10 + sign * (tok->word.str[pos] - '0');
		++pos;
	}
	return (1);
}
