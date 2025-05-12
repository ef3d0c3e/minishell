/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgamba <linogamba@pundalik.org>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 11:59:40 by lgamba            #+#    #+#             */
/*   Updated: 2025/03/17 11:59:41 by lgamba           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include <shell/shell.h>

typedef struct s_drawline
{
	int	cursor_pos;
	int	column_pos;
	int	input_w;
	int	prompt_w;
	int	left_indicator;
	int	right_indicator;
	int printed;
}	t_drawline;

void
	draw_buffer(t_getline *l, t_buffer *buf, t_drawline *dr)
{
	size_t			i = 0;
	t_u8_iterator	it;

	it = it_new((t_string){buf->buffer.str, buf->buffer.len});
	it_next(&it);
	while (i++ < buf->s_clusters.size && it.codepoint.len
			&& dr->printed < l->render.display_width - dr->right_indicator)
	{
		t_cluster *c = &buf->s_clusters.data[i - 1];
		int start = dr->column_pos;
		int	end = start + c->width;
		if (end <= l->render.scrolled)
		{
			dr->column_pos = end;
			it_next(&it);
			continue ;
		}
		if (start >= l->render.scrolled + l->render.display_width)
			break ;
		while (dr->printed < start - l->render.scrolled - dr->left_indicator
				&& dr->printed < l->render.display_width - dr->right_indicator)
		{
			write(l->out_fd, " ", 1);
			dr->printed++;
		}
		write(l->out_fd, it.codepoint.str, it.codepoint.len);
		dr->printed += c->width;
		dr->column_pos = end;
		it_next(&it);
	}
}

void getline_redraw(t_getline *l)
{
	t_drawline	dr;
	size_t		idx;

	// Clear
	ft_dprintf(l->out_fd, "\x1b[2K\r");

	// Measure widths
	idx = 0;
	dr.prompt_w = 0;
	while (idx++ < l->prompt.s_clusters.size)
		dr.prompt_w += l->prompt.s_clusters.data[idx - 1].width;
	idx = 0;
	dr.input_w = 0;
	while (idx++ < l->buffer.s_clusters.size)
		dr.input_w += l->buffer.s_clusters.data[idx - 1].width;

	// 4) compute dr.cursor_pos in columns
	dr.cursor_pos = dr.prompt_w;
	{
		size_t byte_acc = 0, ci = 0;
		t_u8_iterator it = it_new((t_string){l->buffer.buffer.str, l->buffer.buffer.len});
		it_next(&it);
		while (ci < l->buffer.s_clusters.size && it.codepoint.len)
		{
			size_t sz = it.codepoint.len;
			int    w  = l->buffer.s_clusters.data[ci].width;
			if (byte_acc < (size_t)l->cursor_index)
				dr.cursor_pos += w;
			byte_acc += sz;
			it_next(&it);
			ci++;
		}
	}
	if (dr.cursor_pos > dr.input_w + dr.prompt_w)
		dr.cursor_pos = dr.input_w + dr.prompt_w;

	// compute scroll position to ensure cursor visibility, prioritizing showing more text
	int sc = (int)l->render.scrolled;

	dr.left_indicator = sc > 0;

	if (dr.cursor_pos >= sc + l->render.display_width - dr.left_indicator - 1)
		sc = dr.cursor_pos - (l->render.display_width - dr.left_indicator - 2);
	else if (dr.cursor_pos < sc)
		sc = dr.cursor_pos;
	if (sc < 0)
		sc = 0;
	l->render.scrolled = sc;
	dr.right_indicator = (dr.prompt_w + dr.input_w) > sc + (l->render.display_width - dr.left_indicator);

	dr.printed = 0;
	dr.column_pos = 0;
	if (dr.left_indicator && ++dr.printed)
		write(l->out_fd, ">", 1);

	draw_buffer(l, &l->prompt, &dr);
	draw_buffer(l, &l->buffer, &dr);

	if (dr.right_indicator && dr.printed < l->render.display_width)
	{
		while (dr.printed < l->render.display_width - 1)
		{
			write(l->out_fd, " ",1);
			dr.printed++;
		}
		write(l->out_fd, "<",1);
	}

	int vis = dr.cursor_pos - sc + dr.left_indicator;
	if (vis < 0)
		vis = 0;
	if (vis >= l->render.display_width)
		vis = l->render.display_width - 1;
	ft_dprintf(l->out_fd, "\x1b[%dG", vis + 1);
}
