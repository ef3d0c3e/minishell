/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgamba <linogamba@pundalik.org>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 11:59:40 by lgamba            #+#    #+#             */
/*   Updated: 2025/03/17 11:59:41 by lgamba           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "ft_printf.h"
#include "shell/env/env.h"
#include "util/util.h"
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
	export(t_shell *shell, int argc, char **argv)
{
	char				*sep;
	t_shell_var			var;

	if (argc == 1)
		return (rb_apply(&shell->reg_env, print_env, NULL), 0);
	else if (argc != 2)
		return (ft_dprintf(2, "USAGE: export VAR=VALUE\n"), 1);
	sep = ft_strchr(argv[1], '=');
	if (!sep)
		return (ft_dprintf(2, "USAGE: export VAR=VALUE\n"), 1);
	*sep = 0;
	if (!is_valid_identifier(argv[1]))
	{
		ft_dprintf(2, "export: `%s` not a legal identifier\n", argv[1]);
		return (1);
	}
	get_variable(shell, argv[1], &var);
	set_variable(shell, argv[1], ft_strdup(sep + 1), var.exported);
	return (0);
}

const t_builtin
	*builtin_export(void)
{
	static const t_builtin	builtin = (t_builtin){
		.name = "export",
		.desc = "The export builtin",
		.run = export,
		.init = NULL,
		.deinit = NULL,
	};

	return (&builtin);
}
