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

// TODO: Heredoc/herestrings read from stdin
int
	token_redir(t_token_list *list, t_u8_iterator *it)
{
	static const char		*redirs[] = {"<<<", "&>>", "<<-", "&>>", "<&",
		">&", "&>", "<>", ">|", "|&", ">>", "<<", "<", ">", NULL};
	const char				*redir;

	redir = str_alternatives(it_substr(it, 3), redirs, 0);
	if (!redir)
		return (0);
	token_list_push(list, TOK_REDIR, it->byte_pos,
		it->byte_pos + ft_strlen(redir))->reserved_word = redir;
	it_advance(it, ft_strlen(redir));
	return (1);
}
