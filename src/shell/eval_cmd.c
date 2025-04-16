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
#include <shell/eval.h>

char
**cmd_to_argv(const struct s_node_cmd *cmd)
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

char
**env_to_envp(t_environ *env)
{
	struct s_envp_traversal	t;

	t.index = 0;
	t.envp = xmalloc((env->env.size + 1) * sizeof(char **));
	rb_apply(&env->env, format_environ, &t);
	t.envp[t.index] = NULL;
	return (t.envp);
}

char
*resolve_path(t_environ *env, t_string_buffer *buf)
{
	char *const	expanded = stringbuf_cstr(buf);
	char *const	resolved = rb_find(&env->path_program, expanded);
	char		*msg;

	if (!resolved)
	{
		ft_asprintf(&msg, "Failed to find executable `%s`", expanded);
		shell_error(env, msg, __func__);
		return (NULL);
	}
	return (resolved);
}

void
	eval_cmd(t_environ *env, t_ast_node* program)
{
	const struct s_node_cmd	*cmd = &program->cmd;
	const char				*path = resolve_path(env, &cmd->args[0].atom);
	char					**argv;
	char					**envp;
	size_t					i;
	
	if (!path)
		return ;
	argv = cmd_to_argv(cmd);
	envp = env_to_envp(env);
	env->last_status = execve(path, argv, envp);
	i = 0;
	while (envp[i])
		free(envp[i++]);
	free(envp);
	free(argv);
}
