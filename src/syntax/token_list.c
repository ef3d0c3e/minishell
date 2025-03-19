/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_list.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgamba <linogamba@pundalik.org>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 11:59:40 by lgamba            #+#    #+#             */
/*   Updated: 2025/03/17 11:59:41 by lgamba           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "tokenizer.h"
#include <stddef.h>

void
	token_list_push(t_token_list *list, t_token token)
{
	if (list->size >= list->capacity)
	{
		list->capacity = ((list->capacity + !list->capacity) * 2);
		list->tokens = ft_realloc(list->tokens, list->size * sizeof(t_token),
			list->capacity * sizeof(t_token));
	}
	list->tokens[list->size++] = token;
}

void
	token_list_free(t_token_list *list)
{
	size_t	i;

	i = 0;
	while (i < list->size)
	{
		token_free(&list->tokens[i]);
		++i;
	}
	free(list->tokens);
}
