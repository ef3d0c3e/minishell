/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   brace_parse.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgamba <linogamba@pundalik.org>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 11:59:40 by lgamba            #+#    #+#             */
/*   Updated: 2025/03/17 11:59:41 by lgamba           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include <shell/shell.h>

/**
 * @file Parser for brace groups
 */

static int
	brace_parse_inner(t_word *arg, size_t i[2], size_t delims[2], int *balance)
{
	if (arg->atoms[i[0]].text.str[i[1]] == '{')
	{
		if (!*balance)
		{
			delims[0] = i[0];
			delims[1] = i[1];
		}
		++*balance;
	}
	if (arg->atoms[i[0]].text.str[i[1]] == '}')
	{
		if (*balance == 1)
		{
			delims[2] = i[0];
			delims[3] = i[1];
		}
		--*balance;
		if (*balance < 0)
			*balance = 0;
	}
	return (delims[0] != (size_t) - 1 && delims[2] != (size_t) - 1);
}

int
	brace_parse(t_word *arg, t_brace_group *group)
{
	size_t	i[2];
	size_t	delims[4];
	int		balance;

	i[0] = 0;
	delims[0] = -1;
	delims[2] = -1;
	balance = 0;
	while (i[0] < arg->natoms)
	{
		i[1] = 0;
		while ((arg->atoms[i[0]].type == W_LITERAL
				&& !(arg->atoms[i[0]].flags & (FL_SQUOTED | FL_SQUOTED)))
			&& i[1] < arg->atoms[i[0]].text.len)
		{
			if (brace_parse_inner(arg, i, delims, &balance))
			{
				*group = brace_split(arg, delims);
				return (1);
			}
			++i[1];
		}
		++i[0];
	}
	return (0);
}
