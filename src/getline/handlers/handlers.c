/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handlers.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgamba <linogamba@pundalik.org>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 11:59:40 by lgamba            #+#    #+#             */
/*   Updated: 2025/03/17 11:59:41 by lgamba           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "getline/getline.h"
#include <shell/shell.h>

/** @brief Keybinds */
static t_key_handler
	*handlers_keys(void)
{
	static t_key_handler	keys[] = {
		{"\x1b[C", (void *)getline_move, SIG_I, { .i0 = +1 }},
		{"\x06", (void *)getline_move, SIG_I, { .i0 = +1 }},
		{"\x1b[D", (void *)getline_move, SIG_I, { .i0 = -1 }},
		{"\x02", (void *)getline_move, SIG_I, { .i0 = -1 }},
		{"\x01", (void *)getline_move_at, SIG_Z, { .z0 = 0 }},
		{"\x05", (void *)getline_move_at, SIG_Z, { .z0 = SIZE_MAX }},
		{"\x7f", (void *)getline_delete, SIG_I, { .i0 = -1 }},
		{"\x1b[3~", (void *)getline_delete, SIG_I, { .i0 = +1 }},
		{"\x1b[1;5D", (void *)getline_move_word, SIG_I, { .i0 = -1 }},
		{"\x1b[1;5C", (void *)getline_move_word, SIG_I, { .i0 = +1 }},
		{"\x17", (void *)getline_delete_word, SIG_I, { .i0 = -1 }},
		{"\x1b[3;5~", (void *)getline_delete_word, SIG_I, { .i0 = +1 }},
		{"\x09", (void *)getline_complete_menu, SIG_NONE, { 0 }},
		{NULL, NULL, 0, {0}}
	};

	return (keys);
}

/** @brief Keybinds for completion */
static t_key_handler
	*handlers_keys_complete(void)
{
	static t_key_handler	keys[] = {
		{"\x1b[C", (void *)getline_complete_move, SIG_I, { .i0 = +1 }},
		{"\x06", (void *)getline_complete_move, SIG_I, { .i0 = +1 }},
		{"\x1b[D", (void *)getline_complete_move, SIG_I, { .i0 = -1 }},
		{"\x02", (void *)getline_complete_move, SIG_I, { .i0 = -1 }},
		{"\x1b[A", (void *)getline_complete_move_row, SIG_I, { .i0 = +1 }},
		{"\x0e", (void *)getline_complete_move_row, SIG_I, { .i0 = +1 }},
		{"\x1b[B", (void *)getline_complete_move_row, SIG_I, { .i0 = -1 }},
		{"\x10", (void *)getline_complete_move_row, SIG_I, { .i0 = -1 }},
		{NULL, NULL, 0, {0}}
	};

	return (keys);
}

void
	getline_setup_handlers(t_getline *line)
{
	size_t	i;

	line->getc_fn = getline_getc;
	line->overflow_fn = getline_handler_overflow;
	line->highlighter_fn = NULL;
	line->boundaries_fn = getline_handler_word_boundaries;
	line->comp_state.shown = 0;
	line->comp_state.sel = 0;
	line->comp_state.comp_draw_item_fn = getline_handler_draw_comp_item;
	i = 0;
	while (handlers_keys()[i].keyseq)
	{
		rb_insert(&line->keybinds, (void *)handlers_keys()[i].keyseq,
				&handlers_keys()[i]);
		++i;
	}
	i = 0;
	while (handlers_keys_complete()[i].keyseq)
	{
		rb_insert(&line->comp_keybinds,
				(void *)handlers_keys_complete()[i].keyseq,
				&handlers_keys_complete()[i]);
		++i;
	}
}
