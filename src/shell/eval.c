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

void
	shell_exit(t_environ *env, int status)
{
	if (!shell_error_flush(env))
		status = -1;
	env_free(env);
	exit(status);
}

void
	shell_fail(t_environ *env, const char *msg, const char *func)
{
	shell_error(env, ft_strdup(msg), func);
	shell_exit(env, EXIT_FAILURE);
}

void
	shell_perror(t_environ *env, const char *msg, const char *func)
{
	char	*err;

	ft_asprintf(&err, "%s: %m", msg);
	shell_error(env, err, func);
	shell_exit(env, EXIT_FAILURE);
}

void
	eval(t_environ *env, t_ast_node* program)
{
	if (!program)
		return ;
	else if (program->type == NODE_COMMAND)
	{
		eval_cmd(env, program);
	}
	else if (program->type == NODE_LOGIC)
	{
		if (!ft_strcmp(program->logic.token.reserved_word, "|")
			|| !ft_strcmp(program->logic.token.reserved_word, "|&"))
			eval_pipeline(env, program);
		else
			eval_sequence(env, program);
	}
	else if (program->type == NODE_SUBSHELL)
	{
		eval_subshell(env, program);
	}
}
