/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   variable.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgamba <linogamba@pundalik.org>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 11:59:40 by lgamba            #+#    #+#             */
/*   Updated: 2025/03/17 11:59:41 by lgamba           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include <shell/shell.h>

void
	set_variable(t_shell *shell, const char *name, char *value, int local)
{
	t_stack_frame	*frame;
	size_t			i;

	i = 0;
	while (local && i < shell->eval_stack.size)
	{
		frame = &shell->eval_stack.frames[shell->eval_stack.size - i - 1];
		if (rb_find(&frame->locals, name))
		{
			rb_insert(&frame->locals, ft_strdup(name), value);
			return ;
		}
		++i;
	}
	rb_insert(&shell->reg_env, ft_strdup(name), value);
}

char
	*get_variable(t_shell *shell, const char *name)
{
	t_stack_frame	*frame;
	char			*found;
	size_t			i;

	i = 0;
	while (i < shell->eval_stack.size)
	{
		frame = &shell->eval_stack.frames[shell->eval_stack.size - i - 1];
		found = rb_find(&frame->locals, name);
		if (found)
			return (found);
		++i;
	}
	return (rb_find(&shell->reg_env, name));
}
