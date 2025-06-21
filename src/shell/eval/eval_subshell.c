/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   eval_subshell.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgamba <linogamba@pundalik.org>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 11:59:40 by lgamba            #+#    #+#             */
/*   Updated: 2025/03/17 11:59:41 by lgamba           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include <shell/shell.h>

static void
	subshell_child(t_shell *shell, t_ast_node *program, t_redirs_stack *stack)
{
	t_eval_result	result;

	redir_stack_init(stack);
	do_redir(shell, stack, &program->sub.redirs);
	result = eval(shell, program->sub.head);
	undo_redir(shell, stack);
	if (result.type == RES_EXIT)
		shell->last_status = result.param;
	shell_exit(shell, shell->last_status);
}

t_eval_result
	eval_subshell(t_shell *shell, t_ast_node *program)
{
	pid_t			pid;
	int				status;
	t_redirs_stack	stack;

	pid = shell_fork(shell, SRC_LOCATION);
	if (pid == -1)
		shell_perror(shell, "fork() failed", SRC_LOCATION);
	if (pid)
	{
		while (waitpid(pid, &status, 0) == -1)
		{
			if (errno == EINTR)
				continue ;
			shell_perror(shell, "waitpid() failed", SRC_LOCATION);
		}
		shell->last_status = WEXITSTATUS(status);
	}
	else
		subshell_child(shell, program, &stack);
	return ((t_eval_result){RES_NONE, 0});
}
