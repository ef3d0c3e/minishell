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
#include "ft_printf.h"
#include "tokenizer/tokenizer.h"
#include <shell/shell.h>

int
	accept(t_parser *parser, int offset, const char *word)
{
	const t_token	*tok;

	if (offset > 0 && parser->pos + offset >= parser->list.size)
		return (0);
	else if (offset < 0 && (size_t)-offset > parser->pos)
		return (0);
	if (parser->pos + offset >= parser->list.size)
		return (0);
	tok = &parser->list.tokens[parser->pos + offset];
	if (tok->type == TOK_KEYWORD || tok->type == TOK_SEQUENCE
		|| tok->type == TOK_OPERATOR || tok->type == TOK_PIPELINE
		|| tok->type == TOK_GROUPING)
		return (!ft_strcmp(tok->reserved_word, word));
	return (0);
}

int
	accept_word(t_parser *parser, int offset)
{
	const t_token	*tok;

	if (offset > 0 && parser->pos + offset >= parser->list.size)
		return (0);
	else if (offset < 0 && (size_t)-offset > parser->pos)
		return (0);
	if (parser->pos + offset >= parser->list.size)
		return (0);
	tok = &parser->list.tokens[parser->pos + offset];
	if (tok->type == TOK_KEYWORD && !parser->allow_reserved)
		return (0);
	return (token_isword(tok->type));
}

int
	expect(t_parser *parser, int offset, const char *word)
{
	size_t	start;
	char	*err;

	if (accept(parser, offset, word))
		return (1);
	ft_asprintf(&err, "Expected `%s` token", word);
	start = parser->pos;
	if (offset >= 0)
		start = min_sz(start + offset, parser->list.size - 1);
	else if ((size_t)-offset > start)
		start = 0;
	else
		start = min_sz(start + offset, parser->list.size - 1);
	parser_error(parser, err, start, parser->list.size);
	return (0);
}

int
	accept_tok(t_parser *parser, int offset, enum e_token_type type)
{
	if (offset > 0 && parser->pos + offset >= parser->list.size)
		return (0);
	else if (offset < 0 && (size_t)-offset > parser->pos)
		return (0);
	if (parser->pos + offset >= parser->list.size)
		return (0);
	return (parser->list.tokens[parser->pos + offset].type == type);
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

void
	print_pad(const char *pad, size_t n)
{
	size_t	i;

	i = 0;
	while (i++ < n)
		ft_dprintf(2, "%s", pad);
}
