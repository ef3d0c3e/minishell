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
#include <shell/eval.h>

void
	eval_subshell(t_environ *env, t_ast_node *program)
{
	pid_t	pid;
	int		status;

	pid = shell_fork(env, SRC_LOCATION);
	if (pid == -1)
		shell_perror(env, "fork() failed", SRC_LOCATION);
	if (pid)
	{
		if (waitpid(pid, &status, 0) == -1)
			shell_perror(env, "waitpid() failed", SRC_LOCATION);
		env->last_status = WEXITSTATUS(status);
	}
	else
		eval(env, program->expr.head);
}
