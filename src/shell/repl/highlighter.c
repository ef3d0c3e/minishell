/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   repl.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgamba <linogamba@pundalik.org>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 11:59:40 by lgamba            #+#    #+#             */
/*   Updated: 2025/03/17 11:59:41 by lgamba           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "ft_printf.h"
#include "tokenizer/tokenizer.h"
#include <shell/shell.h>

static t_buffer_attr
	get_highlight(enum e_token_type type)
{
	static t_buffer_attr	hl[] = {
		[TOK_KEYWORD] = {0, 0, 0x1f3fdf, 0, 0, 0},
		[TOK_OPERATOR] = {0, 0, 0x1f3fdf, 0, 0, 0},
		[TOK_SEQUENCE] = {0, 0, 0x1f3fdf, 0, 0, 0},
		[TOK_PIPELINE] = {0, 0, 0x1f3fdf, 0, 0, 0},
		[TOK_GROUPING] = {0, 0, 0x1f3fdf, 0, 0, 0},
		[TOK_ASSIGN] = {0, 0, 0x1f3fdf, 0, 0, 0},
		[TOK_REDIR] = {0, 0, 0x1f3fdf, 0, 0, 0},
		[TOK_COMMENT] = {0, 0, 0x7f7f7f, 0, 0, 0},
		[TOK_HEREDOC] = {0, 0, 0x7f7f7f, 0, 0, 0},
		[TOK_HERESTRING] = {0, 0, 0x7f7f7f, 0, 0, 0},
		[TOK_DIGIT] = {0, 0, 0x006fdd, 0, 0, 0},
		[TOK_SPACE] = {0, 0, 0x006fdd, 0, 0, 0},
		[TOK_MINUS] = {0, 0, 0x006fdd, 0, 0, 0},
		[TOK_SINGLE_QUOTE] = {0, 0, 0x006fdd, 0, 0, 0},
		[TOK_WORD] = {0, 0, 0x006fdd, 0, 0, 0},
		[TOK_CMD_SUB] = {0, 0, 0xcf8f1c, 0, 0, 0},
		[TOK_ARITH] = {0, 0, 0xcf8f1c, 0, 0, 0},
		[TOK_PARAM_SIMPLE] = {0, 0, 0xcf8f1c, 0, 0, 0},
		[TOK_PARAM] = {0, 0, 0xcf8f1c, 0, 0, 0},
		[TOK_ERROR] = {0, 0, 0xDF0000, 0, 0, 0},
	};

	return (hl[type]);
}

void
	repl_highlighter(t_getline *line)
{
	t_token_list	list;
	t_buffer_attr	hi;

	list = tokenizer_tokenize((t_string){line->input.buffer.str,
			line->input.buffer.len});
	for (size_t i = 0; i < list.size; ++i)
	{
		hi = get_highlight(list.tokens[i].type);
		hi.start = list.tokens[i].start;
		hi.end = list.tokens[i].end;
		getline_highlight_add(&line->input, hi);
	}
	token_list_free(&list);
}
