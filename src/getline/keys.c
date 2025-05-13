/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   keys.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgamba <linogamba@pundalik.org>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 11:59:40 by lgamba            #+#    #+#             */
/*   Updated: 2025/03/17 11:59:41 by lgamba           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "ft_printf.h"
#include "getline/getline.h"
#include "util/util.h"
#include <shell/shell.h>

void
	getline_move_left(t_getline *line)
{
	getline_move(line, -1);
}

void
	getline_move_right(t_getline *line)
{
	getline_move(line, 1);
}

void
	getline_move_start(t_getline *line)
{
	getline_move_at(line, 0);
}

void
	getline_move_end(t_getline *line)
{
	getline_move_at(line, line->buffer.buffer.len);
}

void
	getline_del(t_getline *line)
{
	getline_delete(line, 1);
}

void
	getline_backspace(t_getline *line)
{
	getline_delete(line, -1);
}

void
	getline_setup_keys(t_getline *line)
{
	line->sequence_len = 0;
	line->keybinds = rb_new((int (*)(const void *, const void *))ft_strcmp,
			NULL, NULL);
	rb_insert(&line->keybinds, "\x1b[C", (void *)getline_move_right);
	rb_insert(&line->keybinds, "\x1b[D", (void *)getline_move_left);
	rb_insert(&line->keybinds, "\x0d", (void *)getline_move_right);
	rb_insert(&line->keybinds, "\x10", (void *)getline_move_left);
	rb_insert(&line->keybinds, "\x7f", (void *)getline_backspace);
	rb_insert(&line->keybinds, "\x1b[3~", (void *)getline_del);
	rb_insert(&line->keybinds, "\x01", (void *)getline_move_start);
	rb_insert(&line->keybinds, "\x05", (void *)getline_move_end);
}

/** @brief Gets the length of a key sequence */
static size_t
	key_sequence_len(t_getline *line)
{
	size_t	i;

	if (!line->sequence_len)
		return (0);
	if ((unsigned char)line->sequence[0] != '\x1b')
		return (1);
	if (line->sequence_len == 1)
		return (SIZE_MAX);
	if (line->sequence[1] != '[' && line->sequence[1] != 'O')
		return (2);
	i = 2;
	while (i < line->sequence_len)
	{
		if ((unsigned char)line->sequence[i] >= '@'
			&& (unsigned char)line->sequence[i] <= '~')
			return (i + 1);
		++i;
	}
	return (SIZE_MAX);
}

/** Returns 1 if `c` is part of a key sequence, thus not added to the line */
int
	getline_handle_key(t_getline *line, int c)
{
	t_keybind_fn	bind;
	size_t			expect;

	if ((c < 0 || (c >= 32 && c != 127)) && !line->sequence_len)
		return (0);
	line->sequence[line->sequence_len++] = c;
	line->sequence[line->sequence_len] = 0;
	expect = key_sequence_len(line);
	if (expect == SIZE_MAX)
		return (1);
	bind = (t_keybind_fn)rb_find(&line->keybinds, line->sequence);
	if (bind)
		bind(line);
	else
	{
		ft_dprintf(2, "\n\rKEYSEQ:");
		for (size_t i = 0; i < line->sequence_len; ++i)
			ft_dprintf(2, "%x ", (int)line->sequence[i]);
	}
	line->sequence_len = 0;
	return (1);
}
