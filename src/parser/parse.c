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
#include <shell/shell.h>

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


/*
t_ast_node
	*parse(t_parser *parser, size_t start, size_t end, int min_prec)
{
	t_ast_node		*node;
	size_t			next;

	if (start == end)
		return (NULL);
	next = parser_next_operator(parser, start, end, min_prec);
	if (next == (size_t)-1)
	{
		if (min_prec == 3)
		{
			if (start < end)
				return (parse_expression(parser, start, end));
			parser_error(parser, ft_strdup("Expected tokens"), start - !!start, end);
		}
		else
			return (parse(parser, start, end, min_prec + 1));
	}
	else if (start == next)
		parser_error(parser, ft_strdup("Expected tokens"), start - !!start, next);
	else
	{
		node = xmalloc(sizeof(t_ast_node));
		node->type = NODE_LOGIC;
		node->logic.token = parser->list.tokens[next];
		node->logic.left = parse(parser, start, next, min_prec + 1);
		node->logic.right = NULL;
		if (next + 1 < end || node->logic.token.reserved_word[0] != ';')
			node->logic.right = parse(parser, next + 1, end, min_prec);
		return (node);
	}
	return (NULL);
}
*/
