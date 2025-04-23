/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_redir_move.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgamba <linogamba@pundalik.org>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 11:59:40 by lgamba            #+#    #+#             */
/*   Updated: 2025/03/17 11:59:41 by lgamba           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "parser/redir.h"
#include "tokenizer/tokenizer.h"
#include "util/util.h"
#include <parser/parser.h>

/** @brief Parses [REDIR][NUM][-] */
static int
	parse_redir_number(
	t_parser *parser,
	size_t start,
	t_redirections *redirs)
{
	const t_token	*tok = &parser->list.tokens[start];
	int				num;

	if (!token_atoi(parser, start + 1, &num))
		return (0);
	if (!ft_strcmp(tok->reserved_word, "<&"))
		make_redirection(redirs, (t_redirectee){.fd = 0},
			(t_redirectee){.fd = num}, R_MOVE_INPUT);
	else if (!ft_strcmp(tok->reserved_word, ">&"))
		make_redirection(redirs, (t_redirectee){.fd = 1},
			(t_redirectee){.fd = num}, R_MOVE_INPUT);
	else
		return (0);
	return (1);
}

/** @brief Parses redirections [REDIR][WORD][-] */
static int
	parse_redir_word(
	t_parser *parser,
	size_t start,
	t_redirections *redirs)
{
	const t_token	*tok = &parser->list.tokens[start];
	t_string_buffer	word;

	stringbuf_init(&word, 64);
	token_wordcontent(&word, &parser->list.tokens[start + 1]);
	if (!ft_strcmp(tok->reserved_word, "<&"))
		make_redirection(redirs, (t_redirectee){.fd = 0},
			(t_redirectee){.filename = word}, R_MOVE_INPUT_WORD);
	else if (!ft_strcmp(tok->reserved_word, ">&"))
		make_redirection(redirs, (t_redirectee){.fd = 1},
			(t_redirectee){.filename = word}, R_MOVE_INPUT_WORD);
	else
	{
		stringbuf_free(&word);
		return (0);
	}
	return (1);
}

// [REDIR][WORD|NUM][-]
size_t
	redir_parser3_move(
	t_parser *parser,
	size_t start,
	t_redirections *redirs)
{
	const t_token	*right = &parser->list.tokens[start + 1];
	int				status;

	status = 0;
	if (right->type == TOK_DIGIT)
		status = parse_redir_number(parser, start, redirs);
	else if (token_isword(right->type) && right->type != TOK_MINUS)
		status = parse_redir_word(parser, start, redirs);
	if (!status)
		parser_error(parser, ft_strdup("Invalid redirection"),
			start, start + 3);
	return (3);
}
