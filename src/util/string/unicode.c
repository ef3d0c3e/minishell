/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unicode.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgamba <linogamba@pundalik.org>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 11:59:40 by lgamba            #+#    #+#             */
/*   Updated: 2025/03/17 11:59:41 by lgamba           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "../util.h"

t_u8_iterator
	it_new(t_string str)
{
	return ((t_u8_iterator){
		.str = str,
		.codepoint = (t_string){.str = NULL, .len = 0},
		.byte_pos = 0,
		.byte_next = 0,
		.cp_pos = 0,
	});
}

t_string
	it_next(t_u8_iterator *it)
{
	if (it->codepoint.len)
		++it->cp_pos;
	if (it->byte_next >= it->str.len)
	{
		it->byte_pos = it->byte_next;
		it->codepoint.str = NULL;
		it->codepoint.len = 0;
		return (it->codepoint);
	}
	it->byte_pos = it->byte_next;
	it->codepoint.str = it->str.str + it->byte_pos;
	it->codepoint.len = min_sz(u8_length(it->codepoint.str[0]),
			it->str.len - it->byte_pos);
	if (it->codepoint.len == 0)
		return ((t_string){.str = NULL, .len = 0});
	it->byte_next += it->codepoint.len;
	if (u8_to_cp(it->codepoint) == 0xFFFD)
	{
		it->byte_pos = it->byte_next;
		it->codepoint.str = NULL;
		it->codepoint.len = 0;
	}
	return (it->codepoint);
}

t_string
	it_prev(t_u8_iterator *it)
{
	size_t			pos;
	size_t			len;
	unsigned char	c;

	if (it->byte_pos == 0)
	{
		it->codepoint.str = NULL;
		it->codepoint.len = 0;
		return (it->codepoint);
	}
	it->byte_next = it->byte_pos;
	pos = it->byte_pos - 1;
	while (pos > 0 && (it->str.str[pos] & 0xC0) == 0x80)
		pos--;
	c = (unsigned char)it->str.str[pos];
	len = u8_length(c);
	if (len == 0 || pos + len != it->byte_next)
	{
		it->codepoint.str = NULL;
		it->codepoint.len = 0;
		return (it->codepoint);
	}
	it->byte_pos = pos;
	it->codepoint.str = it->str.str + pos;
	it->codepoint.len = len;
	it->cp_pos--;
	if (u8_to_cp(it->codepoint) == 0xFFFD)
	{
		it->byte_pos = it->byte_next;
		it->codepoint.str = NULL;
		it->codepoint.len = 0;
	}
	return (it->codepoint);
}

void
	it_advance(t_u8_iterator *it, size_t num)
{
	t_string		cp;
	const size_t	start = it->byte_pos;

	cp.len = 1;
	while (cp.len && it->byte_pos < start + num)
		cp = it_next(it);
}

t_string
	it_substr(const t_u8_iterator *it, size_t len)
{
	if (len == (size_t)-1 || it->byte_pos + len > it->str.len)
		return ((t_string){
			.str = it->str.str + it->byte_pos,
			.len = it->str.len - it->byte_pos
		});
	return ((t_string){
		.str = it->str.str + it->byte_pos,
		.len = len,
	});
}
