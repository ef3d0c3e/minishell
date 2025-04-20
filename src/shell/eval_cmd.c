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
#include "builtins/builtin.h"
#include "parser/parser.h"
#include "shell/opts.h"
#include "util/util.h"
#include <shell/eval.h>

static char
**cmd_to_argv(const struct s_node_cmd *cmd, int *argc)
{
	char	**args;
	size_t	i;

	args = xmalloc((cmd->nargs + 1) * sizeof(char **));
	i = 0;
	while (i < cmd->nargs)
	{
		args[i] = stringbuf_cstr(&cmd->args[i].atom);
		++i;
	}
	args[i] = NULL;
	if (argc)
		*argc = i;
	return (args);
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
	eval_child(t_environ *env, char *path, t_ast_node *program)
{
	const struct s_node_cmd	*cmd = &program->cmd;
	char					**argv;
	char					**envp;
	char					*err;
	size_t					i;
	
	if (!path)
		shell_exit(env, 127);
	argv = cmd_to_argv(cmd, NULL);
	envp = env_to_envp(env);
	env->last_status = execve(path, argv, envp);
	i = 0;
	while (envp[i])
		free(envp[i++]);
	free(envp);
	free(argv);
	ft_asprintf(&err, "Failed to execute `%s`: %m", path);
	free(path);
	shell_error(env, err, SRC_LOCATION);
	shell_exit(env, EXIT_FAILURE);
}

static void
	eval_builtin(t_environ *env, t_ast_node *program)
{
	const struct s_node_cmd	*cmd = &program->cmd;
	const t_builtin			*builtin = rb_find(&env->builtins,
		stringbuf_cstr(&program->cmd.args[0].atom));
	char					**argv;
	int						argc;
	
	argv = cmd_to_argv(cmd, &argc);
	env->last_status = builtin->fn(env, argc, argv);
	free(argv);
}

int
	eval_cmd(t_environ *env, t_ast_node *program)
{
	pid_t	pid;
	int		status;
	char	*path;

	status = resolve_executable(env, stringbuf_cstr(&program->cmd.args[0].atom),
		&path);
	if (status == 2)
		return (eval_builtin(env, program), 1);
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
			shell_perror(env, EXIT_FAILURE, "waitpid() failed", SRC_LOCATION);
		env->last_status = WEXITSTATUS(status);
	}
	else
		eval_child(env, path, program);
	free(path);
	return (1);
}
