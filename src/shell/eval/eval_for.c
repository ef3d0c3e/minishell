/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   eval_for.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgamba <linogamba@pundalik.org>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 11:59:40 by lgamba            #+#    #+#             */
/*   Updated: 2025/03/17 11:59:41 by lgamba           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include <shell/shell.h>

t_eval_result
	eval_for(t_shell *shell, t_ast_node *cmd)
{
	size_t			i;
	t_eval_result	result;
	char			**argv;
	
	argv = word_expansion(shell, &cmd->st_for.args);
	if (!argv)
		return ((t_eval_result){RES_NONE, 0});
	i = 0;
	while (argv[i])
	{
		set_variable(shell, cmd->st_for.ident, ft_strdup(argv[i]), 0);
		result = eval(shell, cmd->st_for.body);
		if (result.type == RES_CONTINUE && result.param == 1 && ++i)
			continue ;
		if ((result.type == RES_BREAK && result.param >= 1)
			|| (result.type == RES_CONTINUE && result.param >= 1)
			|| result.type == RES_RETURN || result.type == RES_EXIT
			|| result.type == RES_STOP)
		{
			if (result.type != RES_CONTINUE)
				args_free(argv);
			if (result.type == RES_BREAK || result.type == RES_CONTINUE)
			{
				--result.param;
				if (!result.param)
					result.type = RES_NONE;
			}
			return (result);
		}
		++i;
	}
	args_free(argv);
	return ((t_eval_result){RES_NONE, 0});
}
