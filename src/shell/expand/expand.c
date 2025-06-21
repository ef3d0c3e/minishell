/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgamba <linogamba@pundalik.org>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 11:59:40 by lgamba            #+#    #+#             */
/*   Updated: 2025/03/17 11:59:41 by lgamba           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "shell/expand/expand.h"
#include <shell/shell.h>
#include <stddef.h>

//i = 0;
//while (i < list.size)
//{
//	ft_dprintf(2, "f[%zu] = `%.*s` fl=%05o fs=%d\n", i,
//	list.fragments[i].word.len, list.fragments[i].word.str,
//	list.fragments[i].flags, list.fragments[i].force_split);
//	++i;
//}
// rb_delete...
//
//i = 0;
//ft_dprintf(2, "---\n");
//while (argv[i])
//{
//	ft_dprintf(2, "argv[%zu] = '%s'\n", i, argv[i]);
//	++i;
//}
// return
char
	**word_expansion(t_shell *shell, t_wordlist *words)
{
	char			**argv;
	size_t			i;
	t_fragment_list	list;
	const char		*ifs;

	rb_insert(&shell->atexit, &list, (void *)fraglist_free);
	i = 0;
	ifs = get_variable_value(shell, "IFS");
	if (!ifs || ifs[0] == 0)
		ifs = " \t\n";
	fraglist_init(&list);
	while (i < words->size)
	{
		if (!expand_arg(shell, &list, &words->list[i++], ifs))
		{
			rb_delete(&shell->atexit, &list);
			fraglist_free(&list);
			return (NULL);
		}
	}
	list = word_split(shell, &list, ifs);
	list = expand_filename(shell, &list);
	rb_delete(&shell->atexit, &list);
	argv = fraglist_to_argv(&list);
	return (argv);
}

char
	**word_expansion_single(t_shell *shell, t_word *word)
{
	t_fragment_list	list;
	char			**argv;
	const char		*ifs;

	rb_insert(&shell->atexit, &list, (void *)fraglist_free);
	ifs = get_variable_value(shell, "IFS");
	if (!ifs || ifs[0] == 0)
		ifs = " \t\n";
	fraglist_init(&list);
	if (!expand_arg(shell, &list, word, ifs))
	{
		rb_delete(&shell->atexit, &list);
		return (NULL);
	}
	list = word_split(shell, &list, ifs);
	rb_delete(&shell->atexit, &list);
	argv = fraglist_to_argv(&list);
	return (argv);
}

static char
	*expansion_cat(t_shell *shell, t_fragment_list *list)
{
	char	*result;
	size_t	size;
	size_t	i;

	size = 0;
	i = 0;
	while (i < list->size)
		size += list->fragments[i++].word.len;
	result = xmalloc(size + 1);
	size = 0;
	i = 0;
	while (i < list->size)
	{
		ft_memcpy(&result[size], list->fragments[i].word.str,
			list->fragments[i].word.len);
		size += list->fragments[i++].word.len;
	}
	fraglist_free(list);
	rb_delete(&shell->atexit, &list);
	return (result[size] = 0, result);
}

char
	*word_expansion_cat(t_shell *shell, struct s_word *arg)
{
	t_fragment_list	list;
	const char		*ifs;

	rb_insert(&shell->atexit, &list, (void *)fraglist_free);
	ifs = get_variable_value(shell, "IFS");
	if (!ifs || ifs[0] == 0)
		ifs = " \t\n";
	fraglist_init(&list);
	if (!expand_arg(shell, &list, arg, ifs))
	{
		rb_delete(&shell->atexit, &list);
		return (NULL);
	}
	return (expansion_cat(shell, &list));
}
