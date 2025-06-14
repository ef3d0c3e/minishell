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
#include <shell/shell.h>

/** @brief Parses [REDIR][NUMBER] */
static int
	parse_redir_number(
	t_parser *parser,
	t_redirections *redirs)
{
	const t_token	*tok = &parser->list.tokens[parser->pos];
	int				num;

	if (!token_atoi(parser, parser->pos + 1, &num))
		return (0);
	if (!ft_strcmp(tok->reserved_word, "<&")
		|| !ft_strcmp(tok->reserved_word, "&<"))
	{
		make_redirection(redirs, (t_redirectee){.fd = 0},
			(t_redirectee){.fd = num}, R_DUPLICATING_INPUT);
		parser->pos += 2;
	}
	else if (!ft_strcmp(tok->reserved_word, ">&")
		|| !ft_strcmp(tok->reserved_word, "&<"))
	{
		make_redirection(redirs, (t_redirectee){.fd = 1},
			(t_redirectee){.fd = num}, R_DUPLICATING_OUTPUT);
		parser->pos += 2;
	}
	else
		return (0);
	return (1);
}

/** @brief Parses [REDIR][-] */
static int
	parse_redir_minus(
	t_parser *parser,
	t_redirections *redirs)
{
	const t_token	*tok = &parser->list.tokens[parser->pos];

	if (!ft_strcmp(tok->reserved_word, "<&")
		|| !ft_strcmp(tok->reserved_word, "&<"))
	{
		make_redirection(redirs, (t_redirectee){.fd = 1},
			(t_redirectee){.fd = 0}, R_CLOSE_THIS);
		parser->pos += 2;
	}
	else if (!ft_strcmp(tok->reserved_word, ">&")
		|| !ft_strcmp(tok->reserved_word, "&>"))
	{
		make_redirection(redirs, (t_redirectee){.fd = 0},
			(t_redirectee){.fd = 0}, R_CLOSE_THIS);
		parser->pos += 2;
	}
	else
		return (0);
	return (1);
}

/** @brief Parses redirections [REDIR][WORD] */
static int
	parse_redir_word(
	t_parser *parser,
	t_redirections *redirs)
{
	static const t_redir_tok_type	outs[] = {
	{">", R_OUTPUT_DIRECTION}, {">>", R_APPENDING_TO}, {">|", R_OUTPUT_FORCE},
	{">&", R_DUPLICATING_OUTPUT_WORD}, {"&>", R_ERR_AND_OUT},
	{"&>>", R_APPEND_ERR_AND_OUT}};
	static const t_redir_tok_type	ins[] = {
	{"<", R_INPUT_DIRECTION}, {"<>", R_INPUT_OUTPUT}, {"<<<", R_READING_STRING},
	{"<&", R_DUPLICATING_INPUT_WORD}};
	const t_redir_tok_type			*found;

	found = redir_alternatives(outs, 6,
			parser->list.tokens[parser->pos].reserved_word);
	if (found && ++parser->pos)
		return (make_redirection(redirs, (t_redirectee){.fd = 1},
			(t_redirectee){.filename = parse_word(parser, 0)}, found->type), 1);
	found = redir_alternatives(ins, 4,
			parser->list.tokens[parser->pos].reserved_word);
	if (!found)
		return (0);
	++parser->pos;
	make_redirection(redirs, (t_redirectee){.fd = 0},
		(t_redirectee){.filename = parse_word(parser, 0)}, found->type);
	return (1);
}

/** @brief Parses a HEREDOC redirection */
static int
	parse_redir_heredoc(
	t_parser *parser,
	t_redirections *redirs)
{
	static const t_redir_tok_type	ins[] = {
	{"<<-", R_DEBLANK_READING_UNTIL}, {"<<", R_READING_UNTIL}};
	const t_redir_tok_type			*found;

	found = redir_alternatives(ins, 2,
			parser->list.tokens[parser->pos].reserved_word);
	if (!found)
		return (0);
	++parser->pos;
	make_redirection(redirs, (t_redirectee){.filename = parse_word(parser, 0)},
		(t_redirectee){.fd = 0}, found->type);
	push_heredoc(parser, &redirs->redirs[redirs->redirs_size - 1]);
	return (1);
}

int
	redir_parser2(
	t_parser *parser,
	t_redirections *redirs)
{
	const t_token	*right = &parser->list.tokens[parser->pos + 1];
	int				status;

	status = 0;
	if (right->type == TOK_DIGIT)
		status = parse_redir_number(parser, redirs);
	else if (right->type == TOK_MINUS)
		status = parse_redir_minus(parser, redirs);
	if (status == 0 && parse_redir_word(parser, redirs))
		return (redirs->redirs[redirs->redirs_size - 1]
			.redirectee.filename.natoms != 0);
	if (status == 0 && parse_redir_heredoc(parser, redirs))
		return (redirs->redirs[redirs->redirs_size - 1]
			.redirector.filename.natoms != 0);
	return (status);
}
