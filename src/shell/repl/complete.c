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
#include "tokenizer/tokenizer.h"
#include <shell/shell.h>

static int
	match(const char *a, const char *b)
{
	size_t	i;
	char	x;
	char	y;
	
	i = 0;
	while (a[i] && b[i])
	{
		x = a[i];
		y = b[i];
		if (x >= 'a' && x <= 'z')
			x -= 32;
		if (y >= 'a' && y <= 'z')
			y -= 32;
		if (x != y)
			return (0);
		++i;
	}
	return (1);
}

static void
	path_traversal(size_t depth, t_rbnode *node, void *cookie)
{
	t_path_tr *const	tr = cookie;
	
	if (tr->filter && !match(tr->filter, node->key))
		return ;
	tr->items[tr->index++] = (t_complete_item){
		.kind = COMPLETE_WORD,
		.name = ft_strdup(node->key),
		.desc = ft_strdup("Executable"),
	};
}

static char
	*get_filter(t_getline *line)
{
	t_token_list	list;
	size_t			i;
	t_string_buffer	buf;

	list = tokenizer_tokenize((t_string){line->input.buffer.str,
			line->input.buffer.len});
	i = 0;
	while (i < list.size)
	{
		if (list.tokens[i].end >= line->cursor_index)
			break ;
		++i;
	}
	if (i >= list.size || !token_isword(list.tokens[i].type))
		return (NULL);
	stringbuf_init(&buf, 24);
	token_wordcontent(&buf, &list.tokens[i]);
	token_list_free(&list);
	if (!buf.len)
	{
		stringbuf_free(&buf);
		return (NULL);
	}
	return (stringbuf_cstr(&buf));
}

t_complete_item
	*repl_completer(t_getline *line)
{
	t_path_tr	tr;

	tr.filter = get_filter(line);
	tr.index = 0;
	tr.items = xmalloc(sizeof(t_complete_item)
			* (line->shell->path_cache.size + 2));
	rb_apply(&line->shell->path_cache, path_traversal, &tr);
	tr.items[tr.index].name = NULL;
	free(tr.filter);
	return (tr.items);
}

