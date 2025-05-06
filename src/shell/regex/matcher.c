/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   matcher.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgamba <linogamba@pundalik.org>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 11:59:40 by lgamba            #+#    #+#             */
/*   Updated: 2025/03/17 11:59:41 by lgamba           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "ft_printf.h"
#include "shell/regex/regex.h"
#include <shell/shell.h>

static int
	ascii_eq(const t_globopts *opts, char l, char r)
{
	if (!opts->nocaseglob || (unsigned char)l > 127 || (unsigned char)r > 127)
		return (l == r);
	if (l >= 'A' && l <= 'Z')
		l += 32;
	if (r >= 'A' && r <= 'Z')
		r += 32;
	return (l == r);
}

static int
match_seq(const t_globopts *opts, t_regex_ast **groups, size_t ngroups, const char *s);

static int
	match_one(
		const t_globopts *opts,
		const t_regex_ast *node,
		const char *s,
		const char **s_next)
{
	size_t	len;
	size_t	i;

	i = 0;
	len = 0;
	if (node->type == M_LITERAL)
	{
		len = ft_strlen(node->literal);
		while (i < len)
			if (!s[i] || !ascii_eq(opts, s[i], node->literal[i]))
				return (!s[i]);
			else
				++i;
		return (*s_next = s + len, 2);
	}
	else if (node->type == M_ANY)
	{
		if (!s[0] || (!opts->dotglob && s[0] == '.'))
			return (!s[0]);
		return (*s_next = s + 1, 2);
	}
	return (0);
}

static int
	match_extglob(
		const t_globopts *opts,
		t_regex_ast **groups,
		size_t ngroups,
		const char *s)
{
	const t_regex_ast	*node = groups[0];
	t_regex_ast *const	*alts = node->glob.groups;
	const char 			*rest;
	size_t 				i;
	int					r;

	i = 0;
	if (node->glob.op == '*' || node->glob.op == '+')
	{
		if (node->glob.op == '*')
		{
			r = match_seq(opts, groups + 1, ngroups - 1, s);
			if (r)
				return (r);
		}
		while (i++ < node->glob.ngroups)
			if (match_one(opts, alts[i - 1], s, &rest) == 2)
				return match_seq(opts, groups, ngroups, rest);
	}
	else if (node->glob.op == '@' || node->glob.op == '?')
	{
		if (node->glob.op == '?')
		{
			r = match_seq(opts, groups + 1, ngroups - 1, s);
			if (r) return r;
		}
		while (i++ < node->glob.ngroups)
			if (match_one(opts, alts[i - 1], s, &rest) == 2 &&
					(r = match_seq(opts, groups + 1, ngroups - 1, rest)))
				return (r);
	}
	else if (node->glob.op == '!')
	{
		int partial = 0;
		while (i++ < node->glob.ngroups)
		{
			r =  match_seq(opts, alts[i - 1]->compound.groups,
					alts[i - 1]->compound.ngroups, s);
			if (r == 2)
				return (0);
			else if (r == 1)
				partial = 1;
		}
		return (partial + !partial * 2);
	}
	return (0);
}

static int
	match_seq(const t_globopts *opts,
	          t_regex_ast **groups,
	          size_t ngroups,
	          const char *s)
{
	const t_regex_ast	*node;
	const char			*rest;
	size_t				i;
	int					r;

	if (ngroups == 0)
		return (!s[0] * 2);
	node = groups[0];
	i = 0;
	if (node->type == M_STAR || node->type == M_GLOBSTAR)
	{
		if (!opts->dotglob && s[0] == '.')
			return (0);
		while (++i)
		{
			r = match_seq(opts, groups + 1, ngroups - 1, s + i - 1);
			if (r) return (r);
			if (!s[i - 1] || (node->type == M_STAR && s[i - 1] == '/'))
				break;
		}
		return (!s[i - 1] * 2);
	}
	else if (node->type == M_EXTGLOB)
		return (match_extglob(opts, groups, ngroups, s));
	r = match_one(opts, node, s, &rest);
	if (r <= 1)
		return (r);
	if (!*rest && ngroups > 1)
		return (1);
	return (match_seq(opts, groups + 1, ngroups - 1, rest));
}

int
	regex_match(const t_globopts *opts, const t_regex *reg, const char *str)
{
	return (match_seq(opts, reg->expr->compound.groups, reg->expr->compound.ngroups, str));
}
