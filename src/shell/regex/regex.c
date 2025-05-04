/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   regex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgamba <linogamba@pundalik.org>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 11:59:40 by lgamba            #+#    #+#             */
/*   Updated: 2025/03/17 11:59:41 by lgamba           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "ft_printf.h"
#include <shell/shell.h>
#include <stdio.h>

void
	regex_free(t_regex_ast *node)
{
	size_t	i;

	i = 0;
	if (!node)
		return ;
	if (node->type == M_LITERAL)
		free(node->literal);
	else if (node->type == M_EXTGLOB)
	{
		while (i < node->glob.ngroups)
			regex_free(node->glob.groups[i++]);
		free(node->glob.groups);
	}
	else if (node->type == M_SEQ || node->type == M_ALT)
	{
		while (i < node->compound.ngroups)
			regex_free(node->compound.groups[i++]);
		free(node->compound.groups);
	}
	free(node);
}

t_regex_ast
	*regex_new(enum e_match_type type)
{
	t_regex_ast	*node;

	node = xmalloc(sizeof(t_regex_ast));
	ft_memset(node, 0, sizeof(t_regex_ast));
	node->type = type;
	return (node);
}

static void
	print_match_type(const t_regex_ast *node)
{
	static const char	*names[] = {
	[M_LITERAL] = "LITERAL",
	[M_ANY] = "ANY",
	[M_STAR] = "STAR",
	[M_EXTGLOB] = "EXTGLOB",
	[M_GLOBSTAR] = "GLOBSTAR",
	[M_SEQ] = "SEQ",
	[M_ALT] = "ALT",
	};

	ft_dprintf(2, "%s", names[node->type]);
}

void
	regex_print(size_t depth, const t_regex_ast *node)
{
	size_t	i;

	i = 0;
	if (!node)
		return ;
	print_pad(" | ", depth);
	print_match_type(node);
	if (node->type == M_LITERAL)
		ft_dprintf(2, " '%s'\n", node->literal);
	else if (node->type == M_EXTGLOB)
	{
		ft_dprintf(2, " %c [%zu]\n", node->glob.op, node->glob.ngroups);
		while (i < node->glob.ngroups)
			regex_print(depth + 1, node->glob.groups[i++]);
	}
	else if (node->type == M_SEQ || node->type == M_ALT)
	{
		ft_dprintf(2, " [%zu]\n", node->compound.ngroups);
		while (i < node->compound.ngroups)
			regex_print(depth + 1, node->compound.groups[i++]);
	}
	else
		ft_dprintf(2, "\n");
}
