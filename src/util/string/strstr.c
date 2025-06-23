/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   strstr.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgamba <linogamba@pundalik.org>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 11:59:40 by lgamba            #+#    #+#             */
/*   Updated: 2025/03/17 11:59:41 by lgamba           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include <util/util.h>

char
	*ft_strstr(const char *haystack, const char *needle)
{
	const size_t	n = ft_strlen(haystack);
	size_t			find_len;
	size_t			i;
	size_t			j;

	if (!*needle)
		return ((char *)haystack);
	if (n == 0)
		return ((char *)0);
	find_len = ft_strlen(needle);
	i = 0;
	while (haystack[i] && i + find_len <= n)
	{
		j = 0;
		while (haystack[i + j] == needle[j])
		{
			if (j + 1 == find_len)
				return (((char *)haystack + i));
			++j;
		}
		++i;
	}
	return (0);
}
