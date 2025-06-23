/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgamba <linogamba@pundalik.org>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 11:59:40 by lgamba            #+#    #+#             */
/*   Updated: 2025/06/13 13:58:46 by thschnei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include <shell/shell.h>
#include <builtins/cowsay/cowsay.h>

static void
	set_property(void *ptr, const char **av)
{
	t_cowsay_args *const	args = ptr;

	if (!ft_strcmp(av[0], "-e"))
		args->eyes = av[1];
	else if (!ft_strcmp(av[0], "-m"))
		args->mouth = av[1];
	else if (!ft_strcmp(av[0], "-t"))
		args->tongue = av[1];
	else if (!ft_strcmp(av[0], "-u"))
		args->udder = av[1];
}

static void
	*init(void *a)
{
	static char			use[] = "cowsay [OPTIONS...] MESSAGE";
	static t_opt		opts[] = {
	{"-e", "TEXT Eyes string", set_property, 1},
	{"-m", "TEXT Mouth string", set_property, 1},
	{"-t", "TEXT Tongue string", set_property, 1},
	{"-u", "TEXT Udder string", set_property, 1},
	};
	const t_behavior	flags = GET_CALLER | NO_EXIT;

	return (_set_behavior(_init_opt(use, opts, *(&opts + 1) - opts, a), flags));
}

static int
	cowsay(t_shell *shell, int ac, char **av)
{
	int				ret;
	t_cowsay_args	args;
	t_string_buffer	msg;

	args.eyes = "oo";
	args.mouth = "__";
	args.tongue = "  ";
	args.udder = "w ";
	args.opts = init(&args);
	ret = _parse_args(args.opts, ac, (const char **)av);
	if (ret == ac - 1)
		msg = stringbuf_from(av[ac - 1]);
	else if (ret == ac)
	{
		stringbuf_init(&msg, 1024);
		read_fd_to_string(shell, STDIN_FILENO, &msg);
	}
	else
		return (1);
	cowsay_print(&args, stringbuf_cstr(&msg));
	stringbuf_free(&msg);
	return (0);
}

const t_builtin
	*builtin_cowsay(void)
{
	static const t_builtin	builtin = (t_builtin){
		.name = "cowsay",
		.desc = "The cowsay builtin",
		.run = cowsay,
		.init = NULL,
		.deinit = NULL,
	};

	return (&builtin);
}
