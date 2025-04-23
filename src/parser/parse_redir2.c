/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_redir2.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgamba <linogamba@pundalik.org>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 11:59:40 by lgamba            #+#    #+#             */
/*   Updated: 2025/03/17 11:59:41 by lgamba           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include <parser/parser.h>

/** @brief Parses [REDIR][NUMBER] */
static size_t
	parse_redir_number(
	t_parser *parser,
	size_t start,
	t_redirections *redirs)
{
	const t_token	*left = &parser->list.tokens[start + 1];
	int				num;

	if (!token_atoi(parser, start + 1, &num))
		return (0);
	if (!ft_strcmp(left->reserved_word, "<&"))
		make_redirection(redirs, (t_redirectee){.fd = 0},
			(t_redirectee){.fd = num}, R_DUPLICATING_INPUT);
	else if (!ft_strcmp(left->reserved_word, ">&"))
		make_redirection(redirs, (t_redirectee){.fd = 1},
			(t_redirectee){.fd = num}, R_DUPLICATING_OUTPUT);
	else
		return (0);
	return (2);
}

/** @brief Parses [REDIR][-] */
static size_t
	parse_redir_minus(
	t_parser *parser,
	size_t start,
	t_redirections *redirs)
{
	const t_token	*tok = &parser->list.tokens[start];

	if (!ft_strcmp(tok->reserved_word, "<&"))
		make_redirection(redirs, (t_redirectee){.fd = 1},
			(t_redirectee){.fd = 0}, R_CLOSE_THIS);
	else if (!ft_strcmp(tok->reserved_word, ">&"))
		make_redirection(redirs, (t_redirectee){.fd = 0},
			(t_redirectee){.fd = 0}, R_CLOSE_THIS);
	else
		return (0);
	return (2);
}

/** @brief Parses redirections [REDIR][WORD] */
static size_t
	parse_redir_word(
	t_parser *parser,
	size_t start,
	t_redirections *redirs)
{
	static const t_redir_tok_type	outs[] = {
	{">", R_OUTPUT_DIRECTION}, {">>", R_APPENDING_TO}, {">|", R_OUTPUT_FORCE},
	{">&", R_DUPLICATING_OUTPUT_WORD}, {"&>", R_ERR_AND_OUT},
	{"&>>", R_APPEND_ERR_AND_OUT}};
	static const t_redir_tok_type	ins[] = {
	{"<", R_INPUT_DIRECTION}, {"<>", R_INPUT_OUTPUT}, {"<<", R_READING_UNTIL},
	{"<<-", R_DEBLANK_READING_UNTIL}, {"<<<", R_READING_STRING},
	{"<&", R_DUPLICATING_INPUT_WORD}};
	const t_redir_tok_type			*found;
	t_string_buffer					word;

	stringbuf_init(&word, 64);
	token_wordcontent(&word, &parser->list.tokens[start + 1]);
	found = redir_alternatives(outs, 6,
			parser->list.tokens[start].reserved_word);
	if (found)
		return (make_redirection(redirs, (t_redirectee){.fd = 1},
			(t_redirectee){.filename = word}, found->type), 2);
	found = redir_alternatives(ins, 6,
			parser->list.tokens[start].reserved_word);
	if (found)
		return (make_redirection(redirs, (t_redirectee){.fd = 0},
			(t_redirectee){.filename = word}, found->type), 2);
	stringbuf_free(&word);
	return (0);
}

size_t
	redir_parser2(
	t_parser *parser,
	size_t start,
	t_redirections *redirs)
{
	const t_token	*right = &parser->list.tokens[start + 1];
	int				status;

	status = 0;
	if (right->type == TOK_DIGIT)
		status = parse_redir_number(parser, start, redirs);
	else if (right->type == TOK_MINUS)
		status = parse_redir_minus(parser, start, redirs);
	else if (token_isword(right->type))
		status = parse_redir_word(parser, start, redirs);
	if (!status)
		parser_error(parser, ft_strdup("Invalid redirection"),
			start, start + 1);
	return (2);
}
