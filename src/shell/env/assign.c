/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   assign.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgamba <linogamba@pundalik.org>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 11:59:40 by lgamba            #+#    #+#             */
/*   Updated: 2025/03/17 11:59:41 by lgamba           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "util/util.h"
#include <shell/shell.h>

void
	prefix_stack_init(t_shell *shell)
{
	shell->prefix_stack.variables = NULL;
	shell->prefix_stack.capacity = 0;
	shell->prefix_stack.size = 0;
}
void
	prefix_stack_deinit(t_shell *shell)
{
	while (shell->prefix_stack.size)
		prefix_stack_pop(shell);
	free(shell->prefix_stack.variables);
}

void
	prefix_stack_push(
	t_shell *shell,
	struct s_assignment *assigns,
	size_t size)
{
	t_prefix_stack	*stack;
	size_t	i;
	char	*result;

	stack = &shell->prefix_stack;
	if (shell->prefix_stack.size >= shell->prefix_stack.capacity)
	{
		stack->variables = ft_realloc(stack->variables,
				sizeof(t_rbtree) * stack->size,
				sizeof(t_rbtree) * (stack->capacity * 2 + !stack->capacity));
		stack->capacity = stack->capacity * 2 + !stack->capacity;
	}
	stack->variables[stack->size] = rb_new(
		(int(*)(const void *, const void *))ft_strcmp, free, free);
	i = 0;
	while (i < size)
	{
		result = arg_expansion_single(shell, &assigns[i].value);
		if (result)
			rb_insert(&stack->variables[stack->size],
					ft_strdup(stringbuf_cstr(&assigns[i].variable)), result);
		++i;
	}
	++stack->size;
}

static void
	reinstall(size_t depth, t_rbnode *node, void *cookie)
{
	t_shell *const	shell = cookie;

}

void
	prefix_stack_pop(t_shell *shell)
{
	printf("POP\n");
	if (!shell->prefix_stack.size)
		shell_fail(shell, "Attempted to pop empty prefix stack", SRC_LOCATION);
	rb_apply(&shell->prefix_stack.variables[--shell->prefix_stack.size],
			reinstall, shell);
	rb_free(&shell->prefix_stack.variables[shell->prefix_stack.size]);
}
