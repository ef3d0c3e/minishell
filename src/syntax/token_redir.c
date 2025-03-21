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

int
	match_simple(
		t_token_list *list,
		t_u8_iterator *it,
		struct s_token_redir *redir)
{
	if (str_cmp(it_substr(it, 3), "&>>"))
		redir->type = REDIR_OUTPUT_AND_ERR;
	else if (str_cmp(it_substr(it, 2), "&>"))
		redir->type = REDIR_OUTPUT_AND_ERR;
	else if (str_cmp(it_substr(it, 2), "<&"))
	{
		redir->type = REDIR_INPUT;
		redir->duplicate = 1;
	}
	else if (str_cmp(it_substr(it, 2), ">&"))
	{
		redir->type = REDIR_OUTPUT;
		redir->duplicate = 1;
	}
	else if (str_cmp(it_substr(it, 2), ">>"))
	{
		redir->type = REDIR_OUTPUT;
		redir->append = 1;
	}
	else if (str_cmp(it_substr(it, 1), ">|"))
	{
		redir->type = REDIR_OUTPUT;
		redir->clobber = 1;
	}
	else if (str_cmp(it_substr(it, 1), ">"))
		redir->type = REDIR_OUTPUT;
	else if (str_cmp(it_substr(it, 1), "<"))
		redir->type = REDIR_INPUT;
	else
		return (0);
	return (1);
}

int
	token_redir(t_token_list *list, t_u8_iterator *it)
{
	static const char		*redirs[] = {"<<<", "<<-", "&>>", "<<", ">>", "&>",
										"<>", ">&", "<&", ">|", ">", "<", NULL};
	const char				*redir;
	struct s_token_redir	data;

	redir = str_alternatives(it_substr(it, 3), redirs);
	if (!redir)
		return (0);
	data.append = 0;
	data.clobber = 0;
	data.move = 0;
	data.duplicate = 0;
	// TODO: Heredoc/herestrings
	match_simple(list, it, &data);
	token_list_push(list, (t_token){
		.type = TOK_REDIR,
		.start = it->byte_pos,
		.end = it->byte_pos + ft_strlen(redir),
		.redir = data,
	});
	it_advance(it, ft_strlen(redir));
	return (1);
}
