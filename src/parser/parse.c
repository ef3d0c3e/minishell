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

	if (start == end)
		return (NULL);
	tok = &parser->list.tokens[start];
	if (tok->type == TOK_GROUPING && tok->reserved_word[0] == '(')
		return (parse_subshell(parser, start, end));
	else
		return (parse_cmd(parser, start, end));
}


t_ast_node
	*parse(t_parser *parser, size_t start, size_t end, int min_prec)
{
	t_ast_node		*node;
	size_t			i;
	size_t			next;

	node = NULL;
	i = start;
	if (i >= end)
	{
		parser_error(parser, ft_strdup("Expected tokens"), start - 1, end);
		return (NULL);
	}
	next = parser_next_operator(parser, i, end, min_prec);
	if (next == (size_t)-1)
	{
		if (min_prec == 3)
		{
			node = parse_expression(parser, i, end);
		}
		else
			node = parse(parser, start, end, min_prec + 1);
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
		node->logic.left = parse(parser, start, next, min_prec + 1);
		node->logic.right = parse(parser, next + 1, end, min_prec);
	}
	return (node);
}
