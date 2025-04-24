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

void
	eval(t_shell *shell, t_ast_node* program)
{
	if (!program)
		return ;
	else if (program->type == NODE_COMMAND)
	{
		eval_cmd(shell, program);
	}
	else if (program->type == NODE_LOGIC)
	{
		if (!ft_strcmp(program->logic.token.reserved_word, "|")
			|| !ft_strcmp(program->logic.token.reserved_word, "|&"))
			eval_pipeline(shell, program);
		else
			eval_sequence(shell, program);
	}
	else if (program->type == NODE_SUBSHELL)
	{
		eval_subshell(shell, program);
	}
}
