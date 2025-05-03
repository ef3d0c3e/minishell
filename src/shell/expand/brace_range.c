/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   braces_range.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgamba <linogamba@pundalik.org>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 11:59:40 by lgamba            #+#    #+#             */
/*   Updated: 2025/03/17 11:59:41 by lgamba           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include <shell/shell.h>

static void
	expand_range(t_brace_group *group, int start, int end, int incr)
{
	int		i;
	size_t	size;

	incr = (!incr) + incr * ((incr > 0) - (incr < 0));
	size = ((end - start) / incr + 1) * (end >= start)
		+ ((start - end) / incr + 1) * (end < start);
	group->nalternatives = 0;
	group->alternatives = xmalloc(sizeof(t_brace_group) * size);
	i = start;
	while (1)
	{
		group->alternatives[group->nalternatives++] = (t_brace_group){
			.prefix = word_from_int(i, 0),
			.nalternatives = 0,
			.alternatives = NULL,
			.next = NULL,
			.selector = 0,
		};
		i = i + incr * ((start <= end) - (end < start));
		if ((start <= end && i > end) || (start >= end && i < end))
			break ;
	}
}

int
	brace_parse_range(t_brace_group *group, t_word *inner)
{
	t_string	s;
	int			v[3];
	int			incr;
	size_t		i;

	if (inner->natoms != 1)
		return (0);
	s = (t_string){inner->atoms[0].text.str, inner->atoms[0].text.len};
	i = str_find(s, "..");
	if (i == (size_t)-1 || !atoi_range_checked(s.str, s.str + i, &v[0]))
		return (0);
	s = (t_string){s.str + i + 2, s.len - i - 2};
	i = str_find(s, "..");
	incr = 1;
	if (i == (size_t)-1)
	{
		incr = 0;
		i = s.len;
	}
	if (!atoi_range_checked(s.str, s.str + i, &v[1]))
		return (0);
	v[2] = 1;
	if (incr && !atoi_range_checked(s.str + i + 2, s.str + s.len, &v[2]))
		return (0);
	return (expand_range(group, v[0], v[1], v[2]), 1);
}
