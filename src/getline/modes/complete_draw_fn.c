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
#include <shell/shell.h>

static int
	draw_bounded(t_getline *line, const char *str, int maxwidth, const char *tr)
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
			write(line->out_fd, tr, ft_strlen(tr));
			break ;
		}
		write(line->out_fd, it.codepoint.str, it.codepoint.len);
		w += len;
		it_next(&it);
	}
	return (w);
}

void
	getline_handler_comp_draw_item(
	t_getline *line,
	size_t i,
	const t_complete_item *item)
{
	const int		desc_len = string_width(item->desc);
	int				w;

	if ((size_t)line->state.comp.sel == i)
		ft_dprintf(line->out_fd, "\x1b[7m");
	ft_dprintf(line->out_fd, "\x1b[37m");
	w = draw_bounded(line, item->name, line->state.comp.col_width - 4, "…");
	while (desc_len + w++ < line->state.comp.col_width - 4)
		ft_dprintf(line->out_fd, " ");
	if (item->desc)
	{
		ft_dprintf(line->out_fd, "\x1b[33m");
		ft_dprintf(line->out_fd, "(");
		w += 3;
		draw_bounded(line, item->desc, line->state.comp.col_width - w, "…");
		ft_dprintf(line->out_fd, ")");
	}
	ft_dprintf(line->out_fd, "\x1b[m");
}
