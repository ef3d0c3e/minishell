/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgamba <linogamba@pundalik.org>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 11:59:40 by lgamba            #+#    #+#             */
/*   Updated: 2025/06/18 08:37:27 by thschnei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "tokenizer.h"
#include "util/util.h"
#include <stddef.h>

static inline const t_tokenizer_fn
	*munchers(void)
{
	static const t_tokenizer_fn	munchers[] = {
		token_comment,
		token_space,
		token_digit,
		token_minus,
		token_operator,
		token_pipeline,
		token_redir,
		token_sequence,
		token_assign,
		token_keyword,
		token_grouping,
		token_double_quote,
		token_single_quote,
		token_arith,
		token_cmd_sub,
		token_param,
		token_param_simple,
		token_word,
		NULL,
	};

	return (munchers);
}

static void
	tokenizer_tokenize_loop(t_string prompt, t_token_list *list,
			t_u8_iterator *it)
{
	size_t	i;

	while (it->codepoint.len)
	{
		i = 0;
		while (munchers()[i])
		{
			if (munchers()[i](list, it))
			{
				i = 0;
				break ;
			}
			++i;
		}
		if (i)
		{
			ft_dprintf(2, "Leftover tokens: `%.*s`\n",
				(int)(prompt.len - it->byte_pos), prompt.str + it->byte_pos);
			break ;
		}
	}
}

t_token_list
	tokenizer_tokenize(t_string prompt)
{
	t_token_list	list;
	t_u8_iterator	it;

	list.tokens = xmalloc(16 * sizeof(t_token));
	list.size = 0;
	list.capacity = 16;
	it = it_new(prompt);
	it_next(&it);
	tokenizer_tokenize_loop(prompt, &list, &it);
	return (list);
}
