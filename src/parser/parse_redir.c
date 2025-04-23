/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_redir.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgamba <linogamba@pundalik.org>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 11:59:40 by lgamba            #+#    #+#             */
/*   Updated: 2025/03/17 11:59:41 by lgamba           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "parser.h"
#include "parser/redir.h"
#include "tokenizer/tokenizer.h"
#include "util/util.h"
#include <limits.h>
#include <stddef.h>

/** @brief Adds a redirection to command */
//static inline void
//	add_redir(
//			t_redirections *redirs,
//			t_token token,
//			int fd_from,
//			t_string_buffer file_to)
//{
//	size_t	new_cap;
//
//	new_cap = redirs->redirs_capacity + !redirs->redirs_capacity * 16;
//	while (new_cap < redirs->redirs_size + 1)
//		new_cap *= 2;
//	redirs->redirs = ft_realloc(redirs->redirs,
//			redirs->redirs_capacity * sizeof(t_redir_data),
//			new_cap * sizeof(t_redir_data));
//	redirs->redirs_capacity = new_cap;
//	redirs->redirs[redirs->redirs_size++] = (t_redir_data){
//		.token = token,
//		.fd = fd_from,
//		.word = file_to,
//	};
//}

// TODO: Handle other redirections
/*
 [N]>[|]WORD
 [N]>>WORD
 [N]>&WORD

 [N]<WORD
 [N]<&WORD

 [N]<&DIGIT-
 [N]>&DIGIT-

 &>WORD
 >&WORD
 >WORD 2>&1
 &>>WORD

 [N]<<[-]WORD
		HERE-DOCUMENT
 [N]<<< WORD
*/

void
	add_redir(t_redirections *redirs, t_redirection redir)
{
	size_t	new_cap;

	new_cap = redirs->redirs_capacity + !redirs->redirs_capacity * 16;
	while (new_cap < redirs->redirs_size + 1)
		new_cap *= 2;
	redirs->redirs = ft_realloc(redirs->redirs,
			redirs->redirs_capacity * sizeof(redir),
			new_cap * sizeof(redir));
	redirs->redirs_capacity = new_cap;
	redirs->redirs[redirs->redirs_size++] = redir;
}

void
	make_redirection(
	t_redirections *redirs,
	t_redirectee source,
	t_redirectee dest,
	enum e_redir_type type)
{
	t_redirection	redir;

	redir.redirector = source;
	redir.redirectee = dest;
	redir.type = type;
	redir.flags = 0;
	redir.here_doc_eof = NULL;
	add_redir(redirs, redir);
}

int
	pat_word(const t_token *tok)
{
	return (token_isword(tok->type) && tok->type != TOK_DIGIT);
}

int
	pat_number(const t_token *tok)
{
	return (tok->type == TOK_DIGIT);
}

int
	pat_minus(const t_token *tok)
{
	return (tok->type == TOK_MINUS);
}

/** @brief Parses redirections [N][REDIR][WORD|N] */

size_t
	redir_parse_number(
	t_parser *parser,
	size_t start,
	t_redirections *redirs)
{
	/*
	static const t_redir_pat	patterns[] = {
		{pat_number, ">", pat_word, R_OUTPUT_DIRECTION},
		{pat_number, "<", pat_word, R_INPUT_DIRECTION},
		{pat_number, ">>", pat_word, R_APPENDING_TO},
		{pat_number, ">|", pat_word, R_OUTPUT_FORCE},
		{pat_number, "<>", pat_word, R_INPUT_OUTPUT},
		{pat_number, "<<", pat_word, R_READING_UNTIL},
		{pat_number, "<<-", pat_word, R_DEBLANK_READING_UNTIL},
		{pat_number, "<<<", pat_word, R_READING_STRING},
		{pat_number, "<&", pat_number, R_DUPLICATING_INPUT},
		{pat_number, ">&", pat_number, R_DUPLICATING_OUTPUT},
		{pat_number, "<&", pat_word, R_DUPLICATING_INPUT_WORD},
		{pat_number, ">&", pat_word, R_DUPLICATING_OUTPUT_WORD},
		{pat_number, "<&", pat_minus, R_CLOSE_THIS},
		{pat_number, ">&", pat_minus, R_CLOSE_THIS},
	};
	*/
	printf("HERE2\n");
	return (3);
}


/**
 * @brief Checks if `a * 10 + sign * b` will overflow
 *
 * @param a Integer value
 * @param sign The sign (-1 or +1)
 * @param b A value between 0 and 9
 *
 * @returns 1 If `a * 10 + sign * b` will overflow
 */
int muladd_10s_overflow(int a, int sign, int b)
{
	// Check if a * 10 will overflow
	if (a > INT_MAX / 10 || a < INT_MIN / 10)
		return 1;

	int mul10 = a * 10;
	int sb = sign * b;

	// Check if adding cb will overflow
	if ((sb > 0 && mul10 > INT_MAX - sb) ||
			(sb < 0 && mul10 < INT_MIN - sb))
		return 1;

	return 0;
}

