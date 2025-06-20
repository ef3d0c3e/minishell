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
	print_options(size_t depth, t_rbnode *node, void *cookie)
{
	const struct s_option	*opt = node->data;

	(void)depth;
	(void)cookie;
	ft_printf("%s: %d (%s)\n", node->key, opt->value, opt->desc);
}

static int
	shopt(t_shell *shell, int argc, char **argv)
{
	int				enable;

	if (argc == 1)
		return (rb_apply(&shell->options, print_options, NULL), 0);
	else if (argc != 3
		|| (ft_strcmp(argv[1], "-s") && ft_strcmp(argv[1], "-u")))
		return (ft_dprintf(2, "USAGE: shopt (-s|-u) OPTION\n"), 1);
	enable = !ft_strcmp(argv[1], "-s");
	if (!option_set(shell, argv[2], enable))
	{
		ft_dprintf(2, "Unknown shell option: '%s'\n", argv[2]);
		return (1);
	}
	return (0);
}

const t_builtin
	*builtin_shopt(void)
{
	static const t_builtin	builtin = (t_builtin){
		.name = "shopt",
		.desc = "The shopt builtin",
		.run = shopt,
		.init = NULL,
		.deinit = NULL,
	};

	return (&builtin);
}
