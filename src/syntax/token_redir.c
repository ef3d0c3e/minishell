/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_redir.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgamba <linogamba@pundalik.org>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 11:59:40 by lgamba            #+#    #+#             */
/*   Updated: 2025/03/17 11:59:41 by lgamba           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "tokenizer.h"

// TODO: This function should handle `[n]>` syntax
int
	token_redir(t_token_list *list, t_u8_iterator *it)
{
	static const char	*redirs[] = {"<<", ">>", ">|", "<", ">", NULL};
	const char			*redir;

	if (it->codepoint.str[0] != '<' && it->codepoint.str[0] != '>')
		return (0);
	redir = str_alternatives(it_substr(it, 2), redirs);
	token_list_push(list, (t_token){
		.type = TOK_REDIR,
		.start = it->byte_pos,
		.end = it->byte_pos + ft_strlen(redir),
		.redir = {
		.input = it->codepoint.str[0] == '<',
		.output = it->codepoint.str[0] == '>',
		.bypass_noclobber = redir == redirs[2],
		.append_flag = redir == redirs[0] || redir == redirs[1],
	}});
	it_advance(it, ft_strlen(redir));
	return (1);
}
