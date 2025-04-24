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
#include <parser/parser.h>

size_t	parse_redir(
	t_parser *parser,
	size_t start,
	size_t end,
	t_redirections *redirs)
{
	const t_token	*list = parser->list.tokens + start;
	int				status;

	status = 0;
	if (end - start >= 3 && list[0].type == TOK_REDIR
		&& token_isword(list[1].type) && list[2].type == TOK_MINUS)
		status = redir_parser3_move(parser, start, redirs);
	if (!status && end - start >= 2 && list[0].type == TOK_REDIR
		&& token_isword(list[1].type))
		status = redir_parser2(parser, start, redirs);
	if (!status && end - start >= 4 && list[0].type == TOK_DIGIT && list[1].type
		== TOK_REDIR && token_isword(list[2].type) && list[3].type == TOK_MINUS)
		status = redir_parser4(parser, start, redirs);
	if (!status && end - start >= 3 && list[0].type == TOK_DIGIT && list[1].type
		== TOK_REDIR && token_isword(list[2].type))
		status = redir_parser3(parser, start, redirs);
	if (list[0].type == TOK_REDIR && !status)
	{
		parser_error(parser, ft_strdup("Invalid redirections"),
			start, start + 1);
		status = 1;
	}
	return (status);
}

size_t	parse_redir_repeat(
	t_parser *parser,
	size_t start,
	size_t end,
	t_redirections *redirs)
{
	size_t	skipped;
	size_t	result;
	size_t	spaces;

	skipped = 0;
	while (1)
	{
		spaces = 0;
		while (start + skipped + spaces < end
			&& parser->list.tokens[start + skipped + spaces].type == TOK_SPACE)
			++spaces;
		if (start + skipped + spaces >= end)
			break ;
		result = parse_redir(parser, start + skipped + spaces, end, redirs);
		skipped += result;
		if (!result || start + skipped >= end)
			break ;
		skipped += spaces;
	}
	return (skipped);
}
