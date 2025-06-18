/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   matcher_seq.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgamba <linogamba@pundalik.org>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 11:59:40 by lgamba            #+#    #+#             */
/*   Updated: 2025/03/17 11:59:41 by lgamba           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include <shell/shell.h>

/** @brief Matches `*` and `**` as part of a M_SEQ expression */
static int
	match_star(
		const t_globopts *opts,
		t_regex_ast **groups,
		size_t ngroups,
		const char *s)
{
	const t_regex_ast	*node = groups[0];
	size_t				i;
	int					r;

	i = 0;
	if (!opts->dotglob && s[0] == '.')
		return (0);
	while (++i)
	{
		r = regex_match_seq(opts, groups + 1, ngroups - 1, s + i - 1);
		if (r)
			return (r);
		if (!s[i - 1] || (node->type == M_STAR && s[i - 1] == '/'))
			break ;
	}
	return (!s[i - 1] * 2);
}

int
	regex_match_seq(
		const t_globopts *opts,
		t_regex_ast **groups,
		size_t ngroups,
		const char *s)
{
	const t_regex_ast	*node;
	const char			*rest;
	int					r;

	if (ngroups == 0)
		return (!s[0] * 2);
	node = groups[0];
	if (node->type == M_STAR || node->type == M_GLOBSTAR)
		return (match_star(opts, groups, ngroups, s));
	else if (node->type == M_EXTGLOB)
		return (regex_match_extglob(opts, groups, ngroups, s));
	r = regex_match_one(opts, node, s, &rest);
	if (r <= 1)
		return (r);
	if (!*rest && ngroups > 1)
		return (groups[0]->type != M_LITERAL || groups[0]->literal[0] != '/');
	return (regex_match_seq(opts, groups + 1, ngroups - 1, rest));
}
