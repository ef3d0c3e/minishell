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
#include "parser/parser.h"
#include <shell/eval.h>

char
**resolve_command(t_environ *env, const struct s_node_cmd *cmd);

/** @brief Frees a command list */
static void
	cmd_free(char **cmd)
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

static inline void
	format_environ(size_t depth, t_rbnode *node, void *data)
{
	struct s_envp_traversal	*t = data;

	(void)depth;
	ft_asprintf(&t->envp[t->index], "%s=%s",
		(char *)node->key, (char *)node->data);
	++t->index;
}

static char
**env_to_envp(t_environ *env)
{
	struct s_envp_traversal	t;

	t.index = 0;
	t.envp = xmalloc((env->env.size + 1) * sizeof(char **));
	rb_apply(&env->env, format_environ, &t);
	t.envp[t.index] = NULL;
	return (t.envp);
}

static void
	eval_child(t_environ *env, char *path, char **argv)
{
	char					**envp;
	char					*err;
	size_t					i;
	
	if (!path)
		shell_exit(env, 127);
	envp = env_to_envp(env);
	env->last_status = execve(path, argv, envp);
	i = 0;
	cmd_free(envp);
	cmd_free(argv);
	ft_asprintf(&err, "Failed to execute `%s`: %m", path);
	free(path);
	shell_error(env, err, SRC_LOCATION);
	shell_exit(env, EXIT_FAILURE);
}

static void
	eval_builtin(t_environ *env, char **argv)
{
	const t_builtin			*builtin = rb_find(&env->builtins, argv[0]);
	int						argc;
	
	argc = 0;
	while (argv[argc])
		++argc;
	env->last_status = builtin->fn(env, argc, argv);
	cmd_free(argv);
}

int
	eval_cmd(t_environ *env, t_ast_node *program)
{
	pid_t	pid;
	int		status;
	char	**argv;
	char	*path;

	argv = resolve_command(env, &program->cmd);
	status = resolve_executable(env, argv[0],
		&path);
	if (status == 2)
		return (eval_builtin(env, argv), 1);
	if (status != 0)
	{
		free(path);
		return (0);
	}
	pid = shell_fork(env, SRC_LOCATION);
	if (pid == -1)
		return (0);
	if (pid)
	{
		if (waitpid(pid, &status, 0) == -1)
			shell_perror(env, "waitpid() failed", SRC_LOCATION);
		env->last_status = WEXITSTATUS(status);
	}
	else
		eval_child(env, path, argv);
	cmd_free(argv);
	free(path);
	return (1);
}
