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
#include "builtins/builtin.h"
#include "util/util.h"
#include <shell/shell.h>


t_shell
	shell_new(const char **envp)
{
	const char	**e;
	t_shell	shell;

	shell.reg_env = rb_new((int (*)(const void *, const void *))ft_strcmp,
			free, free);
	shell.path_cache = rb_new((int (*)(const void *, const void *))ft_strcmp,
			free, free);
	shell.errors.capacity = 0;
	shell.errors.size = 0;
	shell.errors.errors = NULL;
	shell.last_status = 0;
	shell.is_child = 0;
	shell.prompt = NULL;
	shell.token_list = NULL;
	shell.parser = NULL;
	shell.program = NULL;

	shell.is_interactive = 1;
	e = envp;
	while (*e)
		envp_populate(&shell, *(e++));
	temporaries_init(&shell);
	options_init(&shell);
	fd_data_init(&shell);
	path_populate(&shell);
	shell_error_flush(&shell);
	builtin_init(&shell);
	return (shell);
}

void
	shell_parser_free(t_shell *shell)
{
	free(shell->prompt);
	if (shell->token_list)
		token_list_free(shell->token_list);
	if (shell->parser)
		parser_free(shell->parser);
	ast_free(shell->program);
	shell->prompt = NULL;
	shell->token_list = NULL;
	shell->parser = NULL;
	shell->program = NULL;
}

void
	shell_free(t_shell *shell)
{
	builtin_deinit(shell);
	shell_error_flush(shell);
	rb_free(&shell->temporaries);
	rb_free(&shell->reg_env);
	rb_free(&shell->reg_fds);
	rb_free(&shell->path_cache);
	rb_free(&shell->reg_builtins);
	rb_free(&shell->options);
	shell_parser_free(shell);
	free(shell->errors.errors);
}
