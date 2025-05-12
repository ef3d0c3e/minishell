/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prompt.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgamba <linogamba@pundalik.org>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 11:59:40 by lgamba            #+#    #+#             */
/*   Updated: 2025/03/17 11:59:41 by lgamba           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include <shell/shell.h>

void
	getline_set_prompt(t_getline *line, const char *text)
{
	const size_t	len = ft_strlen(text);
	t_u8_iterator	it;
	t_u8_iterator	it2;

	line->prompt = getline_buffer_new();
	// TODO: CSI Handling -> Push to attr buffer
	stringbuf_init(&line->prompt.buffer, len);
	it = it_new((t_string){text, len});
	it_next(&it);
	while (it.codepoint.len)
	{
		stringbuf_append(&line->prompt.buffer, it.codepoint);
		it2 = it_new((t_string){line->prompt.buffer.str,
			line->prompt.buffer.len});
		it_next(&it2);
		it_advance(&it2, it.byte_pos);
		getline_recluster(line, &line->prompt, it2);
		it_next(&it);
	}
}
