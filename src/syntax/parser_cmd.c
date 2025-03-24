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
#include "syntax/tokenizer.h"
#include "util/util.h"
#include <sys/types.h>

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

t_ast_node
	*parse_cmd(t_parser *parser, size_t start, size_t end)
{
	struct s_node_cmd	cmd;
	t_ast_node			*node;
	t_string_buffer		buf;

	cmd.redirs = NULL;
	cmd.redirs_size = 0;
	cmd.redirs_capacity = 0;
	cmd.nargs = 0;
	cmd.args = xmalloc(sizeof(t_ast_node) * (end - start));
	node = xmalloc(sizeof(t_ast_node));
	node->type = NODE_COMMAND;
	node->cmd = cmd;
	while (start != end)
	{
		start += parse_redir_repeat(parser, start, end, &node->cmd);
		if (start >= end)
			break;
		buf = cmd_word(parser, &start, end);
		if (buf.len)
		{
			node->cmd.args[node->cmd.nargs].type = NODE_ATOM;
			node->cmd.args[node->cmd.nargs++].atom = buf;
			buf = (t_string_buffer){.str = NULL, .len = 0, .capacity = 0};
		}
		else
		{
			parser_error(parser, stringbuf_from("Unhandled token type during "
						"command parsing"));
		}
		++start;
	}
	return (node);
}
