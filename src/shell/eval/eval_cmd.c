/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   eval_cmd.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgamba <linogamba@pundalik.org>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 11:59:40 by lgamba            #+#    #+#             */
/*   Updated: 2025/06/19 17:39:22 by thschnei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "shell/env/env.h"
#include "util/util.h"
#include <shell/shell.h>
#include <stddef.h>

void
	eval_install_var(t_shell *shell, t_ast_node *cmd)
{
	t_shell_var	*var;
	size_t		i;
	char		*value;
	char		*name;

	i = 0;
	while (i < cmd->cmd.nassigns)
	{
		value = word_expansion_cat(shell, &cmd->cmd.assigns[i].value);
		if (!value && ++i)
			continue ;
		var = xmalloc(sizeof(t_shell_var));
		name = ft_strdup(stringbuf_cstr(&cmd->cmd.assigns[i].variable));
		var->name = name;
		var->value = value;
		var->exported = 1;
		rb_insert(&shell->reg_env, name, var);
		++i;
	}
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
		eval_install_var(shell, cmd);
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
	size_t	cnt;

	path = NULL;
	argv = word_expansion(shell, &program->cmd.args);
	if (g_signal)
		return (args_free(argv), (t_eval_result){RES_STOP, 0});
	if (!argv)
		return ((t_eval_result){RES_NONE, 0});
	status = 0;
	if (!argv[0])
		return (free(argv), eval_empty(shell, program));
	status = resolve_eval(shell, argv[0], &path);
	cnt = 0;
	while (argv[cnt])
		cnt++;
	set_env_variable(shell, "_", ft_strdup(argv[cnt - 1]));
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
