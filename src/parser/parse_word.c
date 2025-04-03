/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_word.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgamba <linogamba@pundalik.org>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 11:59:40 by lgamba            #+#    #+#             */
/*   Updated: 2025/03/17 11:59:41 by lgamba           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "parser.h"

size_t	parse_word(
	t_parser *parser,
	size_t start,
	size_t end,
	t_string_buffer *buf)
{
	size_t	i;

	i = 0;
	buf->len = 0;
	buf->capacity = 0;
	buf->str = NULL;
	while (start + i < end && token_isword(parser->list.tokens[start + i].type))
	{
		if (buf->str)
			stringbuf_init(buf, 16);
		token_wordcontent(buf, &parser->list.tokens[start + i]);
		++i;
	}
	return (i);
}
