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
#include <shell/shell.h>

static t_eval_result
	eval_block(t_shell *shell, t_ast_node *program)
{
	return (eval(shell, program->block.inner));
}

t_eval_result
	eval(t_shell *shell, t_ast_node* program)
{
	static t_eval_result(*const evaluators[])(t_shell *, t_ast_node *) = {
	[NODE_LIST] = eval_list,
	[NODE_BLOCK] = eval_block,
	[NODE_COMMAND] = eval_cmd,
	[NODE_PIPE] = eval_pipeline,
	[NODE_BINARY] = eval_binary,
	[NODE_SUBSHELL] = eval_subshell,
	[NODE_FUNCTION] = eval_function_definition,
	[NODE_IF] = eval_if,
	[NODE_LOOP] = eval_loop,
	[NODE_FOR] = eval_for,
	};

	if (g_signal == SIGINT)
		return ((t_eval_result){RES_STOP, 0});
	if (!program)
		return ((t_eval_result){RES_NONE, 0});
	return (evaluators[program->type](shell, program));
}
