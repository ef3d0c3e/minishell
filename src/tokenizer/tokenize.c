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


static inline const t_tokenizer_fn
	*munchers(void)
{
	static const t_tokenizer_fn	munchers[] = {
		token_space,
		token_digit,
		token_minus,
		token_redir,
		token_operator,
		token_sequence,
		token_pipeline,
		token_grouping,
		token_keyword,
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

t_token_list
	tokenizer_tokenize(t_string prompt)
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
		while (munchers()[i])
		{
			if (munchers()[i](&list, &it))
			{
				i = 0;
				break ;
			}
			++i;
		}
		if (i)
		{
			ft_dprintf(2, "Leftover tokens: `%.*s`\n",
				(int)(prompt.len - it.byte_pos), prompt.str + it.byte_pos);
			break ;
		}
	}
	return (list);
}
