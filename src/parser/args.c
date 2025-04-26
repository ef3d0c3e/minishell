/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   args.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgamba <linogamba@pundalik.org>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 11:59:40 by lgamba            #+#    #+#             */
/*   Updated: 2025/03/17 11:59:41 by lgamba           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "ft_printf.h"
#include "parser/parser.h"
#include <shell/shell.h>

void
	arg_push(struct s_argument *arg, const t_token *token)
{
	arg->items = ft_realloc(arg->items, sizeof(arg->items[0]) * arg->nitems,
		sizeof(arg->items[0]) * (arg->nitems + 1));
	if (token->type == TOK_PARAM || token->type == TOK_PARAM_SIMPLE)
	{
		arg->items[arg->nitems] = (struct s_arg_item){
			.type = ARG_PARAMETER,
			.data = stringbuf_from_range(token->word.str, token->word.str
				+ token->word.len),
		};
	}
	else
	{
		arg->items[arg->nitems] = (struct s_arg_item){
			.type = ARG_LITERAL,
		};
		stringbuf_init(&arg->items[arg->nitems].data, 64);
		token_wordcontent(&arg->items[arg->nitems].data, token);
	}
	++arg->nitems;
}

void
	arg_free(struct s_argument *arg)
{
	size_t	i;

	i = 0;
	while (i < arg->nitems)
		stringbuf_free(&arg->items[i++].data);
	free(arg->items);
}

void
	arglist_free(struct s_argument *list, size_t size)
{
	size_t	i;

	i = 0;
	while (i < size)
		arg_free(&list[i++]);
	free(list);
}

void
	arg_print(size_t depth, struct s_argument *arg)
{
	size_t	i;

	i = 0;
	while (i++ < depth)
		ft_dprintf(2, " | ");
	i = 0;
	while (i < arg->nitems)
	{
		if (i)
			ft_dprintf(2, " ");
		if (arg->items[i].type == ARG_LITERAL)
			ft_dprintf(2, "'%.*s'",
				(int)arg->items[i].data.len, arg->items[i].data.str);
		else if (arg->items[i].type == ARG_PARAMETER)
			ft_dprintf(2, "{%.*s}",
				(int)arg->items[i].data.len, arg->items[i].data.str);

		++i;
	}
	ft_dprintf(2, "\n");
}

void
	arglist_print(size_t depth, struct s_argument *list, size_t size)
{
	size_t	i;

	i = 0;
	while (i < size)
	{
		arg_print(depth, &list[i]);
		++i;
	}
}
