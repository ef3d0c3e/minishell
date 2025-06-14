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
#include <tokenizer/tokenizer.h>

void
	token_list_init(t_token_list *list, size_t initial_capacity)
{
	list->size = 0;
	list->capacity = initial_capacity;
	list->tokens = xmalloc(sizeof(t_token) * list->capacity);
}

void
	token_list_push_token(t_token_list *list, t_token token)
{
	if (list->size >= list->capacity)
	{
		list->capacity = ((list->capacity + !list->capacity) * 2);
		list->tokens = ft_realloc(list->tokens, list->size * sizeof(t_token),
			list->capacity * sizeof(t_token));
	}
	list->tokens[list->size++] = token;
}

t_token
	*token_list_push(
	t_token_list *list,
	int type,
	size_t start,
	size_t end)
{
	if (list->size >= list->capacity)
	{
		list->capacity = ((list->capacity + !list->capacity) * 2);
		list->tokens = ft_realloc(list->tokens, list->size * sizeof(t_token),
			list->capacity * sizeof(t_token));
	}
	list->tokens[list->size++] = (t_token){
		.type = type,
		.start = start,
		.end = end,
		.flags = FL_NONE,
	};
	if (type == TOK_SINGLE_QUOTE)
		list->tokens[list->size - 1].flags |= FL_SQUOTED;

	return (&list->tokens[list->size - 1]);
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
	list->tokens = NULL;
	list->size = 0;
	list->capacity = 0;
}

void
	token_list_print(t_string input, const t_token_list *list)
{
	size_t	i;

	i = 0;
	while (i < list->size)
	{
		token_print_debug(input, &list->tokens[i]);
		++i;
	}
	ft_dprintf(2, "\n");
}
