/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   string_search.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgamba <linogamba@pundalik.org>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 11:59:40 by lgamba            #+#    #+#             */
/*   Updated: 2025/03/17 11:59:41 by lgamba           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "../util.h"

const char
	*str_alternatives(t_string str, const char **alternatives)
{
	size_t	i;
	size_t	j;

	i = 0;
	while (alternatives[i])
	{
		j = 0;
		while (j < str.len && str.str[j] == alternatives[i][j]
				&& alternatives[i][j])
			++j;
		if (alternatives[i][j] == 0)
			return (alternatives[i]);
		++i;
	}
	return (NULL);
}

const char
	*str_find_alternatives(t_string str, const char **alternatives)
{
	size_t		i;
	t_string	sub;
	const char	*alt;

	i = 0;
	while (i < str.len)
	{
		sub = (t_string){str.str + i, str.len - i};
		alt = str_alternatives(sub, alternatives);
		if (alt)
			return (alt);
		++i;
	}
	return (NULL);
}
