/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fraglist.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgamba <linogamba@pundalik.org>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 11:59:40 by lgamba            #+#    #+#             */
/*   Updated: 2025/03/17 11:59:41 by lgamba           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include <shell/shell.h>

void
	fraglist_init(t_fragment_list *list)
{
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
