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

const char
	*str_alternatives(t_string str, const char **alternatives)
{
	size_t	i;

	i = 0;
	while (alternatives[i])
	{
		if (!str_cmp(str, alternatives[i]))
			return (alternatives[i]);
		++i;
	}
	return (NULL);
}
