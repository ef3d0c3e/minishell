/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   braces_split.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgamba <linogamba@pundalik.org>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 11:59:40 by lgamba            #+#    #+#             */
/*   Updated: 2025/03/17 11:59:41 by lgamba           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include <shell/shell.h>

/**
 * @brief Splitting logic for brace groups
 */

/**
 * @brief Counts the number of braces groups inside word `inner`
 *
 * Counts the number of non-nested, `,`-separated words inside `inner`
 *
 * @param inner Word to count in
 *
 * @returns The number of `,`-separated words in `inner`
 */
static size_t
	brace_count(t_word *inner)
{
	size_t	inds[3];
	int		balance;

	balance = 0;
	inds[0] = 0;
	inds[2] = 1;
	while (inds[0] < inner->natoms)
	{
		if ((inner->atoms[inds[0]].type != W_LITERAL
				|| inner->atoms[inds[0]].flags & (FL_SQUOTED | FL_SQUOTED))
			&& ++inds[0])
			continue ;
		inds[1] = 0;
		while (inds[1]++ < inner->atoms[inds[0]].text.len)
		{
			if (inner->atoms[inds[0]].text.str[inds[1] - 1] == '{')
				++balance;
			if (inner->atoms[inds[0]].text.str[inds[1] - 1] == '}')
				--balance;
			if (inner->atoms[inds[0]].text.str[inds[1] - 1] == ',' && !balance)
				++inds[2];
		}
		++inds[0];
	}
	return (inds[2]);
}

/** @brief Splits to an alternative */
static void
	split_alternative(
	t_word *inner,
	t_brace_group *group,
	size_t range[4])
{
	t_brace_group	*alternative;
	t_word			arg;

	alternative = &group->alternatives[group->nalternatives++];
	if (range[3] == (size_t) - 1 && range[2] == inner->natoms)
	{
		range[3] = 0;
		if (inner->atoms[range[2] - 1].type == W_LITERAL)
			range[3] = inner->atoms[inner->natoms - 1].text.len;
	}
	arg = word_sub(inner, range);
	if (!brace_parse(&arg, alternative))
	{
		alternative->alternatives = NULL;
		alternative->nalternatives = 0;
		alternative->selector = 0;
		alternative->prefix = arg;
		alternative->next = NULL;
	}
	else
		word_free(&arg);
}

/** @brief Splits the inner content of a brace group */
static void
	split_inner(t_brace_group *group, t_word *inner)
{
	int		balance;
	size_t	i[4];

	balance = 0;
	ft_memset(i, 0, sizeof(size_t) * 4);
	while (i[0]++ < inner->natoms)
	{
		i[1] = 0;
		while ((inner->atoms[i[0] - 1].type == W_LITERAL
				&& !(inner->atoms[i[0] - 1].flags & (FL_SQUOTED | FL_DQUOTED)))
			&& i[1]++ < inner->atoms[i[0] - 1].text.len)
		{
			(void)((inner->atoms[i[0] - 1].text.str[i[1] - 1] == '{')
				&& ++balance);
			(void)((inner->atoms[i[0] - 1].text.str[i[1] - 1] == '}')
				&& --balance);
			if (inner->atoms[i[0] - 1].text.str[i[1] - 1] == ',' && !balance)
			{
				split_alternative(inner, group,
					(size_t[4]){i[2], i[3], i[0], i[1] - 1});
				i[2] = i[0] - 1;
				i[3] = i[1];
			}
		}
	}
	split_alternative(inner, group, (size_t[4]){i[2], i[3], i[0] - 1, -1});
}

/** @brief Splits the next part of a brace group */
static void
	brace_split_next(
	t_brace_group **next,
	t_word *arg,
	size_t count,
	const size_t delims[4])
{
	t_word				suffix;
	size_t				end;

	end = 0;
	if (arg->atoms[arg->natoms - 1].type == W_LITERAL)
		end = arg->atoms[arg->natoms - 1].text.len;
	*next = xmalloc(sizeof(t_brace_group));
	suffix = word_sub(arg, (const size_t[4]){delims[2], delims[3]
			+ (count != 1), arg->natoms, end});
	if (!brace_parse(&suffix, *next))
	{
		(*next)->alternatives = NULL;
		(*next)->nalternatives = 0;
		(*next)->selector = 0;
		(*next)->prefix = suffix;
		(*next)->next = NULL;
	}
	else
		word_free(&suffix);
}

t_brace_group
	brace_split(t_word *arg, const size_t delims[4])
{
	t_brace_group	group;
	t_word			inner;
	size_t			count;

	group.selector = 0;
	inner = word_sub(arg, (const size_t[4]){delims[0], delims[1] + 1,
			delims[2] + 1, delims[3]});
	count = brace_count(&inner);
	if (brace_parse_range(&group, &inner))
		count = 0;
	group.prefix = word_sub(arg, (const size_t[4]){0, 0, delims[0] + 1,
			delims[1] + (count == 1)});
	if (count)
	{
		group.alternatives = xmalloc(sizeof(t_brace_group) * count);
		group.nalternatives = 0;
		split_inner(&group, &inner);
	}
	group.next = NULL;
	if (delims[2] < arg->natoms)
		brace_split_next(&group.next, arg, count, delims);
	word_free(&inner);
	return (group);
}
