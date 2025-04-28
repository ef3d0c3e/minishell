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
#include "tokenizer/tokenizer.h"
#include <shell/shell.h>

/** @brief Performs expansion on literals */
void
	expand_literal(
	t_shell *shell,
	t_fragment_list *list,
	struct s_arg_item *param,
	const char *ifs)
{
	if ((param->flags & (FL_DQUOTED | FL_SQUOTED)) == 0
		&& expand_tilde(shell, list, param, ifs) != -1)
		return ;
	else
		fraglist_push(list, stringbuf_from_range(param->text.str,
			param->text.str + param->text.len), param->flags);
}

/** @brief Performs expansion of a single argument */
static int
	expand_arg(
	t_shell *shell,
	t_fragment_list *list,
	struct s_argument *arg,
	const char *ifs)
{
	const size_t	start_size = list->size;
	size_t	i;

	i = 0;
	while (i < arg->nitems)
	{
		if (arg->items[i].type == ARG_LITERAL)
			expand_literal(shell, list, &arg->items[i], ifs);
		else if (arg->items[i].type == ARG_SUBEXPR)
			expand_subexpr(shell, list, &arg->items[i], ifs);
		else if (arg->items[i].type == ARG_PARAMETER)
		{
			if (!expand_param(shell, list, &arg->items[i], ifs)
				&& option_value(shell, "experr"))
			{
				fraglist_free(&list);
				return (0);
			}
		}
		++i;
	}
	list->fragments[start_size].force_split = 1;
	return (1);
}

char
	**arg_expansion(t_shell *shell, const struct s_cmd_node *cmd)
{
	size_t			i;
	t_fragment_list	list;
	char			**argv;
	const char		*ifs;

	i = 0;
	ifs = rb_find(&shell->reg_env, "IFS");
	if (!ifs || ifs[0] == 0)
		ifs = " \t\n";
	fraglist_init(&list);
	while (i < cmd->nargs)
	{
		if (!expand_arg(shell, &list, &cmd->args[i++], ifs))
			return (NULL);
		
	}
	list = word_split(shell, &list, ifs);
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
