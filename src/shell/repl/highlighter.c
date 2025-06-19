/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   highlighter.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgamba <linogamba@pundalik.org>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 11:59:40 by lgamba            #+#    #+#             */
/*   Updated: 2025/06/19 06:46:29 by thschnei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "shell/repl/repl.h"
#include "tokenizer/tokenizer.h"
#include <shell/shell.h>
#include <stddef.h>

static t_buffer_attr
	get_highlight(enum e_token_type type)
{
	static t_buffer_attr	hl[] = {
	[TOK_KEYWORD] = {0, 0, 0x81a1c1, 0, 0, 0},
	[TOK_OPERATOR] = {0, 0, 0x88c0d0, 0, 0, 0},
	[TOK_SEQUENCE] = {0, 0, 0x5e81ac, 0, 0, 0},
	[TOK_PIPELINE] = {0, 0, 0x5e81ac, 0, 0, 0},
	[TOK_GROUPING] = {0, 0, 0x8fbcbb, 0, 0, 0},
	[TOK_ASSIGN] = {0, 0, 0xb48ead, 0, 0, 0},
	[TOK_REDIR] = {0, 0, 0xbf616a, 0, 0, 0},
	[TOK_COMMENT] = {0, 0, 0x4c566a, 0, 0, 0},
	[TOK_HEREDOC] = {0, 0, 0x4c566a, 0, 0, 0},
	[TOK_HERESTRING] = {0, 0, 0x4c566a, 0, 0, 0},
	[TOK_DIGIT] = {0, 0, 0xd08770, 0, 0, 0},
	[TOK_SPACE] = {0, 0, 0x3b4252, 0, 0, 0},
	[TOK_MINUS] = {0, 0, 0xa3be8c, 0, 0, 0},
	[TOK_SINGLE_QUOTE] = {0, 0, 0xebcb8b, 0, 0, 0},
	[TOK_WORD] = {0, 0, 0x88c0d0, 0, 0, 0},
	[TOK_CMD_SUB] = {0, 0, 0x8fbcbb, 0, 0, 0},
	[TOK_ARITH] = {0, 0, 0xd08770, 0, 0, 0},
	[TOK_PARAM_SIMPLE] = {0, 0, 0xa3be8c, 0, 0, 0},
	[TOK_PARAM] = {0, 0, 0xa3be8c, 0, 0, 0},
	[TOK_ERROR] = {0, 0, 0xff5555, 0, 0, 0},
	};

	return (hl[type]);
}

void
	repl_highlighter(t_getline *line)
{
	t_repl_data *const	data = line->data;
	t_buffer_attr		hi;
	size_t				i;

	token_list_free(&data->list);
	data->list = tokenizer_tokenize((t_string){line->input.buffer.str,
			line->input.buffer.len});
	i = 0;
	while (i < data->list.size)
	{
		hi = get_highlight(data->list.tokens[i].type);
		hi.start = data->list.tokens[i].start;
		hi.end = data->list.tokens[i].end;
		getline_highlight_add(&line->input, hi);
		i++;
	}
}
