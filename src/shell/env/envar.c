/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   envar.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgamba <linogamba@pundalik.org>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 11:59:40 by lgamba            #+#    #+#             */
/*   Updated: 2025/03/17 11:59:41 by lgamba           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include <shell/shell.h>

void
	set_env_variable(t_shell *shell, const char *name, char *value)
{
	t_shell_var	*var;

	var = rb_find(&shell->reg_env, name);
	if (!var)
	{
		var = xmalloc(sizeof(t_shell_var));
		var->exported = 1;
		var->name = name;
		var->value = value;
		rb_insert(&shell->reg_env, ft_strdup(name), var);
		return ;
	}
	free(var->value);
	var->value = value;
}
