/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   brace_group.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgamba <linogamba@pundalik.org>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 11:59:40 by lgamba            #+#    #+#             */
/*   Updated: 2025/03/17 11:59:41 by lgamba           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include <shell/shell.h>

void
	brace_group_free(t_brace_group *group, int root)
{
	size_t	i;

	word_free(&group->prefix);
	i = 0;
	while (i < group->nalternatives)
		brace_group_free(&group->alternatives[i++], 1);
	free(group->alternatives);
	if (group->next)
		brace_group_free(group->next, 0);
	if (!root)
		free(group);
}

void
	brace_group_print(size_t depth, const t_brace_group *group)
{
	size_t	i;

	print_pad(" | ", depth);
	ft_dprintf(2, "PREFIX ");
	if (group->prefix.natoms)
		word_print(0, &group->prefix);
	else
		ft_dprintf(2, "\n");
	i = 0;
	while (i < group->nalternatives)
	{
		print_pad(" | ", depth);
		ft_dprintf(2, "ALTERNATIVES %zu\n", i);
		brace_group_print(depth + 1, &group->alternatives[i]);
		++i;
	}
	if (group->next)
	{
		print_pad(" | ", depth);
		ft_dprintf(2, "NEXT\n");
		brace_group_print(depth + 1, group->next);
	}
}
