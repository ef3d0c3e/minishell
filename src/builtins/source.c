/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   source.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgamba <linogamba@pundalik.org>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 11:59:40 by lgamba            #+#    #+#             */
/*   Updated: 2025/03/17 11:59:41 by lgamba           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include <shell/shell.h>

static int
	source(t_shell *shell, int argc, char **argv)
{
	t_string_buffer	buf;
	ssize_t			nread;
	int				fd;

	if (argc != 2)
	{
		ft_dprintf(2, "USAGE: source FILE\n");
		return (1);
	}
	fd = open(argv[1], O_RDONLY);
	if (fd < 0)
	{
		ft_dprintf(2, "Failed to open '%s': %m\n", argv[1]);
		return (1);
	}
	stringbuf_init(&buf, 1024);
	nread = read(fd, buf.str + buf.len, 1024);
	buf.len += nread;
	while (nread > 0)
	{
		stringbuf_reserve(&buf, buf.len + 1024);
		nread = read(fd, buf.str + buf.len, 1024);
		buf.len += nread;
	}
	if (nread < 0)
	{
		ft_dprintf(2, "Read error on '%s': %m\n", argv[1]);
		close(fd);
		stringbuf_free(&buf);
		return (1);
	}
	close(fd);
	ctx_eval_stdout(shell, stringbuf_cstr(&buf));
	return (0);
}

const t_builtin
	*builtin_source(void)
{
	static const t_builtin	builtin = (t_builtin){
		.name = "source",
		.desc = "The source builtin",
		.run = source,
		.init = NULL,
		.deinit = NULL,
	};

	return (&builtin);
}
