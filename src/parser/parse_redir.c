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
	const t_token	*tok = &parser->list.tokens[start];

	if (end - start < 2)
		return (0);
	if (tok->type == TOK_REDIR)
		return (redir_parser2(parser, start, redirs));
	else if (tok->type == TOK_DIGIT
		&& parser->list.tokens[start + 1].type == TOK_REDIR
		&& end - start >= 3)
		return (redir_parser3(parser, start, redirs));
	// TODO: If redir is of the heredoc type, push to the heredoc stack
	// in order to read input after successful command parsing
	return (0);
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
			break;
		result = parse_redir(parser, start + skipped + spaces, end, redirs);
		skipped += result;
		if (!result || start + skipped >= end)
			break;
		skipped += spaces;

	}
	return (skipped);
}
