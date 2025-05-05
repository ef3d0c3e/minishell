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
#include "../util.h"

t_string
	str_new(const char *buf, size_t len)
{
	return ((t_string){
		.str = buf,
		.len = len
	});
}

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

size_t
	str_find(t_string str, const char *token)
{
	const size_t	len = ft_strlen(token);
	size_t			i;

	i = 0;
	while (i + len <= str.len)
	{
		if (!str_cmp((t_string){str.str + i, len}, token))
			return (i);
		++i;
	}
	return ((size_t) - 1);
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
