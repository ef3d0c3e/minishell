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

void
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
		if (waitpid(pid, &status, 0) == -1)
			shell_perror(shell, "waitpid() failed", SRC_LOCATION);
		shell->last_status = WEXITSTATUS(status);
	}
	else
	{
		redir_stack_init(&stack);
		do_redir(shell, &stack, &program->expr.redirs);
		eval(shell, program->expr.head);
		undo_redir(shell, &stack);
		shell_exit(shell, shell->last_status);
	}
}
