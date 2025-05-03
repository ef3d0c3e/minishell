/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_for.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgamba <linogamba@pundalik.org>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 11:59:40 by lgamba            #+#    #+#             */
/*   Updated: 2025/03/17 11:59:41 by lgamba           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "parser/ast/ast.h"
#include "parser/parser.h"
#include "tokenizer/tokenizer.h"
#include <shell/shell.h>

/** @brief Parses a list of whitespace/newline delimited words */
static void
	parse_arglist(t_parser *parser, t_wordlist *list)
{
	const t_token	*tok;
	size_t			arg_pos;

	if (parser->pos < parser->list.size
		&& parser->list.tokens[parser->pos].type == TOK_SPACE)
		++parser->pos;
	arg_pos = 0;
	while (parser->pos < parser->list.size)
	{
		tok = &parser->list.tokens[parser->pos];
		if (tok->type == TOK_PARAM || tok->type == TOK_PARAM_SIMPLE
			|| tok->type == TOK_CMD_SUB
			|| token_isword(tok->type))
		{
			if (arg_pos >= list->size)
				wordlist_push(parser, list);
			else
				word_push(parser, &list->list[list->size - 1]);
		}
		else if (tok->type == TOK_SPACE)
			++arg_pos;
		else
			break ;
		++parser->pos;
	}
}

static t_ast_node
	*parse_for_word(t_parser *parser)
{
	t_ast_node		*stmt;
	t_string_buffer	buf;

	if (!accept_word(parser, 0))
	{
		parser_error(parser, ft_strdup("Expected word"),
			parser->pos, parser->pos + 1);
		return (NULL);
	}
	stringbuf_init(&buf, 64);
	token_wordcontent(&buf, &parser->list.tokens[parser->pos]);
	if (!is_valid_identifier(stringbuf_cstr(&buf)))
	{
		parser_error(parser, ft_strdup("Not a valid identifier"),
			parser->pos, parser->pos + 1);
		stringbuf_free(&buf);
		return (NULL);
	}
	++parser->pos;
	if (!accept_tok(parser, 0, TOK_SPACE) && !accept(parser, 0, "\n"))
		parser_error(parser, ft_strdup("Expected a space after identifier"),
			parser->pos, parser->pos + 1);
	++parser->pos;
	stmt = make_for_node(stringbuf_cstr(&buf));
	return (stmt);
}

t_ast_node
	*parse_for(t_parser *parser)
{
	t_ast_node		*stmt;

	if (!accept_tok(parser, 1, TOK_SPACE) && !accept(parser, 1, "\n"))
	{
		parser_error(parser, ft_strdup("Expected a space after `for`"),
			parser->pos + 1, parser->pos + 2);
		return (NULL);
	}
	parser->pos += 2;
	stmt = parse_for_word(parser);
	if (!stmt)
		return (NULL);
	expect(parser, 0, "in");
	++parser->pos;
	parser_delimiter_push(parser, "do");
	parse_arglist(parser, &stmt->st_for.args);
	if (!accept(parser, 0, ";") && !accept(parser, 0, "\n"))
		parser_error(parser, ft_strdup("Expected separator after word list"),
			parser->pos, parser->pos + 1);
	++parser->pos;
	if (accept_tok(parser, 0, TOK_SPACE))
		++parser->pos;
	expects_delimiter(parser, "do");
	parser_delimiter_push(parser, "done");
	stmt->st_for.body = parse_cmdlist(parser);
	expects_delimiter(parser, "done");
	return (stmt);
}
