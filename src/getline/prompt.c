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

static size_t
	handle_escape(t_getline *line, t_u8_iterator *it)
{
	const t_string	left = it_substr(it, SIZE_MAX);
	const size_t	len = getline_escape_len(left.str, left.len);
	// TOOD: Parse escape sequence...
	it_advance(it, len);
	return (len);
}

void
	getline_set_prompt(t_getline *line, const char *text)
{
	const size_t	len = ft_strlen(text);
	size_t			offset;
	t_u8_iterator	it;
	t_u8_iterator	it2;

	line->prompt = getline_buffer_new();
	stringbuf_init(&line->prompt.buffer, len);
	it = it_new((t_string){text, len});
	it_next(&it);
	offset = 0;
	while (1)
	{
		if (it.codepoint.len == 1 && (unsigned char)it.codepoint.str[0] == '\x1b')
			offset += handle_escape(line, &it);
		if (!it.codepoint.len)
			break ;
		stringbuf_append(&line->prompt.buffer, it.codepoint);
		it2 = it_new((t_string){line->prompt.buffer.str,
			line->prompt.buffer.len});
		it_next(&it2);
		it_advance(&it2, it.byte_pos - offset);
		getline_recluster(line, &line->prompt, it2);
		it_next(&it);
	}
}
