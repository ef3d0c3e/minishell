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
#include "util/util.h"
#include <shell/shell.h>

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
	else if ((param->flags & (FL_DQUOTED | FL_SQUOTED)) == 0
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
	struct s_word *arg,
	const char *ifs)
{
	const size_t	start_size = list->size;
	size_t			i;
	int				status;

	// This yields a list of arguments
	expand_braces(shell, arg);
	i = 0;
	while (i < arg->natoms)
	{
		status = 1;
		if (arg->atoms[i].type == W_LITERAL)
			expand_literal(shell, list, &arg->atoms[i], ifs);
		else if (arg->atoms[i].type == W_SUBEXPR)
			status = expand_subexpr(shell, list, &arg->atoms[i], ifs);
		else if (arg->atoms[i].type == W_PARAMETER)
			status = expand_param(shell, list, &arg->atoms[i], ifs);
		if (status == -1)
		{
			fraglist_free(list);
			return (0);
		}
		++i;
	}
	//printf("HERE: st=%zu\n", start_size);
	//if (status)
	if (start_size < list->size)
		list->fragments[start_size].force_split = 1;
	return (1);
}

static void cleanup(void *ptr)
{
	t_fragment_list *const	list = ptr;

	fraglist_free(list);
}

char
	**arg_expansion(t_shell *shell, struct s_word *words, size_t size)
{
	size_t			i;
	t_fragment_list	list;
	char			**argv;
	const char		*ifs;

	rb_insert(&shell->temporaries, &list, (void *)cleanup);
	i = 0;
	ifs = get_variable_value(shell, "IFS");
	if (!ifs || ifs[0] == 0)
		ifs = " \t\n";
	fraglist_init(&list);
	while (i < size)
	{
		if (!expand_arg(shell, &list, &words[i++], ifs))
		{
			rb_delete(&shell->temporaries, &list);
			return (NULL);
		}
	}
	list = word_split(shell, &list, ifs);
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
}

char
	*arg_expansion_single(t_shell *shell, struct s_word *arg)
{
	t_fragment_list	list;
	const char		*ifs;
	char			*result;
	size_t			i;
	size_t			size;
	
	rb_insert(&shell->temporaries, &list, (void *)cleanup);
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

