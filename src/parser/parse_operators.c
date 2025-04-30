/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_operators.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgamba <linogamba@pundalik.org>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 11:59:40 by lgamba            #+#    #+#             */
/*   Updated: 2025/03/17 11:59:41 by lgamba           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "parser/parser.h"
#include <shell/shell.h>

t_ast_node
	*parse_pipeline(t_parser *parser)
{
	t_ast_node	*left;
	t_ast_node	*right;
	t_token		op;

	left = parse_command(parser);
	if (!left)
	{
		parser_error(parser, ft_strdup("Expected token before operator"),
				0, parser->pos);
		return (NULL);
	}
	while (accept(parser, 0, "|") || accept(parser, 0, "|&"))
	{
		op = parser->list.tokens[parser->pos];
		++parser->pos;
		right = parse_command(parser);
		left = make_logic_node(op, left, right);
	}
	return (left);
}

t_ast_node
	*parse_and_or(t_parser *parser)
{
	t_ast_node	*left;
	t_ast_node	*right;
	t_token		op;

	left = parse_pipeline(parser);
	if (!left)
		return (NULL);
	while (accept(parser, 0, "&&") || accept(parser, 0, "||"))
	{
		op = parser->list.tokens[parser->pos];
		++parser->pos;
		right = parse_command(parser);
		left = make_logic_node(op, left, right);
	}
	return (left);
}
