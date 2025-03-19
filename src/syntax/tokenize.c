/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgamba <linogamba@pundalik.org>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 11:59:40 by lgamba            #+#    #+#             */
/*   Updated: 2025/03/17 11:59:41 by lgamba           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "tokenizer.h"
#include "util/util.h"
#include <stddef.h>
#include <stdio.h>

int	main(int argc, char **argv)
{
	t_tokenizer	t;
	const		t_string input = {
		.str = argv[1],
		.len = ft_strlen(argv[1]),
	};
	tokenizer_init(&t);
	printf("prompt=`%s`\n", argv[1]);

	t_token_list list = tokenizer_tokenize(&t, input);
	size_t i = 0;
	while (i < list.size)
	{
		token_print_debug(2, input, &list.tokens[i]);
		++i;
	}
	token_list_free(&list);
	tokenizer_free(&t);
}

int	token_space(t_token_list *list, t_u8_iterator *it);
int	token_newline(t_token_list *list, t_u8_iterator *it);
int	token_redir(t_token_list *list, t_u8_iterator *it);
int	token_digit(t_token_list *list, t_u8_iterator *it);
int	token_grouping(t_token_list *list, t_u8_iterator *it);
int	token_sequence(t_token_list *list, t_u8_iterator *it);
int	token_pipeline(t_token_list *list, t_u8_iterator *it);

void
	tokenizer_init(t_tokenizer *t)
{
	static t_tokenizer_fn	munchers[] = {
		token_space,
		token_newline,
		token_digit,
		token_redir,
		token_sequence,
		token_pipeline,
		token_grouping,
		NULL,
	};

	t->munchers = munchers;
}

void
	tokenizer_free(t_tokenizer *t)
{
}

t_token_list
	tokenizer_tokenize(const t_tokenizer *t, t_string prompt)
{
	t_token_list	list;
	t_u8_iterator	it;
	size_t			i;

	list.tokens = xmalloc(16 * sizeof(t_token));
	list.size = 0;
	list.capacity = 16;
	it = it_new(prompt);
	it_next(&it);
	while (it.codepoint.len)
	{
		i = 0;
		while (t->munchers[i])
		{
			if (t->munchers[i](&list, &it))
			{
				i = 0;
				break ;
			}
			++i;
		}
		if (i)
		{
			dprintf(2, "Leftover tokens: `%.*s`\n",
				(int)(prompt.len - it.byte_pos), prompt.str + it.byte_pos);
			break ;
		}
	}
	return (list);
}
