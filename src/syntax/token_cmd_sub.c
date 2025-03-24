/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_cmd_sub.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgamba <linogamba@pundalik.org>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 11:59:40 by lgamba            #+#    #+#             */
/*   Updated: 2025/03/17 11:59:41 by lgamba           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "tokenizer.h"

int
	token_cmd_sub(t_token_list *list, t_u8_iterator *it)
{
	size_t	end;

	if (str_cmp(it_substr(it, 2), "$("))
		return (0);
	it_advance(it, 2);
	end = find_matching(it_substr(it, -1), "(", ")");
	if (end == (size_t)-1)
	{
		token_error(list, it->byte_pos - 2, it->byte_pos, "Unterminated `$(` token");
		return (1);
	}
	token_list_push(list, (t_token){
		.type = TOK_CMD_SUB,
		.start = it->byte_pos,
		.end = it->byte_pos + end,
	});
	it_advance(it, end + 1);
	return (1);
}
