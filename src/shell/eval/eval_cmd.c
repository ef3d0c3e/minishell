/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   eval.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgamba <linogamba@pundalik.org>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 11:59:40 by lgamba            #+#    #+#             */
/*   Updated: 2025/03/17 11:59:41 by lgamba           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include <shell/shell.h>

static void
	install_var(t_shell *shell, const char *name, char *value)
{
	t_shell_var	var;

	if (get_variable(shell, name, &var))
		set_variable(shell, name, value, var.exported);
	else
		set_variable(shell, name, value, 0);
}

/** @brief Evaluates command with no arguments */
static t_eval_result
	eval_empty(t_shell *shell, t_ast_node *cmd)
{
	t_redirs_stack	stack;
	size_t			i;
	char			*result;

	redir_stack_init(&stack);
	if (do_redir(shell, &stack, &cmd->cmd.redirs))
	{
		i = 0;
		while (i < cmd->cmd.nassigns)
		{
			result = word_expansion_cat(shell, &cmd->cmd.assigns[i].value);
			if (result)
				install_var(shell,
						stringbuf_cstr(&cmd->cmd.assigns[i].variable), result);
			++i;
		}
	}
	undo_redir(shell, &stack);
	return ((t_eval_result){RES_NONE, 0});
}

t_eval_result
	eval_cmd(t_shell *shell, t_ast_node *program)
{
	int		status;
	char	*err;
	char	**argv;
	char	*path;

	path = NULL;
	argv = word_expansion(shell, &program->cmd.args);
	if (!argv)
		return ((t_eval_result){RES_NONE, 0});
	status = 0;
	if (!argv[0])
		return (free(argv), eval_empty(shell, program));
	status = resolve_eval(shell, argv[0], &path);
	if (status == 1)
		return (eval_special(shell, program, argv));
	else if (status == 2)
		return (eval_function(shell, program, argv));
	else if (status == 3)
		return (eval_builtin(shell, program, argv));
	else if (status == 0)
		return (eval_exec(shell, program, path, argv));
	else
	{
		ft_asprintf(&err, "%s: command not found", argv[0]);
		shell_error(shell, err, SRC_LOCATION);
	}
	args_free(argv);
	return ((t_eval_result){RES_NONE, 0});
}
