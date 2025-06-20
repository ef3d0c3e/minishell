/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgamba <linogamba@pundalik.org>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 11:59:40 by lgamba            #+#    #+#             */
/*   Updated: 2025/06/13 13:53:44 by thschnei         ###   ########.fr       */
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
	if (ft_strcmp(var->name, "_"))
		ft_printf("declare -x %s=%s\n", var->name, var->value);
}

static int
	export(t_shell *shell, int argc, char **argv)
{
	char				*sep;
	t_shell_var			*var;
	char				*name;

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
	var = xmalloc(sizeof(t_shell_var));
	var->exported = 1;
	name = ft_strdup(argv[1]);
	var->name = name;
	var->value = ft_strdup(sep + 1);
	rb_insert(&shell->reg_env, name, var);
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
