/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgamba <linogamba@pundalik.org>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 11:59:40 by lgamba            #+#    #+#             */
/*   Updated: 2025/03/17 11:59:41 by lgamba           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "expansion.h"

// TODO: Expansion
t_token_list
	token_expand(t_environ *env, t_token_list list)
{
	t_token_list	new;
	size_t			i;

	new.tokens = xmalloc(list.size * sizeof(t_token));
	new.size = 0;
	new.capacity = list.size;
	i = 0;
	while (i < list.size)
	{
		if (list.tokens[i].type == TOK_SPACE && i && i + 1 < list.size)
		{
			if (!token_isword(list.tokens[i - 1].type)
				|| !token_isword(list.tokens[i + 1].type))
			{
				++i;
				continue;
			}
		}
		token_list_push(&new, list.tokens[i]);
		++i;
	}
	free(list.tokens);
	return (new);
}
