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
#include "ft_printf.h"
#include "util/util.h"
#include <shell/shell.h>

static void
	parse_color(t_string str, int *color)
{
	size_t	pos;

	*color = -1;
	if (str.len <= 2 || str.str[1] != '[')
		return ;
	pos = 2;
	*color = 0;
	while (pos < str.len && str.str[pos] >= '0' && str.str[pos] <= '9')
	{
		*color = *color * 10 + str.str[pos] - '0';
		++pos;
	}
	if (pos + 1 != str.len || str.str[pos] != 'm')
		*color = -1;
}

static size_t
	handle_escape(t_u8_iterator *it, int *color)
{
	const t_string	left = it_substr(it, SIZE_MAX);
	const size_t	len = getline_escape_len(left.str, left.len);

	if (!left.len || (unsigned char)left.str[0] != '\x1b')
		return (0);
	parse_color(it_substr(it, len), color);
	it_advance(it, len);
	return (len);
}

static void
	apply_color(t_buffer *buf, int color, t_u8_iterator *it, size_t offset)
{
	static const int	map[] = {0x00000, 0xCD3131, 0x0DBC79, 0xE5E510,
	0x2472C8, 0xBC3FBC, 0x11A8CD, 0xE5E5E5, 0x026666, 0xF14C4C,
	0x23D18B, 0xF5F543, 0x3B8EEA, 0xD670D6, 0x29B8DB, 0xF2F2F0};
	int					rgb;

	rgb = -1;
	if (color >= 30 && color <= 39)
		rgb = map[color - 30];
	if (color >= 90 && color <= 97)
		rgb = map[color - 90 + 10];
	getline_highlight_add(buf, (t_buffer_attr){
		.start = it->byte_pos - offset,
		.end = it->byte_next - offset,
		.color = rgb,
		.bold = -1,
		.italic = -1,
		.underline = -1,
	});
}

void
	getline_set_prompt(t_getline *line, const char *text)
{
	const size_t	len = ft_strlen(text);
	size_t			offset;
	t_u8_iterator	it[2];
	int				color;

	color = -1;
	line->prompt = getline_buffer_new();
	stringbuf_init(&line->prompt.buffer, len);
	it[0] = it_new((t_string){text, len});
	it_next(&it[0]);
	offset = 0;
	while (1)
	{
		offset += handle_escape(&it[0], &color);
		if (!it[0].codepoint.len)
			break ;
		stringbuf_append(&line->prompt.buffer, it[0].codepoint);
		it[1] = it_new((t_string){line->prompt.buffer.str,
			line->prompt.buffer.len});
		it_next(&it[1]);
		it_advance(&it[1], it[0].byte_pos - offset);
		getline_recluster(line, &line->prompt, it[1]);
		apply_color(&line->prompt, color, &it[0], offset);
		it_next(&it[0]);
	}
}
