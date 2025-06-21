/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   eval_loop.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgamba <linogamba@pundalik.org>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 11:59:40 by lgamba            #+#    #+#             */
/*   Updated: 2025/03/17 11:59:41 by lgamba           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include <shell/shell.h>

t_eval_result
	eval_loop(t_shell *shell, t_ast_node *cmd)
{
	t_eval_result	result;

	while (1)
	{
		result = eval(shell, cmd->st_loop.cond);
		if ((result.type == RES_BREAK && result.param > 0)
			|| result.type == RES_RETURN || result.type == RES_STOP)
		{
			if (result.type == RES_BREAK)
				--result.type;
			return (result);
		}
		if ((cmd->st_loop.until && !shell->last_status)
			|| (!cmd->st_loop.until && shell->last_status))
			break ;
		result = eval(shell, cmd->st_loop.body);
		if ((result.type == RES_BREAK && result.param > 0)
			|| result.type == RES_RETURN || result.type == RES_STOP)
		{
			if (result.type == RES_BREAK)
				--result.type;
			return (result);
		}
	}
	return ((t_eval_result){RES_NONE, 0});
}
