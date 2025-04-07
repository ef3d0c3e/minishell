/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   strncmp.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgamba <linogamba@pundalik.org>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 11:59:40 by lgamba            #+#    #+#             */
/*   Updated: 2025/03/17 11:59:41 by lgamba           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include <util/util.h>

int
	ft_strncmp(const char *first, const char *second, size_t n)
{
	size_t				i;
	const unsigned char	*x;
	const unsigned char	*y;

	x = (const unsigned char *)first;
	y = (const unsigned char *)second;
	i = 0;
	while (i < n && x[i] && y[i] && x[i] == y[i])
		++i;
	return ((i != n) * (x[i] - y[i]));
}
