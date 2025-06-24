/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_command.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgamba <linogamba@pundalik.org>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 11:59:40 by lgamba            #+#    #+#             */
/*   Updated: 2025/03/17 11:59:41 by lgamba           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include <shell/shell.h>

t_ast_node
	*parse_compound_command_stmt(t_parser *parser, size_t begin)
{
	if (accept(parser, 0, "if"))
		return (parse_if(parser));
	else if (accept(parser, 0, "while"))
		return (parse_while(parser));
	else if (accept(parser, 0, "until"))
		return (parse_until(parser));
	else if (accept(parser, 0, "for"))
		return (parse_for(parser));
	else
		parser_error(parser, ft_strdup("Unexpected token"),
			begin, parser->list.size);
	return (NULL);
}

t_ast_node
	*parse_compound_command(t_parser *parser)
{
	const size_t	begin = parser->pos;
	t_ast_node		*inner;
	t_ast_node		*node;

	node = NULL;
	if (accept(parser, 0, "(") && ++parser->pos)
	{
		inner = parse_cmdlist(parser);
		expect(parser, 0, ")");
		++parser->pos;
		node = make_subshell_node(inner);
		parse_redir_repeat(parser, &node->sub.redirs);
	}
	else if (accept(parser, 0, "{") && ++parser->pos)
	{
		if (parser->list.tokens[parser->pos].type == TOK_SPACE)
			++parser->pos;
		parser_delimiter_push(parser, "}");
		inner = parse_cmdlist(parser);
		expects_delimiter(parser, "}");
		node = make_block_node(inner);
	}
	else
		return (parse_compound_command_stmt(parser, begin));
	return (node);
}

t_ast_node
	*parse_cmdlist(t_parser *parser)
{
	t_ast_node	*cmd;
	t_ast_node	*list;
	t_token		*sep;

	cmd = NULL;
	if (parser->pos < parser->list.size)
		cmd = parse_and_or(parser);
	list = make_list_node();
	if (cmd)
		list_node_push(list, cmd, '\0');
	while (accept(parser, 0, ";") || accept(parser, 0, "\n"))
	{
		sep = &parser->list.tokens[parser->pos];
		++parser->pos;
		if (parser->pos >= parser->list.size)
			break ;
		cmd = parse_and_or(parser);
		if (cmd)
			list_node_push(list, cmd, sep->reserved_word[0]);
	}
	if (list->list.ncmds == 0 && parser->list.size)
		parser_error(parser, ft_strdup("Expected tokens near newline"),
			parser->list.size - 1, parser->list.size);
	return (list);
}

t_ast_node
	*parse_command(t_parser *parser)
{
	if (accept_tok(parser, 0, TOK_SPACE))
		++parser->pos;
	if (parser->pos >= parser->list.size)
	{
		parser_error(parser, ft_strdup("Expected tokens near newline"),
			parser->list.size - 1, parser->list.size);
		return (NULL);
	}
	if (token_isword(parser->list.tokens[parser->pos].type)
		&& accept(parser, 1, "(")
		&& accept(parser, 2, ")"))
		return (parse_function_def(parser));
	if (accept(parser, 0, "(") || accept(parser, 0, "{")
		|| accept(parser, 0, "if")
		|| accept(parser, 0, "while")
		|| accept(parser, 0, "until")
		|| accept(parser, 0, "for")
		|| accept(parser, 0, "case"))
		return (parse_compound_command(parser));
	return (parse_simple_command(parser));
}
