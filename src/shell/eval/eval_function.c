/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   eval_function.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgamba <linogamba@pundalik.org>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 11:59:40 by lgamba            #+#    #+#             */
/*   Updated: 2025/03/17 11:59:41 by lgamba           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "shell/env/env.h"
#include <shell/shell.h>

t_eval_result
	eval_function_definition(t_shell *shell, t_ast_node *def)
{
	char	*name;

	name = stringbuf_cstr(&def->function.name);
	def->function.registered = 1;
	rb_insert(&shell->reg_fns, ft_strdup(name), def);
	return ((t_eval_result){RES_NONE, 0});
}

t_eval_result
	eval_function(t_shell *shell, t_ast_node *cmd, char **argv)
{
	t_eval_result		result;
	t_ast_node *const	function = rb_find(&shell->reg_fns, argv[0]);
	t_redirs_stack		stack;
	
	redir_stack_init(&stack);
	do_redir(shell, &stack, &cmd->cmd.redirs);
	if (shell_error_flush(shell))
	{
		funs_stack_push(shell, function, argv);
		prefix_stack_push(shell, cmd->cmd.assigns, cmd->cmd.nassigns);
		result = eval(shell, function->function.body);
		if (result.type == RES_RETURN)
			shell->last_status = result.param;
		prefix_stack_pop(shell);
		funs_stack_pop(shell);
	}
	undo_redir(shell, &stack);
	return ((t_eval_result){RES_NONE, 0});
}
