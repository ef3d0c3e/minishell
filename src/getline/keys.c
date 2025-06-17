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
#include <shell/shell.h>

/** @brief Calls key handler function */
static int
	exec_bind(t_getline *line)
{
	const t_key_handler	*bind;

	bind = rb_find(&line->modes[line->mode].keybinds, line->sequence);
	if (!bind && line->mode != LINE_INPUT)
	{
		getline_change_mode(line, LINE_INPUT);
		return (exec_bind(line));
	}
	if (!bind)
		return (0);
	if (bind->sig == SIG_NONE)
		((void (*)(t_getline *))bind->function)(line);
	else if (bind->sig == SIG_I)
		((void (*)(t_getline *, int))bind->function)(line, bind->i0);
	else if (bind->sig == SIG_Z)
		((void (*)(t_getline *, int))bind->function)(line, bind->z0);
	getline_redraw(line, 0);
	return (1);
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

/* Snippet to debug keys:
ft_dprintf(2, "\n\rKEYSEQ:");
for (size_t i = 0; i < line->sequence_len; ++i)
{
	if (line->sequence[i] >= 32 && line->sequence[i] <= 126)
		ft_dprintf(2, "'%c' ", (int)line->sequence[i]);
	else
		ft_dprintf(2, "%x ", (int)line->sequence[i]);
}
*/

/** Returns 1 if `c` is part of a key sequence, thus not added to the line */
int
	getline_handle_key(t_getline *line, int c)
{
	const void	*bind;
	size_t		expect;

	if ((c < 0 || (c >= 32 && c != 127)) && !line->sequence_len
		&& line->mode == LINE_INPUT)
		return (0);
	line->sequence[line->sequence_len++] = c;
	line->sequence[line->sequence_len] = 0;
	expect = key_sequence_len(line);
	if (expect == SIZE_MAX)
		return (1);
	if (!exec_bind(line))
	{
		line->sequence_len = 0;
		return (1);
	}
	line->sequence_len = 0;
	return (1);
}
