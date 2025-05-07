/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgamba <linogamba@pundalik.org>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 11:59:40 by lgamba            #+#    #+#             */
/*   Updated: 2025/03/17 11:59:41 by lgamba           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "shell/expand/expand.h"
#include "parser/words/words.h"
#include "util/util.h"
#include <math.h>
#include <shell/shell.h>
#include <stddef.h>
#include <stdio.h>

/** @brief Performs expansion on literals */
void
	expand_literal(
	t_shell *shell,
	t_fragment_list *list,
	struct s_atom *param,
	const char *ifs)
{
	if ((param->flags & (FL_DQUOTED | FL_SQUOTED)) == 0
		&& expand_tilde(shell, list, param, ifs) != 0)
		return ;
	else
	{
		//ft_dprintf(2, "exp list fl=%d '%.*s'\n", param->flags, param->text.len, param->text.str);
		fraglist_push(list, stringbuf_from_range(param->text.str,
					param->text.str + param->text.len), param->flags);
	}
}

/** @brief Performs expansion of a single argument */
static int
	expand_arg(
	t_shell *shell,
	t_fragment_list *list,
	t_word *arg,
	const char *ifs)
{
	const size_t	start_size = list->size;
	t_wordlist		expanded;
	size_t			tmp;
	size_t			j;
	size_t			i;
	int				status;

	// This yields a list of arguments
	expand_braces(arg, &expanded);
	i = 0;
	while (i < expanded.size)
	{
		tmp = list->size;
		j = 0;
		while (j < expanded.list[i].natoms)
		{
			status = 1;
			if (expanded.list[i].atoms[j].type == W_LITERAL)
				expand_literal(shell, list, &expanded.list[i].atoms[j], ifs);
			else if (expanded.list[i].atoms[j].type == W_SUBEXPR)
			{
				rb_insert(&shell->temporaries, &expanded, (void *)wordlist_free);
				status = expand_subexpr(shell, list, &expanded.list[i].atoms[j], ifs);
				rb_delete(&shell->temporaries, &expanded);
				if (!status)
				{
					wordlist_free(&expanded);
					return (0);
				}
			}
			else if (expanded.list[i].atoms[j].type == W_PARAMETER)
				status = expand_param(shell, list, &expanded.list[i].atoms[j], ifs);
			if (status == -1)
			{
				fraglist_free(list);
				wordlist_free(&expanded);
				return (0);
			}
			++j;
		}
		if (tmp < list->size)
			list->fragments[tmp].force_split = 1;
		++i;
	}
	wordlist_free(&expanded);
	//printf("HERE: st=%zu\n", start_size);
	//if (status)
	if (start_size < list->size)
		list->fragments[start_size].force_split = 1;
	return (1);
}

char
	**word_expansion(t_shell *shell, t_wordlist *words)
{
	char			**argv;
	size_t			i;
	t_fragment_list	list;
	const char		*ifs;

	rb_insert(&shell->temporaries, &list, (void *)fraglist_free);
	i = 0;
	ifs = get_variable_value(shell, "IFS");
	if (!ifs || ifs[0] == 0)
		ifs = " \t\n";
	fraglist_init(&list);
	while (i < words->size)
	{
		if (!expand_arg(shell, &list, &words->list[i++], ifs))
		{
			rb_delete(&shell->temporaries, &list);
			return (NULL);
		}
	}
	list = word_split(shell, &list, ifs);
	list = expand_filename(shell, &list);

	//i = 0;
	//while (i < list.size)
	//{
	//	ft_dprintf(2, "f[%zu] = `%.*s` fl=%05o fs=%d\n", i, list.fragments[i].word.len, list.fragments[i].word.str, list.fragments[i].flags, list.fragments[i].force_split);
	//	++i;
	//}
	rb_delete(&shell->temporaries, &list);
	argv = fraglist_to_argv(&list);
	//i = 0;
	//ft_dprintf(2, "---\n");
	//while (argv[i])
	//{
	//	ft_dprintf(2, "argv[%zu] = '%s'\n", i, argv[i]);
	//	++i;
	//}
	return (argv);
	/*
	argv = xmalloc(sizeof(char *) * (list.size + 1));
	i = 0;
	while (i < list.size)
	{
		argv[i] = stringbuf_cstr(&list.fragments[i].word);
		//ft_dprintf(2, "argv[%zu] = '%s'\n", i, argv[i]);
		++i;
	}
	argv[i] = NULL;
	free(list.fragments);
	rb_delete(&shell->temporaries, &list);
	return (argv);
	*/
}

char
	**word_expansion_single(t_shell *shell, t_word *word)
{
	t_fragment_list	list;
	size_t			i;
	char			**argv;
	const char		*ifs;

	rb_insert(&shell->temporaries, &list, (void *)fraglist_free);
	ifs = get_variable_value(shell, "IFS");
	if (!ifs || ifs[0] == 0)
		ifs = " \t\n";
	fraglist_init(&list);
	if (!expand_arg(shell, &list, word, ifs))
	{
		rb_delete(&shell->temporaries, &list);
		return (NULL);
	}
	list = word_split(shell, &list, ifs);
	rb_delete(&shell->temporaries, &list);
	argv = fraglist_to_argv(&list);
	return (argv);
}

char
	*word_expansion_cat(t_shell *shell, struct s_word *arg)
{
	t_fragment_list	list;
	const char		*ifs;
	char			*result;
	size_t			i;
	size_t			size;
	
	rb_insert(&shell->temporaries, &list, (void *)fraglist_free);
	ifs = get_variable_value(shell, "IFS");
	if (!ifs || ifs[0] == 0)
		ifs = " \t\n";
	fraglist_init(&list);
	if (!expand_arg(shell, &list, arg, ifs))
	{
		rb_delete(&shell->temporaries, &list);
		return (NULL);
	}
	size = 0;
	i = 0;
	while (i < list.size)
		size += list.fragments[i++].word.len;
	result = xmalloc(size + 1);
	size = 0;
	i = 0;
	while (i < list.size)
	{
		ft_memcpy(&result[size], list.fragments[i].word.str, list.fragments[i].word.len);
		size += list.fragments[i++].word.len;
	}
	fraglist_free(&list);
	rb_delete(&shell->temporaries, &list);
	return (result[size] = 0, result);
}

