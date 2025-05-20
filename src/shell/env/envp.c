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
#include <shell/shell.h>

void
	envp_populate(t_shell *shell, const char *s)
{
	const char		*sep = ft_strchr(s, '=');
	t_string_buffer	key;
	t_string_buffer	value;
	t_shell_var		*var;
	char			*err;

	if (!sep)
	{
		ft_asprintf(&err, "Invalid environment variable: '%s'", s);
		shell_error(shell, err, SRC_LOCATION);
		shell_exit(shell, EXIT_FAILURE);
	}
	key = stringbuf_from_range(s, sep);
	value = stringbuf_from(sep + 1);
	var = xmalloc(sizeof(t_shell_var));
	var->value = stringbuf_cstr(&value);
	var->name = stringbuf_cstr(&key);
	var->exported = 1;
	rb_insert(&shell->reg_env, stringbuf_cstr(&key), var);
}

/** @brief Formats environ variables to the `VARIABLE=VALUE` format */
static inline void
	format_environ(size_t depth, t_rbnode *node, void *data)
{
	struct s_envp_traversal	*t = data;
	const t_shell_var		*var = node->data;

	(void)depth;
	if (!var->exported)
		return ;
	ft_asprintf(&t->envp[t->index], "%s=%s",
		(char *)node->key, var->value);
	++t->index;
}

char
	**environ_to_envp(t_shell *shell)
{
	struct s_envp_traversal	t;

	t.index = 0;
	t.envp = xmalloc((shell->reg_env.size + 1) * sizeof(char **));
	rb_apply(&shell->reg_env, format_environ, &t);
	t.envp[t.index] = NULL;
	return (t.envp);
}
