/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   string.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgamba <linogamba@pundalik.org>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 11:59:40 by lgamba            #+#    #+#             */
/*   Updated: 2025/03/17 11:59:41 by lgamba           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "string.h"
#include "mem.h"
#include <stddef.h>
#include <stdlib.h>

t_string
	string_new(void)
{
	return ((t_string){.s = NULL, .len = 0, .capacity = 0});
}


void
	string_free(t_string *s)
{
	free(s->s);
}

void
	string_push(t_string *s, t_string_view cp)
{
	size_t	new_cap;

	new_cap = s->capacity + !s->capacity * 256;
	while (new_cap < s->len + cp.len)
		new_cap *= 2;
	s->s = ft_realloc(s->s, s->capacity, new_cap);
	s->capacity = new_cap;
	ft_memcpy(s->s + s->len, cp.s, cp.len);
	s->len += cp.len;
}
