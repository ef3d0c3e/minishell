/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_operator.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgamba <linogamba@pundalik.org>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 11:59:40 by lgamba            #+#    #+#             */
/*   Updated: 2025/03/17 11:59:41 by lgamba           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include <tokenizer/tokenizer.h>

int
	token_operator(t_token_list *list, t_u8_iterator *it)
{
	static const char	*operators[] = {"||", "&&", NULL};
	const char			*operator;

	operator = str_alternatives(it_substr(it, 2), operators, 0);
	if (!operator)
		return (0);
	token_list_push(list, TOK_OPERATOR, it->byte_pos,
		it->byte_pos + ft_strlen(operator))->reserved_word = operator;
	it_advance(it, ft_strlen(operator));
	return (1);
}
