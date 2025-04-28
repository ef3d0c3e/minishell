/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   word_splitting.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgamba <linogamba@pundalik.org>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 11:59:40 by lgamba            #+#    #+#             */
/*   Updated: 2025/03/17 11:59:41 by lgamba           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include <shell/shell.h>

static void
	split(t_fragment_list *list, const char *ifs, t_string_buffer *word)
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
		fraglist_append(list, stringbuf_from_range(word->str + last,
			word->str + i));
		++i;
		last = i;
	}
	if (last != word->len && last != 0)
		fraglist_push(list, stringbuf_from_range(word->str + last,
				word->str + word->len), 0);
	else if (last != word->len)
		fraglist_append(list, stringbuf_from_range(word->str + last,
				word->str + word->len));
	stringbuf_free(word);
}

static void
	split_force(t_fragment_list *list, const char *ifs, t_string_buffer *word)
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
		++i;
		last = i;
	}
	if (last != word->len)
		fraglist_push(list, stringbuf_from_range(word->str + last,
			word->str + word->len), 0);
	stringbuf_free(word);
}

t_fragment_list
	word_split(t_shell *shell, t_fragment_list *list, const char *ifs)
{
	t_fragment_list	new;
	size_t			i;

	fraglist_init(&new);
	i = 0;
	while (i < list->size)
	{
		ft_dprintf(2, "word[%zu] = `%.*s` %d\n", i, list->fragments[i].word.len, list->fragments[i].word.str, list->fragments[i].force_split);
		if ((list->fragments[i].flags & (FL_DQUOTED | FL_DQUOTED)) == 0 &&
			list->fragments[i].force_split)
			split_force(&new, ifs, &list->fragments[i].word);
		else if ((list->fragments[i].flags & (FL_DQUOTED | FL_DQUOTED)) == 0)
			split(&new, ifs, &list->fragments[i].word);
		else if (list->fragments[i].force_split)
			fraglist_push(&new, list->fragments[i].word, 0);
		else
			fraglist_append(&new, list->fragments[i].word);
		++i;
	}
	free(list->fragments);
	return (new);
}