int
	token_atoi(t_parser *parser, size_t start, int *value)
{
	const t_token	*tok = &parser->list.tokens[start];
	size_t			pos;
	int				sign;

	if (tok->type != TOK_DIGIT)
		return (parser_error(parser, ft_strdup("Expected number"), start, start),
			0);
	pos = 0;
	sign = 1;
	if (tok->word.str[pos] == '+' && ++pos)
		sign = 1;
	else if (tok->word.str[pos] == '-' && ++pos)
		sign = -1;
	*value = 0;
	while (pos < tok->word.len && tok->word.str[pos] >= '0'
		&& tok->word.str[pos] <= '9')
	{
		if (muladd_10s_overflow(*value, sign, tok->word.str[pos] - '0'))
			return (parser_error(parser, ft_strdup("Integer has overflown"),
				start, start), 0);
		*value = *value * 10 + sign * (tok->word.str[pos] - '0');
		++pos;
	}
	return (1);
}

const t_redir_tok_type
	*redir_alternatives(
	const t_redir_tok_type *types,
	size_t len,
	const t_token *tok)
{
	size_t	i;

	i = 0;
	while (i < len)
	{
		if (!ft_strcmp(types[i].tok, tok->reserved_word))
			return (&types[i]);
		++i;
	}
	return (NULL);
}

/** @brief Parses redirections [REDIR][WORD] */
size_t
	redir_parse_redir_word(
	t_parser *parser,
	size_t start,
	t_redirections *redirs)
{
	static const t_redir_tok_type	outs[] = {
		{">", R_OUTPUT_DIRECTION}, {">>", R_APPENDING_TO},
		{">|", R_OUTPUT_FORCE}, {">&", R_DUPLICATING_OUTPUT_WORD},
		{"&>", R_ERR_AND_OUT}, {"&>>", R_APPEND_ERR_AND_OUT},
	};
	static const t_redir_tok_type	ins[] = {
		{"<", R_INPUT_DIRECTION}, {"<>", R_INPUT_OUTPUT},
		{"<<", R_READING_UNTIL}, {"<<-", R_DEBLANK_READING_UNTIL},
		{"<<<", R_READING_STRING}, {"<&", R_DUPLICATING_INPUT_WORD},
	};
	const t_redir_tok_type			*found;

	found = redir_alternatives(outs, 6, &parser->list.tokens[start]);
	if (found)
		return (make_redirection(redirs, (t_redirectee){.fd = 1},
				(t_redirectee){.filename = parser->list.tokens[start + 1].word},
				found->type), 2);
	found = redir_alternatives(ins, 6, &parser->list.tokens[start]);
	if (found)
		make_redirection(redirs, (t_redirectee){.fd = 0},
				(t_redirectee){.filename = parser->list.tokens[start + 1].word},
				found->type);
	else
		parser_error(parser, ft_strdup("Invalid redirection"), start, start + 1);
	return (2);
}


/** @brief Parses redirections [REDIR][WORD|N|-] */
size_t
	redir_parse(
	t_parser *parser,
	size_t start,
	t_redirections *redirs)
{
	const t_token	*left = &parser->list.tokens[start + 1];
	int				num;

	if (left->type == TOK_DIGIT)
	{
		if (!token_atoi(parser, start + 1, &num))
			return (2);
		if (!ft_strcmp(left->reserved_word, "<&"))
			make_redirection(redirs, (t_redirectee){.fd = 0},
				(t_redirectee){.fd = num}, R_DUPLICATING_INPUT);
		else if (!ft_strcmp(left->reserved_word, ">&"))
			make_redirection(redirs, (t_redirectee){.fd = 1},
				(t_redirectee){.fd = num}, R_DUPLICATING_OUTPUT);
		else
			parser_error(parser, ft_strdup("Invalid redirection"),
					start, start + 1);
	}
	else if (left->type == TOK_MINUS)
	{
		if (!ft_strcmp(left->reserved_word, "<&"))
			make_redirection(redirs, (t_redirectee){.fd = 1},
				(t_redirectee){.fd = 0}, R_CLOSE_THIS);
		else if (!ft_strcmp(left->reserved_word, ">&"))
			make_redirection(redirs, (t_redirectee){.fd = 0},
				(t_redirectee){.fd = 0}, R_CLOSE_THIS);
		else
			parser_error(parser, ft_strdup("Invalid redirection"),
					start, start + 1);
	}
	else
		return (redir_parse_redir_word(parser, start, redirs));
	return (2);
}

size_t	parse_redir(
	t_parser *parser,
	size_t start,
	size_t end,
	t_redirections *redirs)
{
	const t_token	*tok = &parser->list.tokens[start];

	if (end - start < 2)
		return (0);
	else if (tok->type == TOK_REDIR)
		return (redir_parse(parser, start, redirs));
	else if (tok->type == TOK_DIGIT && end - start >= 3)
		return (redir_parse_number(parser, start, redirs));
	return (0);
}

size_t	parse_redir_repeat(
	t_parser *parser,
	size_t start,
	size_t end,
	t_redirections *redirs)
{
	size_t	skipped;
	size_t	result;
	size_t	spaces;

	skipped = 0;
	while (1)
	{
		spaces = 0;
		while (start + skipped + spaces < end
			&& parser->list.tokens[start + skipped + spaces].type == TOK_SPACE)
			++spaces;
		if (start + skipped + spaces >= end)
			break;
		result = parse_redir(parser, start + skipped + spaces, end, redirs);
		skipped += result;
		if (!result || start + skipped >= end)
			break;
		skipped += spaces;

	}
	return (skipped);
}
