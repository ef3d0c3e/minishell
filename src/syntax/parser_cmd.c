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

t_ast_node
	*parse_cmd(t_parser *parser, size_t start, size_t end)
{
	const t_token		*tok;
	struct s_node_cmd	cmd;
	t_ast_node			*node;

	cmd.redirs_size = 0;
	cmd.redirs_capacity = 0;
	cmd.nargs = 0;
	cmd.args = xmalloc(sizeof(t_string_buffer) * (end - start));
	stringbuf_init(&cmd.args[0], 16);
	while (start != end)
	{
		tok = &parser->list.tokens[start];
		if (token_wordcontent(&cmd.args[cmd.nargs], tok))
		{

		}
		else if (tok->type == TOK_SPACE)
			stringbuf_init(&cmd.args[++cmd.nargs], 16);
		else if (tok->type == TOK_REDIR)
			token_list_push(&cmd.redirs, *tok);
		else
			parser_error(parser, stringbuf_from("Unhandled token type during "
				"command parsing"));
		++start;
	}
	++cmd.nargs;
	node = xmalloc(sizeof(t_ast_node));
	node->type = NODE_COMMAND;
	node->cmd = cmd;
	return (node);
}
