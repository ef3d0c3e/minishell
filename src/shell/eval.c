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
	shell_error_flush(env);
	env_free(env);
	_exit(status);
}

void
	eval(t_environ *env, t_ast_node* program)
{
	if (program->type == NODE_COMMAND)
		eval_cmd(env, program);
	else if (program->type == NODE_LOGIC)
	{
		if (program->logic.token.reserved_word[0] == '|')
			eval_pipeline(env, program);
	}
	shell_error_flush(env);
}
