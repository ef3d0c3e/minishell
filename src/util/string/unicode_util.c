/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unicode_util.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgamba <linogamba@pundalik.org>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 11:59:40 by lgamba            #+#    #+#             */
/*   Updated: 2025/03/17 11:59:41 by lgamba           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "../util.h"

size_t
	u8_length(char c)
{
	if ((c & 0x80) == 0)
		return (1);
	else if ((c & 0xE0) == 0xC0)
		return (2);
	else if ((c & 0xF0) == 0xE0)
		return (3);
	else if ((c & 0xF8) == 0xF0)
		return (4);
	else if ((c & 0xFC) == 0xF8)
		return (5);
	else if ((c & 0xFE) == 0xFC)
		return (6);
	return (0);
}

uint32_t
	u8_to_cp(t_string cp)
{
	if (cp.len == 1)
		return (cp.str[0]);
	else if (cp.len == 2)
		return (((cp.str[0] & 0x1F) << 6)
			| (cp.str[1] & 0x3F));
	else if (cp.len == 3)
		return (((cp.str[0] & 0x0F) << 12)
			| ((cp.str[1] & 0x3F) << 6)
			| (cp.str[2] & 0x3F));
	else if (cp.len == 4)
		return (((cp.str[0] & 0x07) << 18)
			| ((cp.str[1] & 0x3F) << 12)
			| ((cp.str[2] & 0x3F) << 6)
			| (cp.str[3] & 0x3F));
	return (0xFFFD);
}
