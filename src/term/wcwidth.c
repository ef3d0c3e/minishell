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

size_t
	grapheme_width(
	t_shell *shell,
	const char *str,
	int expensive,
	int skip_ansi)
{
	t_u8_iterator	it;
	int				len;
	size_t			total;
	
	total = 0;
	if (expensive) { /* TODO */ }
	/*
	it = it_new((t_string){str, ft_strlen(str)});
	it_next(&it);
	while (it.codepoint.len)
	{
		if (skip_ansi && !ft_strncmp(it_substr(&it, 2).str, "\033[", 2))
		{
			it_advance(&it, 2);
			while (it.codepoint.len && ((it.codepoint.str[0] >= '0'
				&& it.codepoint.str[0] <= '9') || it.codepoint.str[0] == ';'))
					it_next(&it);
			it_next(&it);
			continue ;
		}
		len = codepoint_width(it.str.str);
		if (len > 0)
			total += len;
		it_next(&it);
	}
	*/
	return (total);
}
