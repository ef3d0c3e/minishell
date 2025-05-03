/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wordlist.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgamba <linogamba@pundalik.org>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 11:59:40 by lgamba            #+#    #+#             */
/*   Updated: 2025/03/17 11:59:41 by lgamba           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include <shell/shell.h>

void
	wordlist_free(t_wordlist *list)
{
	size_t	i;

	i = 0;
	while (i < list->size)
		word_free(&list->list[i++]);
	free(list->list);
}

void
	wordlist_print(size_t depth, const t_wordlist *list)
{
	size_t	i;

	i = 0;
	while (i < list->size)
		word_print(depth, &list->list[i++]);
}

void
	wordlist_push(
	t_parser *parser,
	t_wordlist *list)
{
	list->list = ft_realloc(list->list,
			sizeof(t_word) * list->size,
			sizeof(t_word) * (list->size + 1));
	list->list[list->size].atoms = NULL;
	list->list[list->size].natoms = 0;
	word_push(parser, &list->list[list->size++]);
}
