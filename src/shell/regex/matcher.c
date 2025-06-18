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

int
	regex_match_one(
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
		{
			if (!s[i] || !ascii_eq(opts, s[i], node->literal[i]))
				return (!s[i]);
			else
				++i;
		}
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

int
	regex_match(const t_globopts *opts, const t_regex *reg, const char *str)
{
	return (regex_match_seq(opts, reg->expr->compound.groups,
			reg->expr->compound.ngroups, str));
}
