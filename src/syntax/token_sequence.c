/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_sequence.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgamba <linogamba@pundalik.org>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 11:59:40 by lgamba            #+#    #+#             */
/*   Updated: 2025/03/17 11:59:41 by lgamba           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "tokenizer.h"

int
	token_sequence(t_token_list *list, t_u8_iterator *it)
{
	static const char	*separators[] = {"&&", "||", "&", ";", NULL};
	const char			*sep = str_alternatives(it_substr(it, 2), separators);

	if (!sep)
		return (0);
	token_list_push(list, (t_token){
		.type = TOK_SEQUENCE,
		.start = it->byte_pos,
		.end = it->byte_pos + ft_strlen(sep),
		.reserved_word = sep
	});
	it_advance(it, ft_strlen(sep));
	return (1);
}
