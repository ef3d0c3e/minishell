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
#include "ft_printf.h"
#include "parser/parser.h"
#include "tokenizer/tokenizer.h"
#include "util/util.h"
#include <shell/shell.h>
#include <stddef.h>
#include <string.h>

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

void
	expand_arg(t_shell *shell, t_fragment_list *list, struct s_argument *arg)
{
	const size_t	start_size = list->size;
	size_t	i;

	i = 0;
	while (i < arg->nitems)
	{
		if (arg->items[i].type == ARG_LITERAL)
			fraglist_push(list, stringbuf_from_range(arg->items[i].text.str,
				arg->items[i].text.str + arg->items[i].text.len),
				arg->items[i].flags);
		else if (arg->items[i].type == ARG_SUBEXPR)
			expand_subexpr(shell, list, &arg->items[i]);
		else if (arg->items[i].type == ARG_PARAMETER)
			expand_param(shell, list, &arg->items[i]);
		++i;
	}
	list->fragments[start_size].force_split = 1;
}

static void
	split(t_fragment_list *list, const char *ifs, t_string_buffer *word)
{
	size_t	i;
	size_t	last;

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

static t_fragment_list
	word_split(t_shell *shell, t_fragment_list *list)
{
	t_fragment_list	new;
	size_t			i;
	const char		*ifs;

	new.fragments = NULL;
	new.capacity = 0;
	new.size = 0;
	ifs = rb_find(&shell->reg_env, "IFS");
	if (!ifs)
		ifs = " \t\n";
	i = 0;
	while (i < list->size)
	{
		if ((list->fragments[i].flags & (FL_DQUOTED | FL_DQUOTED)) == 0)
			split(&new, ifs, &list->fragments[i].word);
		else
			fraglist_push(&new, list->fragments[i].word, 0);
		++i;
	}
	free(list->fragments);
	return (new);
}

char
	**arg_expansion(t_shell *shell, const struct s_cmd_node *cmd)
{
	size_t			i;
	t_fragment_list	list;
	char			**argv;

	i = 0;
	list.size = 0;
	list.capacity = 0;
	list.fragments = NULL;
	while (i < cmd->nargs)
	{
		expand_arg(shell, &list, &cmd->args[i]);
		++i;
	}
	// TODO: Word splitting here: fraglist into a new fraglist
	list = word_split(shell, &list);
	argv = xmalloc(sizeof(char *) * (list.size + 1));
	i = 0;
	while (i < list.size)
	{
		argv[i] = stringbuf_cstr(&list.fragments[i].word);
		ft_dprintf(2, "argv[%zu] = '%s'\n", i, argv[i]);
		++i;
	}
	argv[i] = NULL;
	free(list.fragments);
	return (argv);
}
