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

void
	getline_setup_handlers(t_getline *line)
{
	line->getc_fn = getline_handlers_getc;
	line->overflow_fn = getline_handler_overflow;
	line->highlighter_fn = NULL;
	line->comp_provider_fn = NULL;
	line->boundaries_fn = getline_handler_word_boundaries;
	line->comp_draw_item_fn = getline_handler_comp_draw_item;
}
