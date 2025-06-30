/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   eval_exec.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgamba <linogamba@pundalik.org>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 11:59:40 by lgamba            #+#    #+#             */
/*   Updated: 2025/03/17 11:59:41 by lgamba           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include <shell/shell.h>

void
	args_free(char **cmd)
{
	size_t	i;

	i = 0;
	if (!cmd)
		return ;
	while (cmd[i])
	{
		free(cmd[i]);
		++i;
	}
	free(cmd);
}

static void
	eval_exec_child(t_shell *shell, t_ast_node *cmd, char *path, char **argv)
{
	char					**shellp;
	char					*err;
	t_redirs_stack			stack;

	if (argv[0] && !path)
		shell_exit(shell, 127);
	eval_install_var(shell, cmd);
	shellp = environ_to_envp(shell);
	redir_stack_init(&stack);
	if (do_redir(shell, &stack, &cmd->cmd.redirs))
		shell->last_status = execve(path, argv, shellp);
	undo_redir(shell, &stack);
	args_free(shellp);
	args_free(argv);
	if (!path)
		shell_exit(shell, EXIT_SUCCESS);
	ft_asprintf(&err, "Failed to execute `%s`: %m", path);
	free(path);
	shell_error(shell, err, SRC_LOCATION);
	shell_exit(shell, EXIT_FAILURE);
}

static int
	eval_exec_parent(t_shell *shell, t_ast_node *cmd, char *path, char **argv)
{
	pid_t	pid;
	int		status;

	status = 0;
	pid = shell_fork(shell, SRC_LOCATION);
	if (pid == -1)
		shell_perror(shell, "fork() failed", SRC_LOCATION);
	if (!pid)
		eval_exec_child(shell, cmd, path, argv);
	while (waitpid(pid, &status, 0) != pid)
	{
		if (errno == EINTR)
			continue ;
		if (errno == ECHILD)
		{
			status = 2;
			break ;
		}
		shell_perror(shell, "waitpid() failed", SRC_LOCATION);
	}
	shell->last_status = WEXITSTATUS(status);
	if (WIFSIGNALED(status))
		shell->last_status = 128 + WTERMSIG(status);
	if (argv[0])
		free(path);
	return (WIFEXITED(status));
}

t_eval_result
	eval_exec(t_shell *shell, t_ast_node *cmd, char *path, char **argv)
{
	int	status;

	status = eval_exec_parent(shell, cmd, path, argv);
	args_free(argv);
	if (!status)
		return ((t_eval_result){RES_STOP, 0});
	return ((t_eval_result){RES_NONE, 0});
}
