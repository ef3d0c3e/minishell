/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   arglist.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgamba <linogamba@pundalik.org>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 11:59:40 by lgamba            #+#    #+#             */
/*   Updated: 2025/03/17 11:59:41 by lgamba           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include <shell/shell.h>

void
	arglist_free(struct s_argument *list, size_t size)
{
	size_t	i;

	i = 0;
	while (i < size)
		arg_free(&list[i++]);
	free(list);
}

void
	arglist_print(size_t depth, struct s_argument *list, size_t size)
{
	size_t	i;

	i = 0;
	while (i < size)
	{
		arg_print(depth, &list[i]);
		++i;
	}
}

void
	arglist_push(
	t_parser *parser,
	struct s_argument **list,
	size_t *len)
{
	*list = ft_realloc(*list, sizeof(struct s_argument) * *len,
			sizeof(struct s_argument) * (*len + 1));
	(*list)[*len].items = NULL;
	(*list)[*len].nitems = 0;
	(*len)++;
	arg_push(parser, &(*list)[*len - 1]);
}
