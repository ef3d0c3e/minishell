/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_while.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgamba <linogamba@pundalik.org>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 11:59:40 by lgamba            #+#    #+#             */
/*   Updated: 2025/03/17 11:59:41 by lgamba           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include <shell/shell.h>

t_ast_node
	*parse_while(t_parser *parser)
{
	int			in_stmt = !parser->allow_reserved;
	t_ast_node	*cond;
	t_ast_node	*body;

	++parser->pos;
	parser->allow_reserved = 0;
	cond = parse_cmdlist(parser);
	expect(parser, 0, "do");
	++parser->pos;
	body = parse_cmdlist(parser);
	expect(parser, 0, "done");
	++parser->pos;
	parser->allow_reserved = !in_stmt;
	return (make_while_node(cond, body));
}
