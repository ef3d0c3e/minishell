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
#include <shell/shell.h>

/** @brief Installs a prefix variable */
static void
	install_var(t_shell *shell, t_rbtree *saved, const char *name, char *value)
{
	t_shell_var		prev;
	t_prefix_var	*var;

	var = xmalloc(sizeof(t_prefix_var));
	var->saved_value = NULL;
	var->exported = 0;
	if (get_variable(shell, name, &prev))
	{
		var->exported = prev.exported;
		var->saved_value = ft_strdup(prev.value);
		set_variable(shell, name, value, prev.exported);
	}
	else
	{
		set_variable(shell, name, value, 0);
	}
	rb_insert(saved, ft_strdup(name), var);
}

/** @brief Uninstalls a prefix variable */
static void
	uninstall_var(size_t depth, t_rbnode *node, void *cookie)
{
	t_shell *const	shell = cookie;
	t_prefix_var	*var;

	(void)depth;
	var = node->data;
	unset_variable(shell, node->key);
	if (!var->saved_value)
		return ;
	set_variable(shell, node->key, var->saved_value, var->exported);
}

void
	prefix_stack_push(
	t_shell *shell,
	struct s_assignment *assigns,
	size_t size)
{
	t_prefix_stack	*stack;
	size_t			i;
	char			*result;

	stack = &shell->prefix_stack;
	if (shell->prefix_stack.size >= shell->prefix_stack.capacity)
	{
		stack->variables = ft_realloc(stack->variables,
				sizeof(t_rbtree) * stack->size,
				sizeof(t_rbtree) * (stack->capacity * 2 + !stack->capacity));
		stack->capacity = stack->capacity * 2 + !stack->capacity;
	}
	stack->variables[stack->size] = rb_new(
			(int (*)(const void *, const void *))ft_strcmp, free,
			free);
	i = 0;
	while (i < size)
	{
		result = word_expansion_cat(shell, &assigns[i].value);
		if (result)
			install_var(shell, &stack->variables[stack->size],
				stringbuf_cstr(&assigns[i].variable), result);
		++i;
	}
	++stack->size;
}

void
	prefix_stack_pop(t_shell *shell)
{
	if (!shell->prefix_stack.size)
		shell_fail(shell, "Attempted to pop empty prefix stack", SRC_LOCATION);
	rb_apply(&shell->prefix_stack.variables[--shell->prefix_stack.size],
		uninstall_var, shell);
	rb_free(&shell->prefix_stack.variables[shell->prefix_stack.size]);
}
