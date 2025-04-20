/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgamba <linogamba@pundalik.org>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 11:59:40 by lgamba            #+#    #+#             */
/*   Updated: 2025/03/17 11:59:41 by lgamba           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include <parser/parser.h>

/** @brief Parse an expression delimited by two operators */
static t_ast_node
	*parse_expression(t_parser *parser, size_t start, size_t end)
{
	t_token	*tok;

	while (parser->list.tokens[start].type == TOK_SPACE)
		++start;
	while (parser->list.tokens[end - 1].type == TOK_SPACE)
		--end;
	tok = &parser->list.tokens[start];

	if (tok->type == TOK_GROUPING && tok->reserved_word[0] == '(')
		return (parse_subshell(parser, start, end));
	else
		return (parse_cmd(parser, start, end));
}


t_ast_node
	*parse(t_parser *parser, size_t start, size_t end)
{
	t_ast_node		*node;
	size_t			i;
	size_t			next;

	node = NULL;
	i = start;
	while (i < end)
	{
		next = parser_next_operator(parser, i, end);
		if (next == (size_t)-1)
		{
			node = parse_expression(parser, i, end);
			i = end;
		}
		else if (start == next)
		{
			parser_error(parser, ft_strdup("Expected tokens"), start, next);
			return (node);
		}
		else
		{
			node = xmalloc(sizeof(t_ast_node));
			node->type = NODE_LOGIC;
			node->logic.token = parser->list.tokens[next];
			node->logic.left = parse_expression(parser, start, next);
			node->logic.right = parse(parser, next + 1, end);
			i = end;
		}
	}
	return (node);
}
