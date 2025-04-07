/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   strchr.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgamba <linogamba@pundalik.org>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 11:59:40 by lgamba            #+#    #+#             */
/*   Updated: 2025/03/17 11:59:41 by lgamba           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include <util/util.h>

char
	*ft_strchr(const char *s, int c)
{
	const unsigned char	*mem;

	mem = (const unsigned char *)s;
	while (*mem != (unsigned char)c)
	{
		if (!*mem)
			return (0);
		++mem;
	}
	return ((char *)mem);
}
