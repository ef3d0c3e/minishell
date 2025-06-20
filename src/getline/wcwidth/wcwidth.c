/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wcwidth.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgamba <linogamba@pundalik.org>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 11:59:40 by lgamba            #+#    #+#             */
/*   Updated: 2025/03/17 11:59:41 by lgamba           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include <shell/shell.h>

int
	codepoint_width(uint32_t cp)
{
	if (cp == 0)
		return (0);
	else if (cp < 32 || (cp >= 0x7f && cp < 0xa0))
		return (-1);
	if (codepoint_is_wide(cp))
		return (2);
	if (codepoint_is_fuser(cp))
		return (0);
	return (1);
}

int
	string_width(const char *str)
{
	t_u8_iterator	it;
	int				width;

	if (!str)
		return (0);
	it = it_new((t_string){str, ft_strlen(str)});
	it_next(&it);
	width = 0;
	while (it.codepoint.len)
	{
		width += codepoint_width(u8_to_cp(it.codepoint));
		it_next(&it);
	}
	return (width);
}

size_t
	string_width_truncate(const char *str, int max)
{
	t_u8_iterator	it;
	int				width;

	it = it_new((t_string){str, ft_strlen(str)});
	it_next(&it);
	width = 0;
	while (it.codepoint.len)
	{
		width += codepoint_width(u8_to_cp(it.codepoint));
		if (width > max)
			return (it.byte_pos);
		it_next(&it);
	}
	return (SIZE_MAX);
}
