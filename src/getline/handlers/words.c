/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   words.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgamba <linogamba@pundalik.org>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 11:59:40 by lgamba            #+#    #+#             */
/*   Updated: 2025/03/17 11:59:41 by lgamba           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include <shell/shell.h>

t_u8_iterator
	getline_handler_word_boundaries(
	t_getline *line,
	t_u8_iterator it,
	int direction)
{
	static const char	*delims[] = {" ", "\t", "\n", NULL};

	if (direction < 0)
	{
		it_prev(&it);
		while (it.codepoint.len && str_alternatives(it.codepoint, delims, 0))
			it_prev(&it);
		while (it.codepoint.len && !str_alternatives(it.codepoint, delims, 0))
			it_prev(&it);
		if (it.codepoint.len)
		it_next(&it);
	}
	else
	{
		while (it.codepoint.len && str_alternatives(it.codepoint, delims, 0))
			it_next(&it);
		while (it.codepoint.len && !str_alternatives(it.codepoint, delims, 0))
			it_next(&it);
	}
	return (it);
}
