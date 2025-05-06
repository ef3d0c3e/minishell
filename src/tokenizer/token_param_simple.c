/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_param_simple.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgamba <linogamba@pundalik.org>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 11:59:40 by lgamba            #+#    #+#             */
/*   Updated: 2025/03/17 11:59:41 by lgamba           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include <tokenizer/tokenizer.h>

int
	is_valid_identifier(const char *ident)
{
	if (!ident[0] || !is_param_ident_start(ident[0]))
		return (0);
	++ident;
	while (*ident && is_param_ident(*ident))
		++ident;
	return (!*ident);
}

int
	is_param_ident(char c)
{
	return (c == '_'
		|| (c >= '0' && c <= '9')
		|| (c >= 'A' && c <= 'Z')
		|| (c >= 'a' && c <= 'z'));
}

int
	is_param_ident_start(char c)
{
	return (c == '_'
		|| (c >= 'A' && c <= 'Z')
		|| (c >= 'a' && c <= 'z'));
}

static int
	handle_special_param(t_token_list *list, t_u8_iterator *it, t_string *next)
{
	const size_t	start = it->byte_pos;
	const char		c = next->str[1];

	if (c != '#' && c != '@' && c != '*' && c != '$' && c != '-' && c != '?')
		return (0);
	it_advance(it, 2);
	token_list_push(list, TOK_PARAM_SIMPLE, start, it->byte_pos)->word
		= stringbuf_from_range(it->str.str + start + 1,
		it->str.str + it->byte_pos);
	return (1);
}

int
	token_param_simple(t_token_list *list, t_u8_iterator *it)
{
	const size_t	start = it->byte_pos;
	t_string		next;
	t_string_buffer	buf;

	if (it->codepoint.str[0] != '$')
		return (0);
	next = it_substr(it, 2);
	if (next.len != 2)
		return (0);
	if (handle_special_param(list, it, &next))
		return (1);
	else if (!is_param_ident(next.str[1]))
		return (0);
	it_advance(it, 2);
	stringbuf_init(&buf, 64);
	stringbuf_append(&buf, (t_string){next.str + 1, 1});
	while (it->codepoint.len == 1 && is_param_ident(it->codepoint.str[0]))
	{
		stringbuf_append(&buf, it->codepoint);
		it_next(it);
	}
	token_list_push(list, TOK_PARAM_SIMPLE, start, it->byte_pos)->word
		= buf;
	return (1);
}

