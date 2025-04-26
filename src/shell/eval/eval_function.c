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
#include <shell/shell.h>

void
	eval_function_definition(t_shell *shell, t_ast_node *def)
{
	char	*name;

	name = stringbuf_cstr(&def->function.name);
	def->function.registered = 1;
	rb_insert(&shell->reg_fns, ft_strdup(name), def);

}

void
	eval_function(t_shell *shell, t_ast_node *cmd, char **argv)
{
	t_ast_node *const	function = rb_find(&shell->reg_fns, argv[0]);
	t_redirs_stack		stack;
	
	redir_stack_init(&stack);
	do_redir(shell, &stack, &cmd->cmd.redirs);
	if (shell_error_flush(shell))
	{
		funs_stack_push(shell, function, argv);
		eval(shell, function->function.body);
		funs_stack_pop(shell);
	}
	undo_redir(shell, &stack);
}
