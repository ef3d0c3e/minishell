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
#include <parser/parser.h>

void
	compound_push(t_ast_node *node, size_t arg_pos, const t_token *token)
{
	struct s_node_compound	*comp;

	if (node->cmd.nargs <= arg_pos)
	{
		node->cmd.nargs = arg_pos + 1;
		node->cmd.args[arg_pos].items = NULL;
		node->cmd.args[arg_pos].nitems = 0;
	}
	comp = &node->cmd.args[arg_pos];
	comp->items = ft_realloc(comp->items, sizeof(comp->items[0]) * comp->nitems,
		sizeof(comp->items[0]) * (comp->nitems + 1));
	if (token->type == TOK_PARAM || token->type == TOK_PARAM_SIMPLE)
	{
		comp->items[comp->nitems] = (t_ast_node){
			.type = NODE_PARAMETER,
			.atom = stringbuf_from_range(token->word.str, token->word.str + token->word.len),
		};
	}
	else
	{
		comp->items[comp->nitems] = (t_ast_node){
			.type = NODE_ATOM,
		};
		stringbuf_init(&comp->items[comp->nitems].atom, 16);
		token_wordcontent(&comp->items[comp->nitems].atom, token);
	}
	++comp->nitems;
}

t_ast_node
	*parse_cmd(t_parser *parser, size_t start, size_t end)
{
	const t_token		*tok;
	struct s_node_cmd	cmd;
	t_ast_node			*node;
	size_t				arg_pos;

	cmd.redirs.redirs = NULL;
	cmd.redirs.redirs_size = 0;
	cmd.redirs.redirs_capacity = 0;
	cmd.nargs = 0;
	if (end - start)
		cmd.args = xmalloc(sizeof(t_ast_node) * (end - start));
	node = xmalloc(sizeof(t_ast_node));
	node->type = NODE_COMMAND;
	node->cmd = cmd;
	arg_pos = 0;
	while (start != end)
	{
		start += parse_redir_repeat(parser, start, end, &node->cmd.redirs);
		if (start >= end)
			break;
		tok = &parser->list.tokens[start];
		if (tok->type == TOK_PARAM || tok->type == TOK_PARAM_SIMPLE
			|| token_isword(tok->type))
		{
			compound_push(node, arg_pos, tok);
			++start;
		}
		else if (tok->type == TOK_SPACE)
		{
			++arg_pos;
			++start;
		}
		else
		{
			parser_error(parser, ft_strdup("Unhandled token type during "
						"command parsing"), start, end);
			return (node);
		}
	}
	return (node);
}
