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
#include "shell/opts.h"
#include <shell/eval.h>
#include <sys/types.h>
#include <unistd.h>

/** @brief Closes fds and waits for pipes execution */
static void
	pipe_wait(t_environ *env, int *fds, pid_t *pids, int redir_stderr)
{
	int	status[2];

	close(fds[0]);
	close(fds[1]);
	if (redir_stderr)
	{
		close(fds[2]);
		close(fds[3]);
	}
	if (waitpid(pids[0], &status[0], 0) == -1
		|| waitpid(pids[1], &status[1], 0) == -1)
		shell_perror(env, EXIT_FAILURE, "waitpid() failed", __FUNCTION__);
	if (!status[1] && option_get(env, "pipefail"))
		shell_exit(env, WEXITSTATUS(status[0]));
	shell_exit(env, WEXITSTATUS(status[1]));
}

/** @brief Runs pipe with stdout */
static void
	pipe_stdout(t_environ *env, t_ast_node* pipeline)
{
	int		fds[2];
	pid_t	pids[2];

	if (pipe(fds) == -1)
		shell_perror(env, EXIT_FAILURE, "pipe() failed", __FUNCTION__);
	pids[0] = shell_fork(env, __FUNCTION__);
	if (pids[0] == 0)
	{
		close(fds[0]);
		if (dup2(fds[1], STDOUT_FILENO) == -1)
			shell_perror(env, EXIT_FAILURE, "dup2() failed", __FUNCTION__);
		close(fds[1]);
		eval_cmd(env, pipeline->logic.left);
		shell_exit(env, env->last_status);
	}
	pids[1] = shell_fork(env, __FUNCTION__);
	if (pids[1] == 0)
	{
		close(fds[1]);
		if (dup2(fds[0], STDIN_FILENO) == -1)
			shell_perror(env, EXIT_FAILURE, "dup2() failed", __FUNCTION__);
		close(fds[0]);
		eval(env, pipeline->logic.right);
		shell_exit(env, env->last_status);
	}
	pipe_wait(env, fds, pids, 0);
}

/** @brief Runs pipe with stdout and stderr redirected */
static void
	pipe_stdout_stderr(t_environ *env, t_ast_node* pipeline)
{
	int		fds[4];
	pid_t	pids[2];

	if (pipe(fds) == -1 || pipe(fds + 2) == -1)
		shell_perror(env, EXIT_FAILURE, "pipe() failed", __FUNCTION__);
	pids[0] = shell_fork(env, __FUNCTION__);
	if (pids[0] == 0)
	{
		close(fds[0]);
		close(fds[2]);
		if (dup2(fds[1], STDOUT_FILENO) == -1
			|| dup2(fds[3], STDERR_FILENO) == -1)
			shell_perror(env, EXIT_FAILURE, "dup2() failed", __FUNCTION__);
		close(fds[1]);
		close(fds[3]);
		eval_cmd(env, pipeline->logic.left);
		shell_exit(env, env->last_status);
	}
	pids[1] = shell_fork(env, __FUNCTION__);
	if (pids[1] == 0)
	{
		close(fds[1]);
		close(fds[3]);
		if (dup2(fds[0], STDIN_FILENO) == -1
			|| dup2(fds[2], STDIN_FILENO) == -1)
			shell_perror(env, EXIT_FAILURE, "dup2() failed", __FUNCTION__);
		close(fds[0]);
		close(fds[2]);
		eval(env, pipeline->logic.right);
		shell_exit(env, env->last_status);
	}
	pipe_wait(env, fds, pids, 1);
}

void
	eval_pipeline(t_environ *env, t_ast_node* pipeline)
{
	const int	redir_stderr = pipeline->logic.token.reserved_word[1] == '&';
	int			status;
	pid_t		pid;
	char		*err;

	pid = fork();
	if (pid == -1)
	{
		ft_asprintf(&err, "fork() failed: %m");
		shell_error(env, err, __FUNCTION__);
		return ;
	}
	if (!pid)
	{
		if (redir_stderr)
			pipe_stdout_stderr(env, pipeline);
		else
			pipe_stdout(env, pipeline);
	}
	if (waitpid(pid, &status, 0) == -1)
		shell_perror(env, EXIT_FAILURE, "waitpid() failed", __FUNCTION__);
	env->last_status = WEXITSTATUS(status);
}
