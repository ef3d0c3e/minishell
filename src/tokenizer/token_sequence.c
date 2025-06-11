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
#include <tokenizer/tokenizer.h>

int
	token_sequence(t_token_list *list, t_u8_iterator *it)
{
	const size_t		start = it->byte_pos;
	static const char	*separators[] = {"\n", "&", ";", NULL};
	const char			*sep = str_alternatives(it_substr(it, 1), separators,
			0);

	if (!sep)
		return (0);
	it_advance(it, ft_strlen(sep));
	while (it->codepoint.len == 1 && it->codepoint.str[0] == '\n')
		it_next(it);
	token_list_push(list,
		TOK_SEQUENCE, start, it->byte_pos)->reserved_word = sep;
	return (1);
}
