/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fraglist.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgamba <linogamba@pundalik.org>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 11:59:40 by lgamba            #+#    #+#             */
/*   Updated: 2025/06/19 13:03:33 by thschnei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "ft_printf.h"
#include <shell/shell.h>

void
	fraglist_init(t_fragment_list *list)
{
	list->fragments = NULL;
	list->size = 0;
	list->capacity = 0;
}

void
	fraglist_free(t_fragment_list *list)
{
	size_t	i;

	i = 0;
	while (i < list->size)
		stringbuf_free(&list->fragments[i++].word);
	free(list->fragments);
	list->fragments = NULL;
	list->size = 0;
	list->capacity = 0;
}

void
	fraglist_push(t_fragment_list *list, t_string_buffer word, int flags)
{
	if (list->size + 1 >= list->capacity)
	{
		list->capacity = list->capacity * 2 + !list->capacity * 4;
		list->fragments = ft_realloc(list->fragments,
				sizeof(t_fragment) * list->size,
				sizeof(t_fragment) * list->capacity);
	}
	list->fragments[list->size++] = (t_fragment){
		.word = word,
		.flags = flags,
	};
}

void
	fraglist_append(t_fragment_list *list, t_string_buffer word)
{
	if (!list->size)
	{
		fraglist_push(list, word, 0);
		return ;
	}
	stringbuf_append(&list->fragments[list->size - 1].word, (t_string){word.str,
		word.len});
	stringbuf_free(&word);
}

char
	**fraglist_to_argv(t_fragment_list *list)
{
	char			**argv;
	t_string_buffer	buf;
	size_t			size;
	size_t			i;

	argv = xmalloc(sizeof(char *) * (list->size + 1));
	size = 0;
	i = 0;
	stringbuf_init(&buf, 24);
	while (i < list->size)
	{
		if (list->fragments[i].force_split && i)
		{
			argv[size++] = stringbuf_cstr(&buf);
			stringbuf_init(&buf, 24);
		}
		stringbuf_append(&buf, (t_string){list->fragments[i].word.str,
			list->fragments[i].word.len});
		++i;
	}
	if (i)
		argv[size++] = stringbuf_cstr(&buf);
	else
		stringbuf_free(&buf);
	return (argv[size] = NULL, fraglist_free(list), argv);
}

void
	fraglist_print(const t_fragment_list *list)
{
	for (size_t i = 0; i < list->size; ++i)
	{
		ft_dprintf(2, "word[%zu] = '%.*s' fs:%d fl=%d\n", i, (int)list->fragments[i].word.len, list->fragments[i].word.str, list->fragments[i].force_split, list->fragments[i].flags & (FL_SQUOTED | FL_DQUOTED));
	}
}
