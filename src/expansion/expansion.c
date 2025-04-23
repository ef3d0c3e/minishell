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
#include "tokenizer/tokenizer.h"
#include <expansion/expansion.h>

/** @brief Removes spaces before and after operators */
static int
	remove_space(const t_token_list *list, size_t *i)
{
	if (list->tokens[*i].type == TOK_SPACE
		&& ((*i && token_precedence(&list->tokens[*i - 1]) >= 0)
		|| (*i + 1 < list->size
		&& token_precedence(&list->tokens[*i + 1]) >= 0)))
	{
		++(*i);
		return (1);
	}
	return (0);
}

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
		if (remove_space(&list, &i))
			continue ;
		if (expand_tilde(env, &list.tokens[i], &new))
			;
		else if (expand_param(env, &list.tokens[i], &new))
			;
		else if (expand_cmdsub(env, &list.tokens[i], &new))
			;
		else if (expand_filename(env, &list.tokens[i], &new))
			;
		else
			token_list_push(&new, list.tokens[i]);
		++i;
	}
	// TODO: Join tokens
	free(list.tokens);
	return (new);
}
