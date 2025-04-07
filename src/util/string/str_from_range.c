/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   str_from_range.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgamba <linogamba@pundalik.org>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 11:59:40 by lgamba            #+#    #+#             */
/*   Updated: 2025/03/17 11:59:41 by lgamba           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "../util.h"

char
*ft_substr(const char *s, size_t start, size_t len)
{
	char	*str;

	str = xmalloc(len + 1);
	ft_memcpy(str, s + start, len);
	str[len] = 0;
	return (str);
}
