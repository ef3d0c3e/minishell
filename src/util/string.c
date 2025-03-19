/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   string.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgamba <linogamba@pundalik.org>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 11:59:40 by lgamba            #+#    #+#             */
/*   Updated: 2025/03/17 11:59:41 by lgamba           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "util.h"
#include <stddef.h>
#include <stdlib.h>

int
	str_cmp(t_string sv, const char *token)
{
	size_t	i;

	i = 0;
	while (i < sv.len && token[i] && sv.str[i] == token[i])
		++i;
	if (i == sv.len)
		return (-token[i]);
	return (sv.str[i] - token[i]);
}

int
	str_starts_with(t_string str, const char *token)
{
	size_t	i;

	i = 0;
	while (i < str.len && token[i] && str.str[i] == token[i])
		++i;
	if (i == str.len || !token[i])
		return (1);
	return (0);
}

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
