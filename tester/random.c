/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   random.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgamba <linogamba@pundalik.org>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 11:59:40 by lgamba            #+#    #+#             */
/*   Updated: 2025/03/17 11:59:41 by lgamba           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "tester.h"

/** @brief 32bit xorshift random number generator */
static inline uint32_t	xorshift(uint32_t *state)
{
	uint32_t		v;

	v = *state;
	*state ^= *state << 13;
	*state ^= *state >> 17;
	*state ^= *state << 5;
	return (v);
}

char
	*random_ascii(const char *set, size_t length, uint32_t *seed)
{
	const size_t	set_len = ft_strlen(set);
	size_t			i;
	char			*text;

	text = malloc(length + 1);
	i = 0;
	while (i < length)
		text[i++] = set[xorshift(seed) % set_len];
	text[i] = 0;
	return (text);
}

uint32_t
	random_int(uint32_t *seed)
{
	return (xorshift(seed));
}
