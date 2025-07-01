/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   eval_pipe.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgamba <linogamba@pundalik.org>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 11:59:40 by lgamba            #+#    #+#             */
/*   Updated: 2025/03/17 11:59:41 by lgamba           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include <shell/shell.h>
#include <stdlib.h>

/** @brief Closes fds and waits for pipes execution */
static void
	pipe_wait(t_shell *shell, int *fds, pid_t *pids)
{
	int	status[2];

	close(fds[0]);
	close(fds[1]);
	while (waitpid(pids[0], &status[0], 0) == -1)
	{
		if (errno == EINTR)
			continue ;
		shell_perror(shell, "waitpid() failed", SRC_LOCATION);
	}
	while (waitpid(pids[1], &status[1], 0) == -1)
	{
		if (errno == EINTR)
			continue ;
		shell_perror(shell, "waitpid() failed", SRC_LOCATION);
	}
	if (!status[1] && option_value(shell, "pipefail"))
		shell_exit(shell, WEXITSTATUS(status[0]));
	shell_exit(shell, WEXITSTATUS(status[1]));
}

/** @brief Runs pipe with stdout */
static void
	pipe_stdout(t_shell *shell, t_ast_node *pipeline)
{
	int		fds[2];
	pid_t	pids[2];

	if (pipe(fds) == -1)
		shell_perror(shell, "pipe() failed", SRC_LOCATION);
	pids[0] = shell_fork(shell, SRC_LOCATION);
	if (pids[0] == 0)
	{
		if ((close(fds[0]), 1) && dup2(fds[1], STDOUT_FILENO) == -1)
			shell_perror(shell, "dup2() failed", SRC_LOCATION);
		close(fds[1]);
		eval(shell, pipeline->logic.left);
		shell_exit(shell, shell->last_status);
	}
	pids[1] = shell_fork(shell, SRC_LOCATION);
	if (pids[1] == 0)
	{
		if ((close(fds[1]), 1) && dup2(fds[0], STDIN_FILENO) == -1)
			shell_perror(shell, "dup2() failed", SRC_LOCATION);
		close(fds[0]);
		eval(shell, pipeline->logic.right);
		shell_exit(shell, shell->last_status);
	}
	pipe_wait(shell, fds, pids);
}

/** @brief Runs pipe with stdout and stderr redirected */
static void
	pipe_stdout_stderr(t_shell *shell, t_ast_node *pipeline)
{
	int		fds[2];
	pid_t	pids[2];

	if (pipe(fds) == -1)
		shell_perror(shell, "pipe() failed", SRC_LOCATION);
	pids[0] = shell_fork(shell, SRC_LOCATION);
	if (pids[0] == 0)
	{
		if ((close(fds[0]), 1) && (dup2(fds[1], STDOUT_FILENO) == -1
				|| dup2(fds[1], STDERR_FILENO) == -1))
			shell_perror(shell, "dup2() failed", SRC_LOCATION);
		close(fds[1]);
		eval(shell, pipeline->logic.left);
		shell_exit(shell, shell->last_status);
	}
	pids[1] = shell_fork(shell, SRC_LOCATION);
	if (pids[1] == 0)
	{
		if ((close(fds[1]), 1) && dup2(fds[0], STDIN_FILENO) == -1)
			shell_perror(shell, "dup2() failed", SRC_LOCATION);
		close(fds[0]);
		eval(shell, pipeline->logic.right);
		shell_exit(shell, shell->last_status);
	}
	pipe_wait(shell, fds, pids);
}

t_eval_result
	eval_pipeline(t_shell *shell, t_ast_node *pipeline)
{
	const int		r_stderr = pipeline->logic.token.reserved_word[1] == '&';
	int				status;
	pid_t			pid;
	char			*err;

	pid = fork();
	if (pid == -1)
	{
		ft_asprintf(&err, "fork() failed: %m");
		shell_error(shell, err, SRC_LOCATION);
		return ((t_eval_result){RES_NONE, 0});
	}
	if (!pid && r_stderr)
		pipe_stdout_stderr(shell, pipeline);
	else if (!pid)
		pipe_stdout(shell, pipeline);
	while (waitpid(pid, &status, 0) == -1)
	{
		if (errno == EINTR)
			continue ;
		shell_perror(shell, "waitpid() failed", SRC_LOCATION);
	}
	shell->last_status = WEXITSTATUS(status);
	if (WIFSIGNALED(status))
	{
		const int	sig = WTERMSIG(status);

		shell->last_status = 0;
		if (sig != SIGPIPE)
		{
			ft_dprintf(2, "Process %d terminated with signal %s (%d)\n", pid, eval_signal_name(sig), sig);
			shell->last_status = 128 + sig;
		}
	}
	return ((t_eval_result){RES_NONE, 0});
}
