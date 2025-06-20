/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   complete_draw_fn.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgamba <linogamba@pundalik.org>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 11:59:40 by lgamba            #+#    #+#             */
/*   Updated: 2025/03/17 11:59:41 by lgamba           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "getline/modes/modes.h"
#include "util/util.h"
#include <shell/shell.h>

static int
	draw_bounded(
	t_string_buffer *buf,
	const char *str,
	int maxwidth,
	const char *tr)
{
	int				w;
	int				len;
	t_u8_iterator	it;

	it = it_new((t_string){str, ft_strlen(str)});
	it_next(&it);
	w = 0;
	while (it.codepoint.len)
	{
		len = codepoint_width(u8_to_cp(it.codepoint));
		if (w + len > maxwidth)
		{
			stringbuf_append_s(buf, tr);
			break ;
		}
		stringbuf_append(buf, it.codepoint);
		w += len;
		it_next(&it);
	}
	return (w);
}

/** @brief Selects color based on the completion item */
static void
	item_kind_color(
	const t_getline *line,
	t_string_buffer *buf,
	const t_complete_item *item)
{
	(void)line;
	stringbuf_append_s(buf, "\x1b[");
	if (item->kind == COMPLETE_OPTION)
		stringbuf_append_i(buf, 94);
	else if (item->kind == COMPLETE_FILE_DIR)
		stringbuf_append_i(buf, 36);
	else if (item->kind == COMPLETE_FILE)
		stringbuf_append_i(buf, 90);
	else if (item->kind == COMPLETE_FILE_SPC)
		stringbuf_append_i(buf, 96);
	else
		stringbuf_append_i(buf, 37);
	stringbuf_append_s(buf, "m");
}

void
	getline_handler_comp_draw_item(
	t_getline *line,
	t_string_buffer *buf,
	size_t i,
	const t_complete_item *item)
{
	const int		desc_len = string_width(item->desc);
	int				w;

	if ((size_t)line->state.comp.sel == i)
		stringbuf_append_s(buf, "\x1b[7m");
	stringbuf_append_s(buf, "\x1b[37m");
	item_kind_color(line, buf, item);
	w = draw_bounded(buf, item->name, line->state.comp.col_width - 4, "…");
	while (desc_len + w++ < line->state.comp.col_width - 4)
		stringbuf_append_s(buf, " ");
	if (item->desc)
	{
		stringbuf_append_s(buf, "\x1b[33m(");
		w += 3;
		draw_bounded(buf, item->desc, line->state.comp.col_width - w, "…");
		stringbuf_append_s(buf, ")");
	}
	stringbuf_append_s(buf, "\x1b[m");
}
