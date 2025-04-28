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
#include <shell/shell.h>

static void
	if_push(t_parser *parser, t_ast_node *stmt, int with_cond)
{
	t_ast_node	*cond;
	t_ast_node	*body;

	if (with_cond)
	{
		cond = parse_cmdlist(parser);
		expect(parser, 0, "then");
		++parser->pos;
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
	int			in_stmt = !parser->allow_reserved;
	t_ast_node	*stmt;

	++parser->pos;
	parser->allow_reserved = 0;
	stmt = make_if_node();
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
	expect(parser, 0, "fi");
	++parser->pos;
	parser->allow_reserved = !in_stmt;
	return (stmt);
}
