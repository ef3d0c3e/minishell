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
#include <shell/shell.h>

/** @brief Parses [REDIR][NUM][-] */
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
			(t_redirectee){.fd = num}, R_MOVE_INPUT);
		parser->pos += 3;
	}
	else if (!ft_strcmp(tok->reserved_word, ">&")
		|| !ft_strcmp(tok->reserved_word, "&>"))
	{
		make_redirection(redirs, (t_redirectee){.fd = 1},
			(t_redirectee){.fd = num}, R_MOVE_INPUT);
		parser->pos += 3;
	}
	else
		return (0);
	return (1);
}

/** @brief Parses redirections [REDIR][WORD][-] */
static int
	parse_redir_word(
	t_parser *parser,
	t_redirections *redirs)
{
	const t_token	*tok = &parser->list.tokens[parser->pos];

	if (!ft_strcmp(tok->reserved_word, "<&")
		|| !ft_strcmp(tok->reserved_word, "&<"))
	{
		++parser->pos;
		make_redirection(redirs, (t_redirectee){.fd = 0},
			(t_redirectee){.filename = parse_word(parser, 0)},
			R_MOVE_INPUT_WORD);
		++parser->pos;
	}
	else if (!ft_strcmp(tok->reserved_word, ">&")
		|| !ft_strcmp(tok->reserved_word, "&>"))
	{
		++parser->pos;
		make_redirection(redirs, (t_redirectee){.fd = 1},
			(t_redirectee){.filename = parse_word(parser, 0)},
			R_MOVE_INPUT_WORD);
		++parser->pos;
	}
	else
		return (0);
	return (1);
}

// [REDIR][WORD|NUM][-]
int
	redir_parser3_move(
	t_parser *parser,
	t_redirections *redirs)
{
	const t_token	*right = &parser->list.tokens[parser->pos + 1];
	int				status;

	status = 0;
	if (right->type == TOK_DIGIT)
		status = parse_redir_number(parser, redirs);
	else if (status == 0 && redir_has_minus(parser, 1)
		&& parse_redir_word(parser, redirs))
		return (redirs->redirs[redirs->redirs_size - 1]
			.redirectee.filename.natoms != 0);
	return (status);
}
