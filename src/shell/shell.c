/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgamba <linogamba@pundalik.org>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 11:59:40 by lgamba            #+#    #+#             */
/*   Updated: 2025/03/17 11:59:41 by lgamba           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "shell/ctx/ctx.h"
#include "shell/env/env.h"
#include "util/util.h"
#include <shell/shell.h>

static void
	var_free(void *ptr)
{
	t_shell_var *var = ptr;

	free(var->value);
	free(var);
}

t_shell
	shell_new(const char **envp)
{
	const char	**e;
	t_shell	shell;

	shell.reg_env = rb_new((int (*)(const void *, const void *))ft_strcmp,
			free, var_free);
	shell.path_cache = rb_new((int (*)(const void *, const void *))ft_strcmp,
			free, free);
	shell.errors.capacity = 0;
	shell.errors.size = 0;
	shell.errors.errors = NULL;
	shell.last_status = 0;
	shell.is_child = 0;
	shell.is_interactive = 1;
	e = envp;
	while (*e)
		envp_populate(&shell, *(e++));
	temporaries_init(&shell);
	options_init(&shell);
	fd_data_init(&shell);
	shell_error_flush(&shell);
	builtin_init(&shell);
	funs_init(&shell);
	prefix_stack_init(&shell);
	path_populate(&shell);
	shell.context = NULL;
	return (shell);
}

void
	shell_free(t_shell *shell)
{
	while (shell->context)
		ctx_free(shell->context);
	builtin_deinit(shell);
	shell_error_flush(shell);
	rb_free(&shell->reg_env);
	rb_free(&shell->reg_fds);
	rb_free(&shell->path_cache);
	rb_free(&shell->reg_builtins);
	rb_free(&shell->options);
	funs_deinit(shell);
	prefix_stack_deinit(shell);
	temporaries_cleanup(shell);
	free(shell->errors.errors);
}
