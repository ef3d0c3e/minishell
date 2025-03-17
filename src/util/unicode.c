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
#include "util.h"

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
	if (it->byte_next >= it->str.len)
	{
		it->byte_pos = it->byte_next;
		return ((t_string){.str = NULL, .len = 0});
	}
	it->byte_pos = it->byte_next;
	it->codepoint.str = it->str.str + it->byte_pos;
	it->codepoint.len = u8_length(it->codepoint.str[0]);
	if (it->codepoint.len == 0)
		return ((t_string){.str = NULL, .len = 0});
	it->byte_next += it->codepoint.len;
	it->cp_pos++;
	return (it->codepoint);
}

t_string
	it_peek(const t_u8_iterator *it)
{
	t_string	codepoint;

	if (it->byte_next >= it->str.len)
	{
		return ((t_string){.str = NULL, .len = 0});
	}
	codepoint.str = it->str.str + it->byte_next;
	codepoint.len = u8_length(codepoint.str[0]);
	if (codepoint.len == 0)
		return ((t_string){.str = NULL, .len = 0});
	return (codepoint);
}
