/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgamba <linogamba@pundalik.org>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 11:59:40 by lgamba            #+#    #+#             */
/*   Updated: 2025/06/13 13:58:46 by thschnei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include <shell/shell.h>

static void
	set_property(void *ptr, const char **av)
{
	t_complete_args *const	args = ptr;

	if (!ft_strcmp(av[0], "-c"))
	{
		args->cmd = av[1];
		return ;
	}
	if (!args->cmd)
	{
		args->fail = 1;
		ft_dprintf(2, "Expected command name");
		return ;
	}
	if (!ft_strcmp(av[0], "-s"))
		args->shortname = av[1];
	else if (!ft_strcmp(av[0], "-l"))
		args->longname = av[1];
	else
		args->desc = av[1];
}

static void
	*init(void *a)
{
	static char			use[] = "complete [OPTIONS...]";
	static t_opt		opts[] = {
	{"-c", "NAME Command name", set_property, 1},
	{"-s", "SHORTNAME Argument shortname", set_property, 1},
	{"-l", "LONGMAME Argument longname", set_property, 1},
	{"-d", "Description Description", set_property, 1},
	};
	const t_behavior	flags = GET_CALLER | NO_EXIT;

	return (_set_behavior(_init_opt(use, opts, *(&opts + 1) - opts, a), flags));
}

t_cmd_comp_opt
	get_comp(t_complete_args *args)
{
	t_cmd_comp_opt	opt;

	ft_memset(&opt, 0, sizeof(t_cmd_comp_opt));
	if (args->desc)
		opt.description = ft_strdup(args->desc);
	if (args->shortname)
		opt.shortname = ft_strdup(args->shortname);
	if (args->longname)
		opt.longname = ft_strdup(args->longname);
	return (opt);
}

static int
	complete(t_shell *shell, int ac, char **av)
{
	int				ret;
	t_complete_args	args;

	ft_memset(&args, 0, sizeof(t_complete_args));
	args.shell = shell;
	args.opts = init(&args);
	ret = _parse_args(args.opts, ac, (const char **)av);
	if (!ret)
		return (1);
	else if (args.fail || !args.cmd
		|| (!args.desc && !args.longname && !args.shortname))
		return (1);
	complete_add(shell, args.cmd, get_comp(&args));
	return (0);
}

const t_builtin
	*builtin_complete(void)
{
	static const t_builtin	builtin = (t_builtin){
		.name = "complete",
		.desc = "The complete builtin",
		.run = complete,
		.init = NULL,
		.deinit = NULL,
	};

	return (&builtin);
}
