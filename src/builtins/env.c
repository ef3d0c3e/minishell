/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thschnei <thschnei@student.42perpignan.fr  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/13 12:44:58 by thschnei          #+#    #+#             */
/*   Updated: 2025/06/13 13:57:50 by thschnei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <shell/shell.h>

static void
	print_env(size_t depth, t_rbnode *node, void *cookie)
{
	t_shell_var *const	var = node->data;

	(void)depth;
	(void)cookie;
	if (!var->exported)
		return ;
	ft_printf("%s=%s\n", var->name, var->value);
}

static int
	env(t_shell *shell, int argc, char **argv)
{
	(void)argc;
	(void)argv;
	return (rb_apply(&shell->reg_env, print_env, NULL), 0);
}

const t_builtin
	*builtin_env(void)
{
	static const t_builtin	builtin = (t_builtin){
		.name = "env",
		.desc = "The env builtin",
		.run = env,
		.init = NULL,
		.deinit = NULL,
	};

	return (&builtin);
}
