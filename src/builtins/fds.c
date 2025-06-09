/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgamba <linogamba@pundalik.org>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 11:59:40 by lgamba            #+#    #+#             */
/*   Updated: 2025/03/17 11:59:41 by lgamba           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include <shell/shell.h>

static void
	print_fn(size_t depth, t_rbnode *node, void *cookie)
{
	char			*msg;

	(void)depth;
	(void)cookie;
	msg = fd_print(node->data);
	ft_printf("%s\n", msg);
	free(msg);
}

static int
	fds(t_shell *shell, int argc, char **argv)
{
	(void)argc;
	(void)argv;
	rb_apply(&shell->reg_fds, print_fn, NULL);
	return (0);
}

static void
	fds_init(t_shell *shell)
{
	t_fd_data	*data;

	data = xmalloc(sizeof(t_fd_data));
	*data = fd_data_from(FDT_OPEN, ft_strdup("[stdin]"), O_RDWR, 0);
	rb_insert(&shell->reg_fds, (void *)(ptrdiff_t)STDIN_FILENO, data);
	data = xmalloc(sizeof(t_fd_data));
	*data = fd_data_from(FDT_OPEN, ft_strdup("[stdout]"), O_RDWR, 0);
	rb_insert(&shell->reg_fds, (void *)(ptrdiff_t)STDOUT_FILENO, data);
	data = xmalloc(sizeof(t_fd_data));
	*data = fd_data_from(FDT_OPEN, ft_strdup("[stderr]"), O_RDWR, 0);
	rb_insert(&shell->reg_fds, (void *)(ptrdiff_t)STDERR_FILENO, data);
}

const t_builtin
	*builtin_fds(void)
{
	static const t_builtin	builtin = (t_builtin){
		.name = "fds",
		.desc = "List all used general purpose file descriptors",
		.run = fds,
		.init = fds_init,
		.deinit = NULL,
	};

	return (&builtin);
}
