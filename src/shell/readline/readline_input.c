/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   readline_input.h                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgamba <linogamba@pundalik.org>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 11:59:40 by lgamba            #+#    #+#             */
/*   Updated: 2025/03/17 11:59:41 by lgamba           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include <shell/shell.h>

int
	readline_getc(FILE *stream)
{
	char	c;
	ssize_t	n;

	(void)stream;
	n = read(STDIN_FILENO, &c, 1);
	if (n == 1)
		return ((unsigned char)c);
	if (n == 0)
		return (EOF);
	if (errno == EINTR && g_signal != SIGQUIT)
		return (-1);
	return (EOF);
}
