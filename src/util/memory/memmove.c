/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   memmove.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgamba <linogamba@pundalik.org>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 11:59:40 by lgamba            #+#    #+#             */
/*   Updated: 2025/03/17 11:59:41 by lgamba           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "../util.h"

void
	*ft_memmove(void *dest, const void *src, size_t n)
{
	size_t				i;

	if (!dest && !src)
		return (dest);
	i = 0;
	if (src < dest)
	{
		while (i < n)
		{
			((char *)dest)[n - i - 1] = ((char *)src)[n - i - 1];
			++i;
		}
	}
	else
	{
		while (i < n)
		{
			((char *)dest)[i] = ((char *)src)[i];
			++i;
		}
	}
	return (dest);
}
