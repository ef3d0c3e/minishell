/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   eval_pipel.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgamba <linogamba@pundalik.org>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 11:59:40 by lgamba            #+#    #+#             */
/*   Updated: 2025/03/17 11:59:41 by lgamba           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "parser/parser.h"
#include <shell/eval.h>
#include <sys/wait.h>
#include <unistd.h>

static void
	pipe_child(t_environ *env, t_ast_node *pipeline, int redir_stderr, int *fds)
{
	char	*err;

	close(fds[1]);
	if (redir_stderr)
		close(fds[3]);
	if (dup2(fds[0], STDIN_FILENO) == -1
		|| (redir_stderr && dup2(fds[2], STDIN_FILENO) == -1))
	{
		ft_asprintf(&err, "pipe() failed: %m");
		shell_error(env, err, __FUNCTION__);
		shell_exit(env, EXIT_FAILURE);
	}
	eval(env, pipeline->logic.right);
	close(fds[0]);
	if (redir_stderr)
		close(fds[2]);
	shell_exit(env, env->last_status);
}

static void
	pipe_parent(t_environ *env, t_ast_node* pipeline)
{
	const int	redir_stderr = pipeline->logic.token.reserved_word[1] == '&';
	pid_t	pid;
	int		fds[4];
	char	*err;
	int		status;

	if (pipe(fds) == -1 || (redir_stderr && pipe(fds + 2) == -1))
	{
		ft_asprintf(&err, "pipe() failed: %m");
		shell_error(env, err, __FUNCTION__);
		shell_exit(env, EXIT_FAILURE);
	}
	pid = fork();
	if (pid == -1)
	{
		ft_asprintf(&err, "fork() failed: %m");
		shell_error(env, err, __FUNCTION__);
		shell_exit(env, EXIT_FAILURE);
	}
	if (pid)
	{
		close(fds[0]);
		if (redir_stderr)
			close(fds[2]);
		if (dup2(fds[1], STDOUT_FILENO) == -1
			|| (redir_stderr && dup2(fds[3], STDOUT_FILENO) == -1))
		{
			ft_asprintf(&err, "pipe() failed: %m");
			shell_error(env, err, __FUNCTION__);
			shell_exit(env, EXIT_FAILURE);
		}
		eval_cmd(env, pipeline->logic.left);
		if (waitpid(pid, &status, 0) == -1)
		{
			ft_asprintf(&err, "waitpid() failed: %m");
			shell_error(env, err, __FUNCTION__);
			shell_exit(env, EXIT_FAILURE);
		}
		if (status)
			env->last_status = status;
	}
	else
		pipe_child(env, pipeline, redir_stderr, fds);
	close(fds[1]);
	if (redir_stderr)
		close(fds[3]);
	shell_exit(env, env->last_status);
}

void
	eval_pipeline(t_environ *env, t_ast_node* pipeline)
{
	int		status;
	char	*err;
	pid_t	pid;
	pipe_parent(env, pipeline);

	pid = fork();
	if (pid == -1)
	{
		ft_asprintf(&err, "fork() failed: %m");
		shell_error(env, err, __FUNCTION__);
		return ;
	}
	if (pid)
	{
		if (waitpid(pid, &status, 0) == -1)
		{
			ft_asprintf(&err, "waitpid() failed: %m");
			shell_error(env, err, __FUNCTION__);
			shell_exit(env, EXIT_FAILURE);
		}
	}
	else
		pipe_parent(env, pipeline);
}
