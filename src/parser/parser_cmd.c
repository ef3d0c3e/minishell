/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_cmd.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgamba <linogamba@pundalik.org>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 11:59:40 by lgamba            #+#    #+#             */
/*   Updated: 2025/03/17 11:59:41 by lgamba           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "parser.h"

t_string_buffer	cmd_word(t_parser *parser, size_t *start, size_t end)
{
	t_string_buffer	buf;

	buf.str = NULL;
	buf.capacity = 0;
	buf.len = 0;
	while (*start != end)
	{
		if (!token_wordcontent(&buf, &parser->list.tokens[*start]))
			break ;
		++(*start);
	}
	return (buf);
}

struct s_node_expr
parse_subexpr(t_parser *parser, t_string expr)
{
	t_string		prev_input;
	t_token_list	prev_list;
	t_token_list	list;
	t_ast_node		*head;

	list = tokenizer_tokenize(expr);
	// FIXME: Check for error tokens

	prev_input = parser->input;
	prev_list = parser->list;
	parser->input = expr;
	parser->list = list;
	head = parse(parser, 0, list.size);
	token_list_free(&list);
	parser->list = prev_list;
	parser->input = prev_input;
	return ((struct s_node_expr){
		.input = expr,
		.head = head,
	});
}

t_ast_node
	*parse_cmd(t_parser *parser, size_t start, size_t end)
{
	const t_token		*tok;
	struct s_node_cmd	cmd;
	t_ast_node			*node;
	t_string_buffer		buf;

	cmd.redirs.redirs = NULL;
	cmd.redirs.redirs_size = 0;
	cmd.redirs.redirs_capacity = 0;
	cmd.nargs = 0;
	if (end - start)
		cmd.args = xmalloc(sizeof(t_ast_node) * (end - start));
	node = xmalloc(sizeof(t_ast_node));
	node->type = NODE_COMMAND;
	node->cmd = cmd;
	while (start != end)
	{
		start += parse_redir_repeat(parser, start, end, &node->cmd.redirs);
		if (start >= end)
			break;
		buf = cmd_word(parser, &start, end);
		if (buf.len)
		{
			node->cmd.args[node->cmd.nargs].type = NODE_ATOM;
			node->cmd.args[node->cmd.nargs++].atom = buf;
			buf = (t_string_buffer){.str = NULL, .len = 0, .capacity = 0};
			continue;
		}
		tok = &parser->list.tokens[start];
		if (tok->type == TOK_CMD_SUB)
		{
			node->cmd.args[node->cmd.nargs].type = NODE_SUBEXPR;
			node->cmd.args[node->cmd.nargs++].expr = parse_subexpr(parser,
					(t_string){
					.str = parser->input.str + tok->start,
					.len = tok->end - tok->start});
		}
		else if (tok->type != TOK_SPACE)
		{
			parser_error(parser, ft_strdup("Unhandled token type during "
						"command parsing"), start, end);
		}
		++start;
	}
	return (node);
}
