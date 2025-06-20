/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   complete.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgamba <linogamba@pundalik.org>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 11:59:40 by lgamba            #+#    #+#             */
/*   Updated: 2025/06/19 06:48:54 by thschnei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "ft_printf.h"
#include "getline/buffer/buffer.h"
#include "shell/repl/repl.h"
#include "tokenizer/tokenizer.h"
#include <shell/shell.h>

void
	complete_buf_push(t_complete_buf *buf, t_complete_item item)
{
	size_t	new_cap;

	if (buf->capacity <= buf->size)
	{
		new_cap = (buf->capacity * 2) + !buf->capacity * 16;
		buf->data = ft_realloc(buf->data,
				sizeof(t_complete_item) * buf->capacity,
				sizeof(t_complete_item) * new_cap);
		buf->capacity = new_cap;
	}
	buf->data[buf->size++] = item;
}

static int
	is_cmd_start(t_getline *line, size_t i)
{
	t_token_list *const	list = &((t_repl_data *)line->data)->list;
	size_t				j;

	if (!i || !list->size)
		return (1);
	j = i;
	if (j && list->tokens[j].type == TOK_SPACE)
	{
		--j;
		if (token_isword(list->tokens[j].type))
		{
			return (0);
		}
		return (1);
	}
	while (j && token_isword(list->tokens[j].type))
		--j;
	if (j && list->tokens[j].type == TOK_SPACE)
		--j;
	if (token_isword(list->tokens[j].type))
		return (0);
	return (1);
}

static char
	*get_filter(t_getline *line, size_t *word_start, size_t *word_end, int *cmd)
{
	t_token_list *const	list = &((t_repl_data *)line->data)->list;
	size_t				i;
	t_string_buffer		buf;

	i = 0;
	while (i < list->size)
	{
		if (list->tokens[i].end >= line->cursor_index)
			break ;
		++i;
	}
	*cmd = is_cmd_start(line, i);
	if (i >= list->size || !token_isword(list->tokens[i].type))
	{
		if (i)
		{
			*word_start = list->tokens[i].end;
			*word_end = list->tokens[i].end;
		}
		return (NULL);
	}
	stringbuf_init(&buf, 24);
	token_wordcontent(&buf, &list->tokens[i]);
	*word_start = list->tokens[i].start;
	*word_end = list->tokens[i].end;
	if (!buf.len)
	{
		stringbuf_free(&buf);
		return (NULL);
	}
	return (stringbuf_cstr(&buf));
}

t_complete_item
	*repl_completer(t_getline *line, size_t *word_start, size_t *word_end)
{
	char			*filter;
	t_complete_buf	items;
	int				cmd;

	items.data = NULL;
	items.capacity = 0;
	items.size = 0;
	*word_start = 0;
	*word_end = line->cursor_index;
	cmd = 1;
	filter = get_filter(line, word_start, word_end, &cmd);
	if (cmd)
		repl_complete_cmd(line->shell, &items, filter);
	repl_complete_filename(&items, filter);
	free(filter);
	complete_buf_push(&items, (t_complete_item){0, NULL, NULL});
	return (items.data);
}
