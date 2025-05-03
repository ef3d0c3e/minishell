/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_loop.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgamba <linogamba@pundalik.org>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 11:59:40 by lgamba            #+#    #+#             */
/*   Updated: 2025/03/17 11:59:41 by lgamba           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "ft_printf.h"
#include "parser/parser.h"
#include <shell/shell.h>

static void
	parser_syntax_error(t_parser *parser, const char *expected)
{
	char	*err;

	if (expected)
		ft_asprintf(&err, "Unexpected token");
	else
		ft_asprintf(&err, "Expected `%s` token", expected);
	parser_error(parser, err, parser->pos, parser->pos + 1);
}

t_ast_node
	*parse_while(t_parser *parser)
{
	t_ast_node	*cond;
	t_ast_node	*body;

	++parser->pos;
	parser_delimiter_push(parser, "do");
	cond = parse_cmdlist(parser);
	expects_delimiter(parser, "do");

	parser_delimiter_push(parser, "done");
	body = parse_cmdlist(parser);
	expects_delimiter(parser, "done");
	return (make_loop_node(cond, body, 0));
}

t_ast_node
	*parse_until(t_parser *parser)
{
	t_ast_node	*stmt;

	stmt = parse_while(parser);
	stmt->st_loop.until = 1;
	return (stmt);
}
