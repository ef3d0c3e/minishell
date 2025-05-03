/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   eval_special.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgamba <linogamba@pundalik.org>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 11:59:40 by lgamba            #+#    #+#             */
/*   Updated: 2025/03/17 11:59:41 by lgamba           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include <shell/shell.h>

static t_eval_result
	special_return(t_shell *shell, char **av)
{
	int		num;
	char	*err;

	num = 0;
	if (!shell->eval_stack.size)
	{
		ft_asprintf(&err, "'return' is only valid inside functions", av[1]);
		shell_error(shell, err, SRC_LOCATION);
	}
	if (av[1] && !atoi_checked(av[1], &num))
	{
		ft_asprintf(&err, "return: numeric argument required, got: `%s` ",
			av[1]);
		shell_error(shell, err, SRC_LOCATION);
		if (av[2])
		{
			ft_asprintf(&err, "return: expected one numeric argument");
			shell_error(shell, err, SRC_LOCATION);
		}
	}
	return ((t_eval_result){RES_RETURN, num});
}

static t_eval_result
	special_break(t_shell *shell, char **av)
{
	int		num;
	char	*err;

	num = 1;
	// TODO: Loop context...
	//if (!shell->eval_stack.size)
	//{
	//	ft_asprintf(&err, "'return' is only valid inside functions", av[1]);
	//	shell_error(shell, err, SRC_LOCATION);
	//}
	if (av[1] && !atoi_checked(av[1], &num))
	{
		ft_asprintf(&err, "break: numeric argument required, got: `%s` ",
			av[1]);
		shell_error(shell, err, SRC_LOCATION);
		if (av[2])
		{
			ft_asprintf(&err, "break: expected one numeric argument");
			shell_error(shell, err, SRC_LOCATION);
		}
		num = 1;
	}
	return ((t_eval_result){RES_BREAK, num});
}

static t_eval_result
	special_continue(t_shell *shell, char **av)
{
	int		num;
	char	*err;

	num = 1;
	// TODO: Loop context...
	//if (!shell->eval_stack.size)
	//{
	//	ft_asprintf(&err, "'return' is only valid inside functions", av[1]);
	//	shell_error(shell, err, SRC_LOCATION);
	//}
	if (av[1] && !atoi_checked(av[1], &num))
	{
		ft_asprintf(&err, "continue: numeric argument required, got: `%s` ",
			av[1]);
		shell_error(shell, err, SRC_LOCATION);
		if (av[2])
		{
			ft_asprintf(&err, "continue: expected one numeric argument");
			shell_error(shell, err, SRC_LOCATION);
		}
		num = 1;
	}
	return ((t_eval_result){RES_CONTINUE, num});
}

static t_eval_result
	special_exit(t_shell *shell, char **av)
{
	int		num;
	char	*err;

	num = 0;
	if (av[1] && !atoi_checked(av[1], &num))
	{
		ft_asprintf(&err, "exit: numeric argument required, got: `%s` ",
			av[1]);
		shell_error(shell, err, SRC_LOCATION);
		if (av[2])
		{
			ft_asprintf(&err, "exit: expected one numeric argument");
			shell_error(shell, err, SRC_LOCATION);
		}
		num = 0;
	}
	shell->last_status = num;
	return ((t_eval_result){RES_EXIT, 0});
}

t_eval_result
	eval_special(t_shell *shell, t_ast_node *cmd, char **av)
{
	t_eval_result	result;

	(void)cmd;
	result = (t_eval_result){RES_NONE, 0};
	if (!ft_strcmp(av[0], "return"))
		result = special_return(shell, av);
	else if (!ft_strcmp(av[0], "break"))
		result = special_break(shell, av);
	else if (!ft_strcmp(av[0], "continue"))
		result = special_continue(shell, av);
	else if (!ft_strcmp(av[0], "exit"))
		result = special_exit(shell, av);
	else
		shell_fail(shell, "Invalid special builtin", SRC_LOCATION);
	args_free(av);
	return (result);
}
