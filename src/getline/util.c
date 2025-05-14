/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   util.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgamba <linogamba@pundalik.org>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 11:59:40 by lgamba            #+#    #+#             */
/*   Updated: 2025/03/17 11:59:41 by lgamba           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include <shell/shell.h>

size_t
	getline_escape_len(const char *s, size_t len)
{
	size_t	i;

	if (!len || (unsigned char)s[0] != '\x1b')
		return (0);
	if (len == 1)
		return (1);
	if (s[1] != '[')
		return (2);
	i = 2;
	while (i < len)
	{
		if ((unsigned char)s[i] >= '@' && (unsigned char)s[i] <= '~')
			return (i + 1);
		++i;
	}
	return (i);
}
