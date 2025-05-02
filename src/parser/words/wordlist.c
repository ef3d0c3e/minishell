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
	wordlist_free(t_word *list, size_t size)
{
	size_t	i;

	i = 0;
	while (i < size)
		word_free(&list[i++]);
	free(list);
}

void
	wordlist_print(size_t depth, const t_word *list, size_t size)
{
	size_t	i;

	i = 0;
	while (i < size)
	{
		word_print(depth, &list[i]);
		++i;
	}
}

void
	wordlist_push(
	t_parser *parser,
	t_word **list,
	size_t *len)
{
	*list = ft_realloc(*list, sizeof(t_word) * *len,
			sizeof(t_word) * (*len + 1));
	(*list)[*len].atoms = NULL;
	(*list)[*len].natoms = 0;
	(*len)++;
	word_push(parser, &(*list)[*len - 1]);
}
