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

static char
	*get_filter(t_getline *line, size_t *word_start, size_t *word_end)
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
	if (i >= list->size || !token_isword(list->tokens[i].type))
		return (NULL);
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

	items.data = NULL;
	items.capacity = 0;
	items.size = 0;
	*word_start = 0;
	*word_end = line->cursor_index;
	filter = get_filter(line, word_start, word_end);
	repl_complete_cmd(line->shell, &items, filter);
	repl_complete_filename(&items, filter);
	free(filter);
	complete_buf_push(&items, (t_complete_item){0, NULL, NULL});
	return (items.data);
}
