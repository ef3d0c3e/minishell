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
		{"\x09", (void *)getline_complete_move, SIG_I, { .i0 = +1 }},
		{"\x1b[D", (void *)getline_complete_move, SIG_I, { .i0 = -1 }},
		{"\x02", (void *)getline_complete_move, SIG_I, { .i0 = -1 }},
		{"\x1b[Z", (void *)getline_complete_move, SIG_I, { .i0 = -1 }},
		{"\x1b[A", (void *)getline_complete_move_row, SIG_I, { .i0 = +1 }},
		{"\x0e", (void *)getline_complete_move_row, SIG_I, { .i0 = +1 }},
		{"\x1b[B", (void *)getline_complete_move_row, SIG_I, { .i0 = -1 }},
		{"\x10", (void *)getline_complete_move_row, SIG_I, { .i0 = -1 }},
		{NULL, NULL, 0, {0}}
	};

	return (keys);
}

// TODO
static t_complete_item
	*get_item(t_getline *line)
{
	static const t_complete_item items[] = {
		{COMPLETE_WORD, "test123", "this is a test"},
		{COMPLETE_FILE, "a.out", "file"},
		{COMPLETE_FILE, "b.out", "1"},
		{COMPLETE_FILE, "c.out", "2 lorem ipsum dolor sit amet very long description abcdefghij"},
		{COMPLETE_OPTION, "--help", "help page"},
		{0, NULL, NULL},
	};
	t_complete_item	*ret = xmalloc(sizeof(items));
	for (size_t i = 0; i < sizeof(items) / sizeof(items[0]); ++i)
	{
		ret[i].kind = items[i].kind;
		ret[i].name = NULL;
		ret[i].desc = NULL;
		if (items[i].name)
		{
			ret[i].name = ft_strdup(items[i].name);
			ret[i].desc = ft_strdup(items[i].desc);
		}
	}

	return (ret);
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
	line->comp_state.items = NULL;
	line->comp_draw_item_fn = getline_handler_comp_draw_item;
	line->comp_provider_fn = get_item;
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
