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
#include "ft_printf.h"
#include <shell/shell.h>

void
	getline_recycle_input(t_getline *line, const char *input, size_t len)
{
	stringbuf_append(&line->input_queue, (t_string){input, len});
}

int
	getline_read_char(t_getline *line)
{
	int	c;

	if (line->input_queue.len)
	{
		c = line->input_queue.str[0];
		stringbuf_replace(&line->input_queue, 0, 1, "");
		return (c);
	}
	return (line->getc_fn(line));
}
