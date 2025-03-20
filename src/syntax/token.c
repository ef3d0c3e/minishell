/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgamba <linogamba@pundalik.org>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 11:59:40 by lgamba            #+#    #+#             */
/*   Updated: 2025/03/17 11:59:41 by lgamba           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "tokenizer.h"
#include "util/util.h"
#include <stdio.h>

void
	token_print_debug(int fd, t_string prompt, const t_token *token)
{
	static const char	*names[] = {
	[TOK_SPACE] = "SPACE",
	[TOK_DIGIT] = "DIGIT",
	[TOK_NEWLINE] = "NEWLINE",
	[TOK_GROUPING] = "GROUPING",
	[TOK_PIPELINE] = "PIPELINE",
	[TOK_SEQUENCE] = "SEQUENCE",
	[TOK_KEYWORD] = "KEYWORD",
	[TOK_REDIR] = "REDIR",
	[TOK_COMMENT] = "COMMENT",
	[TOK_HEREDOC] = "HEREDOC",
	[TOK_HERESTRING] = "HERESTRING",
	[TOK_SINGLE_QUOTE] = "SINGLE QUOTE",
	[TOK_DOUBLE_QUOTE] = "DOUBLE QUOTE",
	[TOK_EOF] = "EOF",
	[TOK_ERROR] = "ERROR",
	[TOK_ERROR_CUSTOM] = "ERROR_CUSTOM",
	};
	const t_string	literal = {.str = prompt.str + token->start,
		.len = token->end - token->start };

	dprintf(2, "(%s, %zu..%zu, `%.*s`)",
		names[token->type],
		token->start, token->end,
		(int)literal.len, literal.str);
}

void
	token_free(t_token *token)
{
	// TODO
}
