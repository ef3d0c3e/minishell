/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   envp.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgamba <linogamba@pundalik.org>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 11:59:40 by lgamba            #+#    #+#             */
/*   Updated: 2025/03/17 11:59:41 by lgamba           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "parser/parser.h"
#include <shell/shell.h>

/** @brief Function to free function definitions */
static void
	fun_free(void *fun)
{
	t_ast_node *node;

	node = fun;
	node->function.registered = 0;
	ast_free(node);
}

void
	funs_init(t_shell *shell)
{
	shell->eval_stack.capacity = 1024;
	shell->eval_stack.size = 0;
	shell->eval_stack.frames = xmalloc(sizeof(t_stack_frame)
		* shell->eval_stack.capacity);
	shell->reg_fns = rb_new((int(*)(const void *, const void *))ft_strcmp,
		free, fun_free);
}

void
	funs_deinit(t_shell *shell)
{
	while (shell->eval_stack.size)
		funs_stack_pop(shell);
	free(shell->eval_stack.frames);
	rb_free(&shell->reg_fns);
}

void
funs_stack_push(t_shell *shell, t_ast_node *function, char **argv)
{
	t_stack_frame	frame;

	if (shell->eval_stack.size == shell->eval_stack.capacity)
		shell_fail(shell, "Stack overflow", SRC_LOCATION);
	frame.args = argv;
	frame.nargs = 0;
	while (frame.args[frame.nargs])
		++frame.nargs;
	frame.function = function;
	frame.locals = rb_new((int(*)(const void *, const void *))ft_strcmp,
		free, free);
	shell->eval_stack.frames[shell->eval_stack.size++] = frame;
}

void
funs_stack_pop(t_shell *shell)
{
	const size_t	i = shell->eval_stack.size - 1;
	size_t			j;

	if (shell->eval_stack.size == 0)
		shell_fail(shell, "Stack underflow", SRC_LOCATION);
	j = 0;
	while (j < shell->eval_stack.frames[i].nargs)
		free(shell->eval_stack.frames[i].args[j++]);
	free(shell->eval_stack.frames[i].args);
	rb_free(&shell->eval_stack.frames[i].locals);
	--shell->eval_stack.size;
}
