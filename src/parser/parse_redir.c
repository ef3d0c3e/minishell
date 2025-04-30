/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_redir.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgamba <linogamba@pundalik.org>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 11:59:40 by lgamba            #+#    #+#             */
/*   Updated: 2025/03/17 11:59:41 by lgamba           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "parser/redir_parser.h"
#include "tokenizer/tokenizer.h"
#include <shell/shell.h>

int
	redir_has_minus(t_parser *parser, int offset)
{
	size_t	i;

	i = parser->pos + offset;
	while (i < parser->list.size && (token_isword(parser->list.tokens[i].type)
		|| parser->list.tokens[i].type == TOK_CMD_SUB
		|| parser->list.tokens[i].type == TOK_PARAM_SIMPLE
		|| parser->list.tokens[i].type == TOK_PARAM))
		++i;
	if (i == parser->list.size)
		return (0);
	return (parser->list.tokens[i].type == TOK_MINUS);
}

int
	parse_redir(
	t_parser *parser,
	t_redirections *redirs)
{
	const size_t	left = parser->list.size - parser->pos;
	const t_token	*list = parser->list.tokens + parser->pos;
	int				status;

	status = 0;
	if (left >= 2 && list[0].type == TOK_DIGIT && list[1].type == TOK_REDIR)
	{
		if (left >= 4 && redir_has_minus(parser, 2))
			status = redir_parser4(parser, redirs);
		if (left >= 3 && !status)
			status = redir_parser3(parser, redirs);
		if (!status)
		{
			parser_error(parser, ft_strdup("Invalid redirections"),
					parser->pos, parser->pos + 1);
			parser->pos += 2;
		}
	}
	else if (list[0].type == TOK_REDIR)
	{
		if (left >= 3 && redir_has_minus(parser, 1))
			status = redir_parser3_move(parser, redirs);
		if (left >= 2 && !status)
			status = redir_parser2(parser, redirs);
		if (!status)
		{
			parser_error(parser, ft_strdup("Invalid redirections"),
					parser->pos, parser->pos + 1);
			parser->pos += 1;
		}
	}
	return (status != 0);
}

void
	parse_redir_repeat(
	t_parser *parser,
	t_redirections *redirs)
{
	int	result;
	size_t	spaces;

	parser->pos = parser->pos;
	while (1)
	{
		spaces = 0;
		while (parser->pos + spaces < parser->list.size
			&& parser->list.tokens[parser->pos + spaces].type == TOK_SPACE)
			++spaces;
		if (parser->pos + spaces >= parser->list.size)
			break ;
		parser->pos += spaces;
		result = parse_redir(parser, redirs);
		if (!result)
			parser->pos -= spaces;
		if (!result || parser->pos >= parser->list.size)
			break ;
	}
}
