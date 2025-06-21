/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   eval_if.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgamba <linogamba@pundalik.org>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 11:59:40 by lgamba            #+#    #+#             */
/*   Updated: 2025/03/17 11:59:41 by lgamba           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include <shell/shell.h>

t_eval_result
	eval_if(t_shell *shell, t_ast_node *cmd)
{
	size_t			i;
	t_eval_result	result;

	i = 0;
	while (i < cmd->st_if.nconds)
	{
		result = eval(shell, cmd->st_if.conds[i]);
		if (result.type != RES_NONE)
			return (result);
		if (shell->last_status)
		{
			++i;
			continue ;
		}
		result = eval(shell, cmd->st_if.bodies[i]);
		return (result);
		++i;
	}
	if (i < cmd->st_if.nbodies)
		return (eval(shell, cmd->st_if.bodies[i]));
	return ((t_eval_result){RES_NONE, 0});
}
