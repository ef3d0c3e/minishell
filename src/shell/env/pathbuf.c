/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pathbuf.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgamba <linogamba@pundalik.org>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 11:59:40 by lgamba            #+#    #+#             */
/*   Updated: 2025/03/17 11:59:41 by lgamba           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include <shell/shell.h>

void
	pathbuf_init(t_pathbuf *buf, size_t initial_capacity)
{
	stringbuf_init(&buf->buf, initial_capacity);
}

int
	pathbuf_append(t_pathbuf *buf, const char *component, int force_dir)
{
	int		has_slash;
	size_t	len;
	size_t	start;

	if (!component)
		return (0);
	has_slash = 0;
	if (buf->buf.len && buf->buf.str[buf->buf.len - 1] == '/')
		has_slash = 1;
	len = ft_strlen(component);
	while (len > 1 && component[len - 1] == '/')
	{
		if (force_dir && len > 2 && component[len - 2] != '/')
			break ;
		--len;
	}
	start = 0;
	while (has_slash && start < len && component[start] == '/')
		++start;
	stringbuf_append(&buf->buf, (t_string){component + start, len - start});
	if (force_dir && (len <= 1 || component[len - 1] != '/'))
		stringbuf_append(&buf->buf, (t_string){"/", 1});
	return (1);
}

char
	*pathbuf_cstr(t_pathbuf *buf)
{
	return (stringbuf_cstr(&buf->buf));
}

void
	pathbuf_free(t_pathbuf *buf)
{
	stringbuf_free(&buf->buf);
}
