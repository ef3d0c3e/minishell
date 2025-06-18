/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   regex_util.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgamba <linogamba@pundalik.org>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 11:59:40 by lgamba            #+#    #+#             */
/*   Updated: 2025/03/17 11:59:41 by lgamba           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include <shell/shell.h>

/** @brief Recursively expand regex back to a string */
static int
	regex_path_expand(const t_regex_ast *node, t_string_buffer *buf)
{
	size_t	i;

	if (node->type == M_LITERAL)
		stringbuf_append(buf, (t_string){node->literal,
			ft_strlen(node->literal)});
	else if (node->type != M_SEQ)
		return (0);
	i = 0;
	while (i < node->compound.ngroups)
	{
		if (!regex_path_expand(node->compound.groups[i++], buf))
			break ;
	}
	return (1);
}

char
	*regex_path(const t_regex *regex)
{
	t_string_buffer	buf;

	stringbuf_init(&buf, 24);
	regex_path_expand(regex->expr, &buf);
	while (buf.len && buf.str[buf.len - 1] != '/')
		--buf.len;
	if (!buf.len)
		stringbuf_append(&buf, (t_string){".", 1});
	return (stringbuf_cstr(&buf));
}

static size_t
	regex_recurse_depth_group(const t_regex_ast *node)
{
	size_t	i;
	size_t	total;
	size_t	status;

	i = 0;
	total = 0;
	if (node->type == M_EXTGLOB)
	{
		while (i < node->glob.ngroups)
			total = max_sz(total, regex_recurse_depth(node->glob.groups[i++]));
	}
	else if (node->type == M_SEQ)
	{
		while (i < node->compound.ngroups)
		{
			status = regex_recurse_depth(node->compound.groups[i++]);
			if (status == (size_t) - 1)
				return (-1);
			total += status;
		}
	}
	return (total);
}

size_t
	regex_recurse_depth(const t_regex_ast *node)
{
	size_t	total;
	size_t	status;
	size_t	i;

	if (!node)
		return (0);
	total = 0;
	i = 0;
	if (node->type == M_LITERAL)
	{
		while (node->literal[i])
			total += node->literal[i++] == '/';
	}
	else if (node->type == M_EXTGLOB || node->type == M_SEQ)
	{
		status = regex_recurse_depth_group(node);
		if (status == (size_t) - 1)
			return (-1);
		total += status;
	}
	else if (node->type == M_GLOBSTAR)
		return (-1);
	return (total);
}
