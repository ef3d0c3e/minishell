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
#include "shell/eval/eval.h"
#include <shell/shell.h>

/** @brief Sets the variable for the for loop, if there exists a local variable
 * by the same name, it is used instead */
static void
	set_variable(t_shell *shell, const char *name, char *value)
{
	t_stack_frame *const	frame = &shell->eval_stack.frames
		[shell->eval_stack.size - 1];
	char					*found;

	found = NULL;
	if (shell->eval_stack.size)
		found = rb_find(&frame->locals, name);
	if (found)
	{
		rb_insert(&frame->locals, ft_strdup(name), value);
		return ;
	}
	rb_insert(&shell->reg_env, ft_strdup(name), value);
}

t_eval_result
	eval_for(t_shell *shell, t_ast_node *cmd)
{
	size_t			i;
	t_eval_result	result;
	char			**argv;
	
	argv = arg_expansion(shell, cmd->st_for.args, cmd->st_for.nargs);
	if (!argv)
		return ((t_eval_result){RES_NONE, 0});
	i = 0;
	while (argv[i])
	{
		set_variable(shell, cmd->st_for.ident, ft_strdup(argv[i]));
		result = eval(shell, cmd->st_for.body);
		if ((result.type == RES_BREAK && result.param > 0)
			|| result.type == RES_RETURN)
		{
			args_free(argv);
			if (result.type == RES_BREAK)
				--result.type;
			return (result);
		}
		++i;
	}
	args_free(argv);
	return ((t_eval_result){RES_NONE, 0});
}
