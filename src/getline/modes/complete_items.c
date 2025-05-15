/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   complete_items.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgamba <linogamba@pundalik.org>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 11:59:40 by lgamba            #+#    #+#             */
/*   Updated: 2025/03/17 11:59:41 by lgamba           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include <shell/shell.h>

void
	getline_complete_free_items(t_getline *line)
{
	size_t	i;

	i = 0;
	while (line->state.comp.items && line->state.comp.items[i].name)
	{
		free(line->state.comp.items[i].name);
		free(line->state.comp.items[i].desc);
		++i;
	}
	free(line->state.comp.items);
	line->state.comp.items = NULL;
	line->state.comp.nitems = 0;
}

void
	getline_complete_populate_items(t_getline *line)
{
	if (!line->comp_provider_fn)
		return ;
	getline_complete_free_items(line);
	line->state.comp.items = line->comp_provider_fn(line);
	line->state.comp.nitems = 0;
	while (line->state.comp.items[line->state.comp.nitems].name)
		++line->state.comp.nitems;
}
