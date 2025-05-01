/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_func.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgamba <linogamba@pundalik.org>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 11:59:40 by lgamba            #+#    #+#             */
/*   Updated: 2025/03/17 11:59:41 by lgamba           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include <shell/shell.h>

t_ast_node
	*parse_function_def(t_parser *parser)
{
	const int		in_stmt = parser->allow_reserved;
	size_t			start;
	t_string_buffer	name;
	t_ast_node		*body;

	stringbuf_init(&name, 64);
	token_wordcontent(&name, &parser->list.tokens[parser->pos]);
	parser->pos += 3;
	while (accept(parser, 0, "\n"))
		++parser->pos;
	start = parser->pos;
	parser->allow_reserved = 0;
	body = parse_compound_command(parser);
	if (!body  || (body->type != NODE_BLOCK && body->type != NODE_SUBSHELL))
		parser_error(parser, ft_strdup("Expected a block or subshell"),
		start, parser->pos);
	parser->allow_reserved = in_stmt;
	return (make_function_node(name, body));
}
