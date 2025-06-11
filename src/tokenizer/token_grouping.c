/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_grouping.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgamba <linogamba@pundalik.org>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 11:59:40 by lgamba            #+#    #+#             */
/*   Updated: 2025/03/17 11:59:41 by lgamba           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include <tokenizer/tokenizer.h>

// https://www.gnu.org/software/bash/manual/bash.html#Redirections

int
	token_grouping(t_token_list *list, t_u8_iterator *it)
{
	static const char	*groupings[] = {"(", ")", NULL};
	const char			*grouping;

	grouping = str_alternatives(it_substr(it, 1), groupings, 0);
	if (!grouping)
		return (0);
	token_list_push(list, TOK_GROUPING, it->byte_pos,
		it->byte_pos + ft_strlen(grouping))->reserved_word = grouping;
	it_advance(it, ft_strlen(grouping));
	return (1);
}
