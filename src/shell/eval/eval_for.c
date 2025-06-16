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
#include "ft_printf.h"
#include "shell/eval/eval.h"
#include "util/util.h"
#include <shell/shell.h>

static int
	process_result(t_eval_result *result)
{
	int	cont;

	cont = 1;
	if ((result->type == RES_BREAK && result->param >= 1)
			|| (result->type == RES_CONTINUE && result->param >= 1)
			|| result->type == RES_RETURN || result->type == RES_EXIT
			|| result->type == RES_STOP)
	{
		if (result->type == RES_BREAK || result->type == RES_CONTINUE)
		{
			cont = 0;
			--result->param;
			if (!result->param)
			{
				if (result->type == RES_CONTINUE)
					cont = 1;
				result->type = RES_NONE;
			}
		}
	}
	return (cont);
}


t_eval_result
	eval_for(t_shell *shell, t_ast_node *cmd)
{
	size_t			i;
	t_eval_result	result;
	char			**argv;
	
	argv = word_expansion(shell, &cmd->st_for.args);
	if (!argv || !argv[0])
	{
		if (argv)
			args_free(argv);
		return ((t_eval_result){RES_NONE, 0});
	}
	rb_insert(&shell->atexit, argv, (void *)args_free);
	i = 0;
	while (argv[i])
	{
		set_variable(shell, cmd->st_for.ident, ft_strdup(argv[i]), 0);
		result = eval(shell, cmd->st_for.body);
		if (!process_result(&result))
			break ;
		++i;
	}
	rb_delete(&shell->atexit, argv);
	args_free(argv);
	return (result);
}
