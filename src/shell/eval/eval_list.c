/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   eval_list.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgamba <linogamba@pundalik.org>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 11:59:40 by lgamba            #+#    #+#             */
/*   Updated: 2025/03/17 11:59:41 by lgamba           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "shell/eval/eval.h"
#include <shell/shell.h>

t_eval_result
	eval_list(t_shell *shell, t_ast_node *cmd)
{
	size_t			i;
	t_eval_result	result;

	// TODO: Apply separator logic
	i = 0;
	result = (t_eval_result){RES_NONE, 0};
	while (i < cmd->list.ncmds)
	{
		result = eval(shell, cmd->list.cmds[i++]);
		if (result.type != RES_NONE)
			break ;
	}
	return (result);
}
