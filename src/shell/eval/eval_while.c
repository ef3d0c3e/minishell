/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   eval_while.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgamba <linogamba@pundalik.org>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 11:59:40 by lgamba            #+#    #+#             */
/*   Updated: 2025/03/17 11:59:41 by lgamba           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include <shell/shell.h>

t_eval_result
	eval_while(t_shell *shell, t_ast_node *cmd)
{
	t_eval_result	result;
	
	while (1)
	{
		result = eval(shell, cmd->st_while.cond);
		if ((result.type == RES_BREAK && result.param > 0)
			|| result.type == RES_RETURN)
		{
			if (result.type == RES_BREAK)
				--result.type;
			return (result);
		}
		if (shell->last_status)
			break;
		result = eval(shell, cmd->st_while.body);
		if ((result.type == RES_BREAK && result.param > 0)
			|| result.type == RES_RETURN)
		{
			if (result.type == RES_BREAK)
				--result.type;
			return (result);
		}
	}
	return ((t_eval_result){RES_NONE, 0});
}
