/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_arg.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgamba <linogamba@pundalik.org>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 11:59:40 by lgamba            #+#    #+#             */
/*   Updated: 2025/03/17 11:59:41 by lgamba           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include <shell/shell.h>

//ft_dprintf(2, "exp list fl=%d '%.*s'\n", param->flags,
//param->text.len, param->text.str);
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
		fraglist_push(list, stringbuf_from_range(param->text.str,
				param->text.str + param->text.len), param->flags);
	}
}

static int
	expand_arg_loop(t_shell *shell, void *dat[2], size_t i[2], const char *ifs)
{
	t_wordlist *const		expanded = dat[0];
	t_fragment_list *const	list = dat[1];
	int						status;

	status = 1;
	if (expanded->list[i[0]].atoms[i[1]].type == W_LITERAL)
		expand_literal(shell, list, &expanded->list[i[0]].atoms[i[1]], ifs);
	else if (expanded->list[i[0]].atoms[i[1]].type == W_SUBEXPR)
	{
		rb_insert(&shell->atexit, expanded, (void *)wordlist_free);
		status = expand_subexpr(shell, list, &expanded->list[i[0]].atoms[i[1]]);
		rb_delete(&shell->atexit, expanded);
		if (!status)
			return (fraglist_free(list), wordlist_free(expanded), 0);
	}
	else if (expanded->list[i[0]].atoms[i[1]].type == W_PARAMETER)
		status = expand_param(shell, list, &expanded->list[i[0]].atoms[i[1]],
				ifs);
	if (status == -1)
		return (fraglist_free(list), wordlist_free(expanded), 0);
	++i[1];
	return (1);
}

/** @brief Performs expansion of a single argument */
int
	expand_arg(
	t_shell *shell,
	t_fragment_list *list,
	t_word *arg,
	const char *ifs)
{
	const size_t	start_size = list->size;
	t_wordlist		expanded;
	size_t			i[2];
	size_t			tmp;

	expand_braces(arg, &expanded);
	i[0] = 0;
	while (i[0] < expanded.size)
	{
		tmp = list->size;
		i[1] = 0;
		while (i[1] < expanded.list[i[0]].natoms)
		{
			if (!expand_arg_loop(shell, (void *[2]){&expanded, list}, i, ifs))
				return (0);
		}
		if (tmp < list->size)
			list->fragments[tmp].force_split = 1;
		++i[0];
	}
	wordlist_free(&expanded);
	if (start_size < list->size)
		list->fragments[start_size].force_split = 1;
	return (1);
}
