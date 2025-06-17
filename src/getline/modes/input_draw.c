/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input_draw.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgamba <linogamba@pundalik.org>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 11:59:40 by lgamba            #+#    #+#             */
/*   Updated: 2025/03/17 11:59:41 by lgamba           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include <shell/shell.h>

void
getline_input_draw_buffer(t_getline *l, t_buffer *buf, t_drawline *dr);

static void
	init_state(t_getline *l, t_drawline *dr)
{
	size_t			idx;
	t_u8_iterator	it;

	idx = 0;
	dr->prompt_w = 0;
	while (idx++ < l->prompt.s_clusters.size)
		dr->prompt_w += l->prompt.s_clusters.data[idx - 1].width;
	idx = 0;
	dr->input_w = 0;
	while (idx++ < l->input.s_clusters.size)
		dr->input_w += l->input.s_clusters.data[idx - 1].width;
	dr->cursor_pos = dr->prompt_w;
	it = it_new((t_string){l->input.buffer.str, l->input.buffer.len});
	it_next(&it);
	idx = 0;
	while (idx < l->input.s_clusters.size && it.codepoint.len)
	{
		if (it.byte_pos < (size_t)l->cursor_index)
			dr->cursor_pos += l->input.s_clusters.data[idx].width;
		it_next(&it);
		idx++;
	}
	if (dr->cursor_pos > dr->input_w + dr->prompt_w)
		dr->cursor_pos = dr->input_w + dr->prompt_w;
}

static void
	update_scroll(t_getline *l, t_drawline *dr)
{
	int	sc;

	sc = (int)l->scrolled;
	dr->left_indicator = sc > 0;
	if (dr->cursor_pos >= sc + l->display_width - dr->left_indicator - 1)
		sc = dr->cursor_pos - l->display_width + dr->left_indicator + 2;
	else if (dr->cursor_pos < sc)
		sc = dr->cursor_pos;
	if (sc < 0)
		sc = 0;
	l->scrolled = sc;
	dr->right_indicator = (dr->prompt_w + dr->input_w) > sc
		+ (l->display_width - dr->left_indicator);
}

/** @brief Updates the line by recomputing highlight data */
static void
	draw_update(t_getline *l, int update)
{
	if (!update)
		return ;
	free(l->input.s_attrs.data);
	l->input.s_attrs.data = NULL;
	l->input.s_attrs.size = 0;
	l->input.s_attrs.capacity = 0;
	if (l->highlighter_fn)
		l->highlighter_fn(l);
}

void
	getline_input_draw(t_getline *l, int update)
{
	t_drawline	dr;
	int			vis;

	draw_update(l, update);
	ft_dprintf(l->out_fd, "\x1b[2K\r");
	init_state(l, &dr);
	update_scroll(l, &dr);
	dr.printed = 0;
	dr.column_pos = 0;
	if (dr.left_indicator && ++dr.printed)
		l->overflow_fn(l, 0);
	getline_input_draw_buffer(l, &l->prompt, &dr);
	getline_input_draw_buffer(l, &l->input, &dr);
	if (dr.right_indicator && dr.printed < l->display_width)
	{
		while (dr.printed < l->display_width - 1 && ++dr.printed)
			write(l->out_fd, " ", 1);
		l->overflow_fn(l, 1);
	}
	vis = dr.cursor_pos - l->scrolled + dr.left_indicator;
	if (vis < 0)
		vis = 0;
	if (vis >= l->display_width)
		vis = l->display_width - 1;
	ft_dprintf(l->out_fd, "\x1b[%dG", vis + 1);
}
