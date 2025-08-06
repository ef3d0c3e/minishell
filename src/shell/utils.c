/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgamba <linogamba@pundalik.org>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 11:59:40 by lgamba            #+#    #+#             */
/*   Updated: 2025/03/17 11:59:41 by lgamba           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include <shell/shell.h>

/** @brief Reads incoming data on file descriptor `fd` to buffer `buf` */
void
	read_fd_to_string(t_shell *shell, int fd, t_string_buffer *buf)
{
	ssize_t sz;

	while (1)
	{
		sz = read(fd, buf->str + buf->len, 1024);
		if (sz < 0)
		{
			if (errno == EINTR)
				continue; // Retry the read
			shell_perror(shell, "read() failed", SRC_LOCATION);
			break;
		}
		else if (sz == 0)
			break; // EOF

		buf->len += sz;
		stringbuf_reserve(buf, buf->len + 1024);
	}
}
