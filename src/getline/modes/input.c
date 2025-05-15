/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgamba <linogamba@pundalik.org>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 11:59:40 by lgamba            #+#    #+#             */
/*   Updated: 2025/03/17 11:59:41 by lgamba           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include <shell/shell.h>

static void
	getline_input_enable(t_getline *line)
{
	line->state.action = ACT_NONE;
}

static t_key_handler
	*keybinds(void)
{
	static t_key_handler	keys[] = {
		{"\x0d", (void *)getline_input_action, SIG_I, { .i0 = ACT_ENTER }},
		{"\x03", (void *)getline_input_action, SIG_I, { .i0 = ACT_CANCEL }},
		{"\x04", (void *)getline_input_action, SIG_I, { .i0 = ACT_QUIT }},
		{"\x0c", (void *)getline_input_action, SIG_I, { .i0 = ACT_CLEAR_SCREEN }},

		{"\x1b[C", (void *)getline_input_move, SIG_I, { .i0 = +1 }},
		{"\x06", (void *)getline_input_move, SIG_I, { .i0 = +1 }},
		{"\x1b[D", (void *)getline_input_move, SIG_I, { .i0 = -1 }},
		{"\x02", (void *)getline_input_move, SIG_I, { .i0 = -1 }},
		{"\x01", (void *)getline_input_move_at, SIG_Z, { .z0 = 0 }},
		{"\x05", (void *)getline_input_move_at, SIG_Z, { .z0 = SIZE_MAX }},
		{"\x7f", (void *)getline_input_delete, SIG_I, { .i0 = -1 }},
		{"\x1b[3~", (void *)getline_input_delete, SIG_I, { .i0 = +1 }},
		{"\x1b[1;5D", (void *)getline_input_move_word, SIG_I, { .i0 = -1 }},
		{"\x1b[1;5C", (void *)getline_input_move_word, SIG_I, { .i0 = +1 }},
		{"\x17", (void *)getline_input_delete_word, SIG_I, { .i0 = -1 }},
		{"\x1b[3;5~", (void *)getline_input_delete_word, SIG_I, { .i0 = +1 }},

		//{"\x1b[A", (void *)getline_history_scroll, SIG_I, { .i0 = +1 }},
		//{"\x1b[B", (void *)getline_history_scroll, SIG_I, { .i0 = -1 }},

		{"\x09", (void *)getline_change_mode, SIG_I, { LINE_TAB }},

		{"\x10", (void *)getline_change_mode, SIG_I, { LINE_HISTSCROLL }},
		{"\x1b[A", (void *)getline_change_mode, SIG_I, { LINE_HISTSCROLL }},
		{NULL, NULL, 0, {0}}
	};

	return (keys);
}

void
	getline_setup_input_mode(t_line_mode *mode)
{
	size_t	i;

	mode->keybinds = rb_new((int (*)(const void *, const void *))ft_strcmp,
			NULL, NULL);
	mode->enable_mode_fn = getline_input_enable;
	mode->disable_mode_fn = NULL;
	mode->draw_mode_fn = getline_input_draw;
	i = 0;
	while (keybinds()[i].keyseq)
	{
		rb_insert(&mode->keybinds,
				(void *)keybinds()[i].keyseq,
				&keybinds()[i]);
		++i;
	}
}
