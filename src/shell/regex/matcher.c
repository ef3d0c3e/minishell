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
#include "shell/regex/regex.h"
#include <shell/shell.h>

static int
	ascii_eq(const t_globopts *opts, char l, char r)
{
	if (!opts->nocaseglob || (unsigned char)l > 127 || (unsigned char)r > 127)
		return (l == r);
	if (l >= 'A' && l <= 'Z')
		l -= 32;
	if (r >= 'A' && r <= 'Z')
		r -= 32;
	return (l == r);
}

static int
match_seq(const t_globopts *opts, t_regex_ast **groups, size_t ngroups, const char *s);

/**
 * Match a single AST node starting at s.
 * On success, sets *s_next to the next unmatched character.
 */
int
	match_one(const t_globopts *opts, const t_regex_ast *node, const char *s, const char **s_next)
{
	const char	*tmp;
	size_t		i;
	size_t		len;

	i = 0;
	if (node->type == M_LITERAL)
	{
		len = ft_strlen(node->literal);
		while (i < len)
		{
			if (!s[i] || !ascii_eq(opts, s[i], node->literal[i]))
				return (0);
			++i;
		}
		return (*s_next = s + len, 1);
	}
	else if (node->type == M_ANY)
	{
		if (!s[0] || (!opts->dotglob && s[0] == '.'))
			return (0);
		return (*s_next = s + 1, 1);
	}
	else if (node->type == M_SEQ)
	{
		if (!match_seq(opts, node->compound.groups, node->compound.ngroups, s))
			return (0);
		*s_next = s;
		while (**s_next)
		{
			if (!match_one(opts, node, *s_next, &tmp))
				break ;
			*s_next = tmp;
		}
		return (1);
	}
	else if (node->type == M_ALT)
	{
		while (i < node->compound.ngroups)
		{
			if (match_one(opts, node->compound.groups[i], s, s_next))
				return (1);
			++i;
		}
	}
	return (0);
}

static int
	match_extglob(const t_globopts *opts, t_regex_ast **groups, size_t ngroups, const char *s)
{
	const t_regex_ast	*node = groups[0];
	t_regex_ast **const	alts = node->glob.groups;
	const size_t		nalts = node->glob.ngroups;
	const char			*rest_s;
	size_t				i;

	i = 0;
	if (node->glob.op == '+' || node->glob.op == '*')
	{
		if (node->glob.op == '*' && match_seq(opts, groups + 1, ngroups - 1, s))
			return (1);
		while (i < nalts)
		{
			if (match_one(opts, alts[i], s, &rest_s)
				&& match_seq(opts, groups, ngroups, rest_s))
				return (1);
			++i;
		}
	}
	else if (node->glob.op == '@' || node->glob.op == '?')
	{
		if (node->glob.op == '?' && match_seq(opts, groups + 1, ngroups - 1, s))
			return (1);
		while (i < nalts)
		{
			if (match_one(opts, alts[i], s, &rest_s)
				&& match_seq(opts, groups + 1, ngroups - 1, rest_s))
				return (1);
			++i;
		}
	}
	else if (node->glob.op == '!')
	{
		while (i < nalts)
		{
			if (match_one(opts, alts[i], s, &rest_s))
				return (0);
			++i;
		}
		return (s[0] && match_seq(opts, groups + 1, ngroups - 1, s + 1));
	}
	return (0);
}

static int
	match_seq(const t_globopts *opts, t_regex_ast **groups, size_t ngroups, const char *s)
{
	const t_regex_ast	*node;
	size_t				i;
	const char			*rest_s;

	if (ngroups == 0)
		return (!s[0]);
	node = groups[0];
	if (node->type == M_STAR || node->type == M_GLOBSTAR)
	{
		if (!opts->dotglob && s[0] == '.')
			return (0);
		i = 0;
		while (1)
		{
			if (match_seq(opts, groups + 1, ngroups - 1, s + i))
				return (1);
			if (!s[i] || (s[i] == '/' && node->type == M_STAR))
				break ;
			++i;
		}
		return (0);
	}
	else if (node->type == M_EXTGLOB)
		return (match_extglob(opts, groups, ngroups, s));
	else
		return (match_one(opts, node, s, &rest_s)
				&& match_seq(opts, groups + 1, ngroups - 1, rest_s));
}

int
	regex_match(const t_globopts *opts, const t_regex *reg, const char *str)
{
	return (match_seq(opts, reg->expr->compound.groups, reg->expr->compound.ngroups, str));
}

int main(int ac, char **av, const char **envp)
{
	t_globopts opts = {
		.extglob = 1,
		.globstar = 1,
		.dotglob = 1,
		.nocaseglob = 0,
		.nullglob = 1,
		.failglob = 1,
	};
	t_regex_builder builder = regex_builder_new();
	regex_builder_literal(&opts, &builder, "f");
	regex_builder_expr(&opts, &builder, av[1]);

	t_regex reg = builder.regex;
	regex_print(0, reg.expr);

	static const char *tests[] = {"foo", ".foo", "bar", ".bar/", NULL};
	size_t i = 0;
	while (tests[i])
	{
		ft_printf("'%s': [%d]\n", tests[i], regex_match(&opts, &reg, tests[i]));
		++i;
	}

	regex_free(reg.expr);
	return (0);
}
