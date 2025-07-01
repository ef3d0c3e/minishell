/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   word_splitting.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgamba <linogamba@pundalik.org>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 11:59:40 by lgamba            #+#    #+#             */
/*   Updated: 2025/06/19 07:33:17 by thschnei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "ft_printf.h"
#include "parser/words/words.h"
#include <shell/shell.h>

static void
	split_force(
			t_fragment_list *list,
			const char *ifs,
			t_string_buffer *word,
			int fs
			)
{
	size_t					i;
	size_t					last;

	i = 0;
	last = 0;
	while (i < word->len)
	{
		if (!ft_strchr(ifs, word->str[i]))
		{
			++i;
			continue ;
		}
		fraglist_push(list, stringbuf_from_range(word->str + last,
				word->str + i), 0);
		list->fragments[list->size - 1].force_split = fs * (!last) + (!!last);
		++i;
		last = i;
	}
	//if (last != word->len)
	{
		fraglist_push(list, stringbuf_from_range(word->str + last,
				word->str + word->len), 0);
		list->fragments[list->size - 1].force_split = fs * (!last) + (!!last);
	}
	stringbuf_free(word);
}

t_fragment_list
	word_split(t_shell *shell, t_fragment_list *list, const char *ifs)
{
	t_fragment_list	new;
	size_t			i;

	(void)shell;
	fraglist_init(&new);
	i = 0;
	while (i < list->size)
	{
		if ((list->fragments[i].flags & (FL_SQUOTED | FL_DQUOTED)) == 0)
			split_force(&new, ifs, &list->fragments[i].word,
				list->fragments[i].force_split);
		else
		{
			fraglist_push(&new, list->fragments[i].word,
				list->fragments[i].flags);
			new.fragments[new.size - 1].force_split
				= list->fragments[i].force_split;
		}
		++i;
	}
	free(list->fragments);
	return (new);
}
