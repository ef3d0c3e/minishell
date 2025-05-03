/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_if.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgamba <linogamba@pundalik.org>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 11:59:40 by lgamba            #+#    #+#             */
/*   Updated: 2025/03/17 11:59:41 by lgamba           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "parser/parser.h"
#include <shell/shell.h>
#include <stdio.h>

static void
	if_push(t_parser *parser, t_ast_node *stmt, int with_cond)
{
	t_ast_node	*cond;
	t_ast_node	*body;

	if (with_cond)
	{
		parser_delimiter_push(parser, "then");
		cond = parse_cmdlist(parser);
		expects_delimiter(parser, "then");
	}
	body = parse_cmdlist(parser);

	if (with_cond)
	{
		stmt->st_if.conds = ft_realloc(stmt->st_if.conds,
				sizeof(t_ast_node **) * stmt->st_if.nconds,
				sizeof(t_ast_node **) * (stmt->st_if.nconds + 1));
		stmt->st_if.conds[stmt->st_if.nconds++] = cond;
	}
	stmt->st_if.bodies = ft_realloc(stmt->st_if.bodies,
		sizeof(t_ast_node **) * stmt->st_if.nbodies,
		sizeof(t_ast_node **) * (stmt->st_if.nbodies + 1));
	stmt->st_if.bodies[stmt->st_if.nbodies++] = body;
}

t_ast_node
	*parse_if(t_parser *parser)
{
	t_ast_node	*stmt;

	++parser->pos;
	stmt = make_if_node();
	parser_delimiter_push(parser, "fi");
	parser_delimiter_push(parser, "elif");
	parser_delimiter_push(parser, "else");
	if_push(parser, stmt, 1);
	while (accept(parser, 0, "elif"))
	{
		++parser->pos;
		if_push(parser, stmt, 1);
	}
	if (accept(parser, 0, "else"))
	{
		++parser->pos;
		if_push(parser, stmt, 0);
	}
	parser_delimiter_pop(parser, "else");
	parser_delimiter_pop(parser, "elif");
	expects_delimiter(parser, "fi");
	return (stmt);
}
