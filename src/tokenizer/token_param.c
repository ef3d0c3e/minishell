/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_param.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgamba <linogamba@pundalik.org>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 11:59:40 by lgamba            #+#    #+#             */
/*   Updated: 2025/03/17 11:59:41 by lgamba           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include <tokenizer/tokenizer.h>

/** @brief Handles special parameters that will be resolved at evaluation
 * time */
static int
	param_special(t_token_list *list, t_u8_iterator *it, size_t end)
{
	static const char	*special[] = {
		"?", NULL
	};
	const char			*kind = str_alternatives(it_substr(it, 1), special, 0);

	if (kind && ft_strlen(kind) == end)
	{
		token_list_push(list, TOK_PARAM_SIMPLE, it->byte_pos,
			it->byte_pos + ft_strlen(kind))->word = stringbuf_from(kind);
		it_advance(it, ft_strlen(kind) + 1);
		return (1);
	}
	return (0);
}

int
	token_param(t_token_list *list, t_u8_iterator *it)
{
	size_t	end;

	if (str_cmp(it_substr(it, 2), "${"))
		return (0);
	it_advance(it, 2);
	end = find_unescaped(it_substr(it, -1), "}");
	if (end == (size_t)-1)
	{
		token_error(list, it->byte_pos - 2, it->byte_pos, "Unterminated `${` token");
		return (1);
	}
	if (param_special(list, it, end))
		return (1);
	token_list_push(list, TOK_PARAM, it->byte_pos, it->byte_pos + end)->word 
		= stringbuf_from_range(it->str.str + it->byte_pos,
		it->str.str + it->byte_pos + end);
	it_advance(it, end + 1);
	return (1);
}

