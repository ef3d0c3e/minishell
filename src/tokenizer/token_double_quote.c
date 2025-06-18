/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_double_quote.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgamba <linogamba@pundalik.org>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 11:59:40 by lgamba            #+#    #+#             */
/*   Updated: 2025/06/18 08:26:37 by thschnei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "util/util.h"
#include <stddef.h>
#include <tokenizer/tokenizer.h>

/** @brief Token munchers for the content of a double quoted string */
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

/** @brief Handles `\` character in double quotes: a '\' followed by '`', '$',
 * '"', '\' or '\n' is removed */
static void
	dquote_backslash(t_u8_iterator *it)
{
	static const char	*escaped[] = {
		"\\`", "\\$", "\\\"", "\\\\", "\\\n", NULL
	};

	if (it->codepoint.str[0] != '\\')
		return ;
	if (str_alternatives(it_substr(it, 2), escaped, 0))
		it_advance(it, 1);
}

static void
	expand_dquote_loop(t_u8_iterator *it, t_token_list *inner)
{
	size_t	i;

	while (it->codepoint.len)
	{
		i = 0;
		while (quoted_munchers()[i])
		{
			if (quoted_munchers()[i](inner, it))
			{
				i = 0;
				break ;
			}
			++i;
		}
		if (!i)
			continue ;
		dquote_backslash(it);
		list_push_codepoint(inner, it);
		it_next(it);
	}
}

/** @brief Tokenize the inside of a double quoted string */
static void
	expand_dquote(t_token_list *list, t_string quoted, size_t offset)
{
	t_token_list	inner;
	t_u8_iterator	it;
	size_t			i;

	i = 0;
	token_list_init(&inner, 16);
	it = it_new(quoted);
	it_next(&it);
	expand_dquote_loop(&it, &inner);
	i = 0;
	while (i < inner.size)
	{
		inner.tokens[i].flags |= FL_DQUOTED;
		++i;
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
		token_error(list, it->byte_pos - 1, it->byte_pos,
			"Unterminated `\"` token");
		return (1);
	}
	expand_dquote(list, it_substr(it, end), it->byte_pos);
	it_advance(it, end + 1);
	return (1);
}
