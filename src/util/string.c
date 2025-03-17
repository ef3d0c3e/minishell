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

int
	str_cmp(t_string sv, const char *token)
{
	size_t	i;

	i = 0;
	while (i < sv.len && token[i] && sv.s[i] == token[i])
		++i;
	if (i == sv.len)
		return (-token[i]);
	return (sv.s[i] - token[i]);
}
