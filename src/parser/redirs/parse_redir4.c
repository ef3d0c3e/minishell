/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_redir4.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgamba <linogamba@pundalik.org>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 11:59:40 by lgamba            #+#    #+#             */
/*   Updated: 2025/03/17 11:59:41 by lgamba           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include <shell/shell.h>

/** @brief Parses [NUM][REDIR][NUM][-] */
static int
	parse_redir_number(
	t_parser *parser,
	t_redirections *redirs,
	t_redirectee source)
{
	const t_token	*tok = &parser->list.tokens[parser->pos + 1];
	int				num;

	if (!token_atoi(parser, parser->pos + 2, &num))
		return (0);
	if (!ft_strcmp(tok->reserved_word, "<&")
		|| !ft_strcmp(tok->reserved_word, "&<"))
	{
		make_redirection(redirs, source, (t_redirectee){.fd = num},
			R_MOVE_INPUT);
		parser->pos += 4;
	}
	else if (!ft_strcmp(tok->reserved_word, ">&")
		|| !ft_strcmp(tok->reserved_word, "&>"))
	{
		make_redirection(redirs, source, (t_redirectee){.fd = num},
			R_MOVE_OUTPUT);
		parser->pos += 4;
	}
	else
		return (0);
	return (1);
}

/** @brief Parses redirections [NUM][REDIR][WORD][-] */
static int
	parse_redir_word(
	t_parser *parser,
	t_redirections *redirs,
	t_redirectee source)
{
	static const t_redir_tok_type	tokens[] = {
	{">&", R_MOVE_OUTPUT_WORD}, {"<&", R_MOVE_INPUT_WORD}};
	const t_redir_tok_type			*found;

	found = redir_alternatives(tokens, 2,
			parser->list.tokens[parser->pos + 1].reserved_word);
	if (!found)
		return (0);
	parser->pos += 2;
	make_redirection(redirs, source,
		(t_redirectee){.filename = parse_word(parser, 0)}, found->type);
	parser->pos += 1;
	return (1);
}

int
	redir_parser4(
	t_parser *parser,
	t_redirections *redirs)
{
	const t_token	*right = &parser->list.tokens[parser->pos + 2];
	t_redirectee	source;
	int				status;

	if (!token_atoi(parser, parser->pos, &source.fd))
		return (0);
	status = 0;
	if (right->type == TOK_DIGIT)
		status = parse_redir_number(parser, redirs, source);
	if (status == 0 && redir_has_minus(parser, 2)
		&& parse_redir_word(parser, redirs, source))
		return (redirs->redirs[redirs->redirs_size - 1]
			.redirectee.filename.natoms != 0);
	return (status);
}
