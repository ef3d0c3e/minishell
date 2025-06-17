/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input_draw_buffer.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgamba <linogamba@pundalik.org>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 11:59:40 by lgamba            #+#    #+#             */
/*   Updated: 2025/03/17 11:59:41 by lgamba           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include <shell/shell.h>

/** @brief Initializes data for drawing */
static void
	init_data(t_buffer *buf, t_u8_iterator *it, t_buffer_attr **hi)
{
	hi[0] = NULL;
	*it = it_new((t_string){buf->buffer.str, buf->buffer.len});
	it_next(it);
}

/** @brief Update and display highlight for the prompt */
static void
	update_highlight(
	t_getline *l,
	t_buffer *buf,
	t_u8_iterator *it,
	t_buffer_attr **hi)
{
	hi[1] = getline_highlight_get(buf, it->byte_pos);
	if (hi[1] && hi[1] != hi[0])
	{
		getline_highlight_display(l, hi[1]);
		hi[0] = hi[1];
	}
	if (!hi[1] && hi[0])
	{
		getline_highlight_display(l, hi[1]);
		hi[0] = NULL;
	}
}

/** @brief Print spaces after the content's end */
static int
	print_padding(t_getline *l, t_drawline *dr, int start)
{
	if (start >= l->scrolled + l->display_width)
		return (0);
	while (dr->printed < start - l->scrolled - dr->left_indicator
		&& dr->printed < l->display_width - dr->right_indicator)
	{
		write(l->out_fd, " ", 1);
		dr->printed++;
	}
	return (1);
}

static int
	check_end(t_getline *l, t_drawline *dr, t_u8_iterator *it, int end)
{
	if (end <= l->scrolled)
	{
		dr->column_pos = end;
		it_next(it);
		return (0);
	}
	return (1);
}

void
	getline_input_draw_buffer(t_getline *l, t_buffer *buf, t_drawline *dr)
{
	size_t			i;
	t_u8_iterator	it;
	int				start;
	int				end;
	t_buffer_attr	*hi[2];

	init_data(buf, &it, hi);
	i = 0;
	while (i++ < buf->s_clusters.size && it.codepoint.len
		&& dr->printed < l->display_width - dr->right_indicator)
	{
		start = dr->column_pos;
		end = start + buf->s_clusters.data[i - 1].width;
		if (!check_end(l, dr, &it, end))
			continue ;
		if (!print_padding(l, dr, start))
			break ;
		update_highlight(l, buf, &it, hi);
		write(l->out_fd, it.codepoint.str, it.codepoint.len);
		dr->printed += buf->s_clusters.data[i - 1].width;
		dr->column_pos = end;
		it_next(&it);
	}
	(void)(hi[0] && (getline_highlight_display(l, NULL), 1));
}
