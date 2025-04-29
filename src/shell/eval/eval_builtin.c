/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   eval_builtin.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgamba <linogamba@pundalik.org>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 11:59:40 by lgamba            #+#    #+#             */
/*   Updated: 2025/03/17 11:59:41 by lgamba           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include <shell/shell.h>


t_eval_result
	eval_builtin(t_shell *shell, t_ast_node *cmd, char **argv)
{
	const t_builtin			*builtin = rb_find(&shell->reg_builtins, argv[0]);
	int						argc;
	t_redirs_stack			stack;
	
	argc = 0;
	while (argv[argc])
		++argc;
	redir_stack_init(&stack);
	do_redir(shell, &stack, &cmd->cmd.redirs);
	if (shell_error_flush(shell))
		shell->last_status = builtin->run(shell, argc, argv);
	undo_redir(shell, &stack);
	args_free(argv);
	return ((t_eval_result){RES_NONE, 0});
}
