/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_double_quote.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgamba <linogamba@pundalik.org>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 11:59:40 by lgamba            #+#    #+#             */
/*   Updated: 2025/03/17 11:59:41 by lgamba           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include <tokenizer/tokenizer.h>

static inline const t_tokenizer_fn
	*quoted_munchers(void)
{
	static const t_tokenizer_fn	munchers[] = {
		token_arith,
		token_cmd_sub,
		token_param,
		token_param_simple,
		NULL,
	};
	
	return (munchers);
}

static void
	token_list_debug(t_string input, const t_token_list *list)
{
	size_t	i;

	i = 0;
	while (i < list->size)
	{
		token_print_debug(2, input, &list->tokens[i]);
		++i;
	}
	ft_dprintf(2, "\n");
}

static void
	expand_dquote(t_token_list *list, t_string quoted, size_t offset)
{
	t_token_list	inner;
	t_u8_iterator	it;
	size_t			i;

	i = 0;
	inner.tokens = xmalloc(16 * sizeof(t_token));
	inner.size = 0;
	inner.capacity = 16;
	it = it_new(quoted);
	it_next(&it);
	while (it.codepoint.len)
	{
		i = 0;
		while (quoted_munchers()[i])
		{
			if (quoted_munchers()[i](&inner, &it))
			{
				i = 0;
				break ;
			}
			++i;
		}
		if (i)
		{
			list_push_codepoint(&inner, &it);
			it_next(&it);
		}
	}
	list_extend(list, &inner, offset);
}

int
	token_double_quote(t_token_list *list, t_u8_iterator *it)
{
	size_t			end;

	if (it->codepoint.str[0] != '"')
		return (0);
	it_advance(it, 1);
	end = find_unescaped(it_substr(it, -1), "\"");
	if (end == (size_t)-1)
	{
		token_error(list, it->byte_pos-1, it->byte_pos, "Unterminated `\"` token");
		return (1);
	}
	expand_dquote(list, it_substr(it, end), it->byte_pos);
	it_advance(it, end + 1);
	return (1);
}
