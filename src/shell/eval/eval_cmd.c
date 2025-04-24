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
#include <shell/redir/eval_redir.h>
#include <shell/eval.h>

/** @brief Frees a command list */
static void
	arglist_free(char **cmd)
{
	size_t	i;

	i = 0;
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
	char					**envp;
	char					*err;
	t_redirs_stack			stack;
	
	if (!path)
		shell_exit(env, 127);
	envp = environ_to_envp(env);
	redir_stack_init(&stack);
	do_redir(env, &stack, &cmd->cmd.redirs);
	if (shell_error_flush(env))
		env->last_status = execve(path, argv, envp);
	undo_redir(env, &stack);
	arglist_free(envp);
	arglist_free(argv);
	ft_asprintf(&err, "Failed to execute `%s`: %m", path);
	free(path);
	shell_error(env, err, SRC_LOCATION);
	shell_exit(env, EXIT_FAILURE);
}

static void
	eval_exec_parent(t_shell *shell, t_ast_node *cmd, char *path, char **argv)
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
		free(path);
	}
	else
		eval_exec_child(env, cmd, path, argv);
}

static void
	eval_builtin(t_shell *shell, t_ast_node *cmd, char **argv)
{
	const t_builtin			*builtin = rb_find(&env->builtins, argv[0]);
	int						argc;
	t_redirs_stack			stack;
	
	argc = 0;
	while (argv[argc])
		++argc;
	redir_stack_init(&stack);
	do_redir(env, &stack, &cmd->cmd.redirs);
	if (shell_error_flush(env))
		env->last_status = builtin->fn(env, argc, argv);
	undo_redir(env, &stack);
}

int
	eval_cmd(t_shell *shell, t_ast_node *program)
{
	int		status;
	char	*err;
	char	**argv;
	char	*path;

	argv = command_to_argv(env, &program->cmd);
	status = resolve_eval(env, argv[0],
		&path);
	if (status == 2)
		eval_builtin(env, program, argv);
	else if (status == 0)
		eval_exec_parent(env, program, path, argv);
	else
	{
		ft_asprintf(&err, "%s: command not found", argv[0]);
		shell_error(env, err, SRC_LOCATION);
	}
	arglist_free(argv);
	return (status != -1);
}
