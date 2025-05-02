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
#include <shell/shell.h>

/** @brief Parses a list of whitespace/newline delimited words */
static void
	parse_arglist(t_parser *parser, struct s_wordlist **args, size_t *len)
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
			|| (arg_pos != 0 && token_isword(tok->type))
			|| accept_word(parser, 0))
		{
			if (arg_pos >= *len)
				arglist_push(parser, args, len);
			else
				arg_push(parser, &(*args)[*len - 1]);
		}
		else if (tok->type == TOK_SPACE)
			++arg_pos;
		else
			break ;
		++parser->pos;
	}
}

t_ast_node
	*parse_for(t_parser *parser)
{
	int				in_stmt = !parser->allow_reserved;
	t_ast_node		*stmt;
	t_string_buffer	buf;

	if (!accept_tok(parser, 1, TOK_SPACE) && !accept(parser, 1, "\n"))
	{
		parser_error(parser, ft_strdup("Expected a space after `for`"),
			parser->pos + 1, parser->pos + 2);
		return (NULL);
	}
	parser->pos += 2;
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
	expect(parser, 0, "in");
	++parser->pos;
	parser->allow_reserved = 0;
	parse_arglist(parser, &stmt->st_for.args, &stmt->st_for.nargs);
	if (!accept(parser, 0, ";") && !accept(parser, 0, "\n"))
		parser_error(parser, ft_strdup("Expected separator after word list"),
			parser->pos, parser->pos + 1);
	expect(parser, 1, "do");
	parser->pos += 2;
	stmt->st_for.body = parse_cmdlist(parser);
	parser->allow_reserved = !in_stmt;
	expect(parser, 0, "done");
	++parser->pos;
	return (stmt);
}
