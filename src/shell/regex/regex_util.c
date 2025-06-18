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
static void
	regex_path_expand(const t_regex_ast *node, t_string_buffer *buf)
{
	size_t	i;

	if (node->type == M_LITERAL)
		stringbuf_append(buf, (t_string){node->literal,
			ft_strlen(node->literal)});
	if (node->type != M_SEQ)
		return ;
	i = 0;
	while (i < node->compound.ngroups)
		regex_path_expand(node->compound.groups[i++], buf);
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
