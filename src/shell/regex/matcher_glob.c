/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   matcher_glob.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgamba <linogamba@pundalik.org>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 11:59:40 by lgamba            #+#    #+#             */
/*   Updated: 2025/03/17 11:59:41 by lgamba           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include <shell/shell.h>

/** @brief Matches '+' and '*' as part of an extglob expression */
static int
	match_more(
		const t_globopts *opts,
		t_regex_ast **groups,
		size_t ngroups,
		const char *s)
{
	const t_regex_ast	*node = groups[0];
	t_regex_ast *const	*alts = node->glob.groups;
	const char			*rest;
	size_t				i;
	int					r;

	i = 0;
	if (node->glob.op == '*')
	{
		r = regex_match_seq(opts, groups + 1, ngroups - 1, s);
		if (r)
			return (r);
	}
	while (i++ < node->glob.ngroups)
		if (regex_match_one(opts, alts[i - 1], s, &rest) == 2)
			return (regex_match_seq(opts, groups, ngroups, rest));
	return (0);
}

/** @brief Matches '@' and '?' as part of an extglob expression */
static int
	match_any(
		const t_globopts *opts,
		t_regex_ast **groups,
		size_t ngroups,
		const char *s)
{
	const t_regex_ast	*node = groups[0];
	t_regex_ast *const	*alts = node->glob.groups;
	const char			*rest;
	size_t				i;
	int					r;

	i = 0;
	if (node->glob.op == '?')
	{
		r = regex_match_seq(opts, groups + 1, ngroups - 1, s);
		if (r)
			return (r);
	}
	while (i++ < node->glob.ngroups)
	{
		if (regex_match_one(opts, alts[i - 1], s, &rest) == 2)
		{
			r = regex_match_seq(opts, groups + 1, ngroups - 1, rest);
			if (r)
				return (r);
		}
	}
	return (0);
}

/** @brief Matches '!' as part of an extglob expression */
static int
	match_not(
		const t_globopts *opts,
		t_regex_ast **groups,
		const char *s)
{
	const t_regex_ast	*node = groups[0];
	t_regex_ast *const	*alts = node->glob.groups;
	int					partial;
	size_t				i;
	int					r;

	i = 0;
	partial = 0;
	while (i++ < node->glob.ngroups)
	{
		r = regex_match_seq(opts, alts[i - 1]->compound.groups,
				alts[i - 1]->compound.ngroups, s);
		if (r == 2)
			return (0);
		else if (r == 1)
			partial = 1;
	}
	return (partial + !partial * 2);
}

int
	regex_match_extglob(
		const t_globopts *opts,
		t_regex_ast **groups,
		size_t ngroups,
		const char *s)
{
	const t_regex_ast	*node = groups[0];

	if (node->glob.op == '*' || node->glob.op == '+')
		return (match_more(opts, groups, ngroups, s));
	else if (node->glob.op == '@' || node->glob.op == '?')
		return (match_any(opts, groups, ngroups, s));
	else if (node->glob.op == '!')
		return (match_not(opts, groups, s));
	return (0);
}
