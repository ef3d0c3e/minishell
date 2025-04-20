
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_sub.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgamba <linogamba@pundalik.org>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 11:59:40 by lgamba            #+#    #+#             */
/*   Updated: 2025/03/17 11:59:41 by lgamba           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "parser.h"

/* Finds closing ')' */
static inline size_t
	find_delim(const t_token_list  *list, size_t start, size_t end)
{
	size_t			balance;
	const t_token	*tok;

	balance = 1;
	while (start < end)
	{
		tok = &list->tokens[start++];
		if (tok->type != TOK_GROUPING)
			continue ;
		if (tok->reserved_word[0] == ')')
			--balance;
		else if (tok->reserved_word[0] == '(')
			++balance;
		if (!balance)
			return (start - 1);
	}
	return ((size_t)-1);
}

struct s_node_expr
	parse_delimited(t_parser *parser, size_t start, size_t end)
{
	const t_token	*const tok_start = &parser->list.tokens[start];
	const t_token	*const tok_end = &parser->list.tokens[end];
	const t_string input = {.str = parser->input.str + tok_start->end,
		.len = tok_end->start - tok_start->end
	};

	printf("CONTENT=`%.*s` %zu %zu\n", input.len, input.str, start, end);
	return ((struct s_node_expr){
		.input = input,
		.head = parse(parser, start + 1, end),
		.redirs = {
			.redirs = NULL,
			.redirs_capacity = 0,
			.redirs_size = 0,
		}
	});
}

t_ast_node
	*parse_subshell(t_parser *parser, size_t start, size_t end)
{
	t_ast_node		*node;
	const size_t	delim = find_delim(&parser->list, start + 1, end);
	size_t			redirlen;

	if (delim == (size_t)-1)
	{
		parser_error(parser, ft_strdup("Unterminated `(` delimiter"), start, end);
		return (NULL);
	}
	node = xmalloc(sizeof(t_ast_node));
	node->type = NODE_SUBSHELL;
	node->expr = parse_delimited(parser, start, delim);
	redirlen = 0;
	if (delim != (size_t)-1)
		redirlen = parse_redir_repeat(parser, delim + 1, end, &node->expr.redirs);
	if (delim + redirlen + 1 != end)
		parser_error(parser, ft_strdup("Leftover tokens after `)`"), delim + redirlen + 1, end);
	return (node);
}
