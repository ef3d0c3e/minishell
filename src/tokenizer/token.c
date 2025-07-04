/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgamba <linogamba@pundalik.org>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 11:59:40 by lgamba            #+#    #+#             */
/*   Updated: 2025/06/18 08:31:56 by thschnei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "tokenizer.h"

void
	token_print_debug(t_string prompt, const t_token *token)
{
	static const char	*names[] = {
	[TOK_SPACE] = "SPACE", [TOK_DIGIT] = "DIGIT",
	[TOK_MINUS] = "MINUS", [TOK_GROUPING] = "GROUPING",
	[TOK_PIPELINE] = "PIPELINE", [TOK_OPERATOR] = "OPERATOR",
	[TOK_SEQUENCE] = "SEQUENCE", [TOK_KEYWORD] = "KEYWORD",
	[TOK_REDIR] = "REDIR", [TOK_COMMENT] = "COMMENT",
	[TOK_HEREDOC] = "HEREDOC", [TOK_HERESTRING] = "HERESTRING",
	[TOK_SINGLE_QUOTE] = "SINGLE QUOTE", [TOK_CMD_SUB] = "COMMAND SUB",
	[TOK_PARAM_SIMPLE] = "PARAM_SIMPLE", [TOK_PARAM] = "PARAM",
	[TOK_ARITH] = "ARITH", [TOK_ASSIGN] = "ASSIGN",
	[TOK_WORD] = "WORD", [TOK_ERROR] = "ERROR",
	};
	const t_string		literal = {.str = prompt.str + token->start,
		.len = token->end - token->start};

	ft_dprintf(2, "(%s, %zu..%zu, `%.*s`)",
		names[token->type],
		token->start, token->end,
		(int)literal.len, literal.str);
}

void
	token_free(t_token *token)
{
	if (token->type == TOK_WORD
		|| token->type == TOK_SINGLE_QUOTE
		|| token->type == TOK_DIGIT
		|| token->type == TOK_CMD_SUB
		|| token->type == TOK_ASSIGN
		|| token->type == TOK_PARAM_SIMPLE
		|| token->type == TOK_PARAM)
		stringbuf_free(&token->word);
}
