/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   word_sub.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgamba <linogamba@pundalik.org>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 11:59:40 by lgamba            #+#    #+#             */
/*   Updated: 2025/03/17 11:59:41 by lgamba           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "util/util.h"
#include <shell/shell.h>

static void
	word_sub_iter(t_word *word, t_word *new, size_t i, const size_t range[4])
{
	const t_string_buffer	*current = &word->atoms[i].text;

	if (i == range[0] && word->atoms[i].type == W_LITERAL)
	{
		new->atoms[i - range[0]] = (struct s_atom){
			.type = W_LITERAL,
				.flags = word->atoms[i].flags,
		};
		if (i + 1 == range[2])
			new->atoms[i - range[0]].text = stringbuf_from_range(current->str + range[1],
					current->str + range[3]);
		else
			new->atoms[i - range[0]].text = stringbuf_from_range(current->str + range[1],
					current->str + current->len);
	}
	else if (i + 1 == range[2])
		new->atoms[i - range[0]] = (struct s_atom){
			.type = W_LITERAL,
				.flags = word->atoms[i].flags,
				.text = stringbuf_from_range(current->str,
						current->str + range[3])
		};
	else
		new->atoms[i - range[0]] = atom_copy(&word->atoms[i]);
}

t_word
	word_sub(t_word *word, const size_t range[4])
{
	t_word				new;
	size_t				i;
	
	new.natoms = (range[2] - range[0]);
	new.atoms = xmalloc(sizeof(struct s_atom) * new.natoms);
	i = range[0];
	while (i < range[2])
	{
		word_sub_iter(word, &new, i, range);
		++i;
	}
	return (new);
}
