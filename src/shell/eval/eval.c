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

t_eval_result
	eval(t_shell *shell, t_ast_node* program)
{
	t_eval_result	result;
	size_t			i;

	result = (t_eval_result){RES_NONE, 0};
	if (!program)
		return (result);
	if (program->type == NODE_LIST)
	{
		// TODO: Apply separator logic
		i = 0;
		while (i < program->list.ncmds)
		{
			result = eval(shell, program->list.cmds[i++]);
			if (result.type != RES_NONE)
				break ;
		}
	}
	else if (program->type == NODE_BLOCK)
		return (eval(shell, program->block.inner));
	else if (program->type == NODE_COMMAND)
		return (eval_cmd(shell, program));
	else if (program->type == NODE_LOGIC)
		if (!ft_strcmp(program->logic.token.reserved_word, "|")
			|| !ft_strcmp(program->logic.token.reserved_word, "|&"))
			return (eval_pipeline(shell, program));
		else
			return (eval_sequence(shell, program));
	else if (program->type == NODE_SUBSHELL)
		return (eval_subshell(shell, program));
	else if (program->type == NODE_FUNCTION)
		return (eval_function_definition(shell, program));
	else if (program->type == NODE_IF)
		return (eval_if(shell, program));
	else if (program->type == NODE_FOR)
		return (eval_for(shell, program));
	return (result);
}
