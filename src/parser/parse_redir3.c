/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_redir3.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgamba <linogamba@pundalik.org>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 11:59:40 by lgamba            #+#    #+#             */
/*   Updated: 2025/03/17 11:59:41 by lgamba           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include <shell/shell.h>

/** @brief Parses [NUM][REDIR][NUM] */
static int
	parse_redir_number(
	t_parser *parser,
	size_t start,
	t_redirections *redirs,
	t_redirectee source)
{
	const t_token	*tok = &parser->list.tokens[start + 1];
	int				num;

	if (!token_atoi(parser, start + 2, &num))
		return (0);
	if (!ft_strcmp(tok->reserved_word, "<&"))
		make_redirection(redirs, source, (t_redirectee){.fd = num},
			R_DUPLICATING_INPUT);
	else if (!ft_strcmp(tok->reserved_word, ">&"))
		make_redirection(redirs, source, (t_redirectee){.fd = num},
			R_DUPLICATING_OUTPUT);
	else
		return (0);
	return (1);
}

/** @brief Parses [NUM][REDIR][-] */
static int
	parse_redir_minus(
	t_parser *parser,
	size_t start,
	t_redirections *redirs,
	t_redirectee source)
{
	const t_token	*tok = &parser->list.tokens[start];

	if (!ft_strcmp(tok->reserved_word, "<&"))
		make_redirection(redirs, source, (t_redirectee){.fd = 1},
			R_CLOSE_THIS);
	else if (!ft_strcmp(tok->reserved_word, ">&"))
		make_redirection(redirs, source, (t_redirectee){.fd = 0},
			R_CLOSE_THIS);
	else
		return (0);
	return (1);
}

/** @brief Parses redirections [NUM][REDIR][WORD] */
static int
	parse_redir_word(
	t_parser *parser,
	size_t start,
	t_redirections *redirs,
	t_redirectee source)
{
	static const t_redir_tok_type	tokens[] = {
	{">", R_OUTPUT_DIRECTION}, {">>", R_APPENDING_TO}, {">|", R_OUTPUT_FORCE},
	{">&", R_DUPLICATING_OUTPUT_WORD}, {"<", R_INPUT_DIRECTION},
	{"<>", R_INPUT_OUTPUT}, {"<<", R_READING_UNTIL},
	{"<<-", R_DEBLANK_READING_UNTIL}, {"<<<", R_READING_STRING},
	{"<&", R_DUPLICATING_INPUT_WORD}};
	const t_redir_tok_type			*found;
	t_string_buffer					word;

	found = redir_alternatives(tokens, 11,
			parser->list.tokens[start + 1].reserved_word);
	if (!found)
		return (0);
	stringbuf_init(&word, 64);
	token_wordcontent(&word, &parser->list.tokens[start + 2]);
	make_redirection(redirs, source, (t_redirectee){.filename = word},
		found->type);
	return (1);
}

size_t
	redir_parser3(
	t_parser *parser,
	size_t start,
	t_redirections *redirs)
{
	const t_token	*right = &parser->list.tokens[start + 2];
	const t_token	*left = &parser->list.tokens[start];
	t_redirectee	source;
	int				status;

	if (!token_atoi(parser, start, &source.fd))
		return (3);
	status = 0;
	if (right->type == TOK_DIGIT)
		status = parse_redir_number(parser, start, redirs, source);
	else if (right->type == TOK_MINUS)
		status = parse_redir_minus(parser, start, redirs, source);
	else if (token_isword(right->type))
		status = parse_redir_word(parser, start, redirs, source);
	if (!status)
		return (0);
	return (3);
}
