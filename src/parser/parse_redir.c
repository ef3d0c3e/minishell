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
#include <shell/shell.h>

int
	parse_redir(
	t_parser *parser,
	t_redirections *redirs)
{
	const size_t	end = parser->list.size;
	const t_token	*list = parser->list.tokens + parser->pos;
	size_t			skip;

	skip = 0;
	if (end - parser->pos >= 3 && list[0].type == TOK_REDIR
		&& token_isword(list[1].type) && list[2].type == TOK_MINUS)
		skip = redir_parser3_move(parser, parser->pos, redirs);
	if (!skip && end - parser->pos >= 2 && list[0].type == TOK_REDIR
		&& token_isword(list[1].type))
		skip = redir_parser2(parser, parser->pos, redirs);
	if (!skip && end - parser->pos >= 4 && list[0].type == TOK_DIGIT
		&& list[1].type == TOK_REDIR && token_isword(list[2].type) 
		&& list[3].type == TOK_MINUS)
		skip = redir_parser4(parser, parser->pos, redirs);
	if (!skip && end - parser->pos >= 3 && list[0].type == TOK_DIGIT
		&& list[1].type == TOK_REDIR && token_isword(list[2].type))
		skip = redir_parser3(parser, parser->pos, redirs);
	if (list[0].type == TOK_REDIR && !skip)
	{
		parser_error(parser, ft_strdup("Invalid redirections"),
			parser->pos, parser->pos + 1);
		skip = 1;
	}
	return (parser->pos += skip, skip != 0);
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
