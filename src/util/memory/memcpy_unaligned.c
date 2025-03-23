/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   memcpy_unaligned.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgamba <linogamba@pundalik.org>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 11:59:40 by lgamba            #+#    #+#             */
/*   Updated: 2025/03/17 11:59:41 by lgamba           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "../util.h"

void
	*ft_memcpy(void *dest, const void *src, size_t len)
{
	char		*d;
	const char	*s = (const char *)src;

	d = (char *)dest;
	while (len > 0)
	{
		*d = *s;
		++d;
		++s;
		--len;
	}
	return (dest);
}
