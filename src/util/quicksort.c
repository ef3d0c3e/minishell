/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quicksort.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgamba <linogamba@pundalik.org>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 11:59:40 by lgamba            #+#    #+#             */
/*   Updated: 2025/03/17 11:59:41 by lgamba           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include <util/util.h>

/*
static void
	swap(void *a, void *b, size_t size)
{
	size_t	i;
	uint8_t	t;

	i = 0;
	while (i < size)
	{
		t = ((uint8_t*)a)[i];
		((uint8_t*)a)[i] = ((uint8_t*)b)[i];
		((uint8_t*)b)[i] = t;
		++i;
	}
}
*/

void 
	quicksort(void *base, size_t nmemb, size_t size,
	int (*cmp)(const void *, const void *))
{
	// TODO	
	qsort(base, nmemb, size, cmp);
}
