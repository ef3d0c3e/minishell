/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   variable.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgamba <linogamba@pundalik.org>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 11:59:40 by lgamba            #+#    #+#             */
/*   Updated: 2025/03/17 11:59:41 by lgamba           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "ft_printf.h"
#include <shell/shell.h>

void
	set_variable(t_shell *shell, const char *name, char *value, int export)
{
	t_shell_var		*var;
	char			*name_cpy;
	t_stack_frame	*frame;
	size_t			i;

	i = 0;
	while (!export && i < shell->eval_stack.size)
	{
		frame = &shell->eval_stack.frames[shell->eval_stack.size - i - 1];
		if (rb_find(&frame->locals, name))
		{
			rb_insert(&frame->locals, ft_strdup(name), value);
			return ;
		}
		++i;
	}
	var = rb_find(&shell->reg_env, name);
	if (var)
	{
		free(var->value);
		var->value = value;
		return ;
	}
	name_cpy = ft_strdup(name);
	var = xmalloc(sizeof(t_shell_var));
	var->name = name_cpy;
	var->value = value;
	var->exported = export;
	rb_insert(&shell->reg_env, name_cpy, var);
}

char
	*get_variable_value(t_shell *shell, const char *name)
{
	t_shell_var	var;

	if (!get_variable(shell, name, &var))
		return (NULL);
	return (var.value);
}

int
	get_variable(t_shell *shell, const char *name, t_shell_var *var)
{
	t_shell_var		*found;
	t_stack_frame	*frame;
	size_t			i;

	var->name = name;
	var->exported = 0;
	var->value = NULL;
	i = 0;
	while (i < shell->eval_stack.size)
	{
		frame = &shell->eval_stack.frames[shell->eval_stack.size - i - 1];
		var->value = rb_find(&frame->locals, name);
		if (var->value)
			return (1);
		++i;
	}
	found = rb_find(&shell->reg_env, name);
	if (found)
	{
		var->exported = found->exported;
		var->value = found->value;
	}
	return (var->value != NULL);
}

int
	unset_variable(t_shell *shell, const char *name)
{
	t_stack_frame	*frame;
	size_t			i;

	i = 0;
	while (i < shell->eval_stack.size)
	{
		frame = &shell->eval_stack.frames[shell->eval_stack.size - i - 1];
		if (rb_delete(&frame->locals, name))
			return (1);
		++i;
	}
	return (rb_delete(&shell->reg_env, name));
}
