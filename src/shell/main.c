/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgamba <linogamba@pundalik.org>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 11:59:40 by lgamba            #+#    #+#             */
/*   Updated: 2025/03/17 11:59:41 by lgamba           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "shell/env/env.h"
#include <shell/shell.h>

static int
	should_add_to_history(const char *input)
{
	while (*input)
	{
		if (*input != ' ' && *input != '\t' && *input != '\n')
			return (1);
		++input;
	}
	return (0);
}

static char
	*repl_get_input(t_shell *shell, t_getline *line, int clean_mode)
{
	t_string_buffer	prompt;
	char			*input;

	if (clean_mode)
		prompt = stringbuf_from(" \033[35m󰘧\033[0m ");
	else
		prompt = ctx_eval_string(shell, ft_strdup("prompt_left"),
				ft_strdup("Prompt")).content;
	input = getline_read(line, stringbuf_cstr(&prompt));
	stringbuf_free(&prompt);
	return (input);
}

static void
	repl(t_shell *shell, t_getline *line, int clean_mode)
{
	t_eval_result	result;
	char			*input;

	if (!clean_mode)
		profile_source(shell);
	while (1)
	{
		if (g_signal == SIGINT)
			shell->last_status = 130;
		g_signal = 0;
		input = repl_get_input(shell, line, clean_mode);
		if (!input && g_signal != SIGINT)
			break ;
		if (!input)
			continue ;
		if (should_add_to_history(input))
			getline_history_add(line, ft_strdup(input), 0);
		result = ctx_eval_stdout(shell, input);
		if (result.type == RES_EXIT)
		{
			shell->last_status = result.param;
			break ;
		}
	}
}

int
	main(int ac, const char **av, const char **envp)
{
	t_program_opts	opts;
	t_shell			shell;
	t_getline		line;

	opts.headless = NULL;
	opts.clean_mode = 0;
	opts.opts = shell_args_init(&opts);
	if (_parse_args(opts.opts, ac, av) != ac)
		exit(ft_dprintf(2, "USAGE: %s [OPTIONS...]\n", av[0]) * 0);
	shell = shell_new(envp);
	set_env_variable(&shell, "SHELL", ft_strdup(av[0]));
	if (opts.headless)
		ctx_eval_stdout(&shell, ft_strdup(opts.headless));
	else
	{
		shell.cap = fetch_capability(get_variable_value(&shell, "TERM"));
		line = repl_setup(&shell);
		signal_install(&shell, 0);
		rb_insert(&shell.atexit, &line, (void *)getline_cleanup);
		repl(&shell, &line, opts.clean_mode);
		rb_delete(&shell.atexit, &line);
		getline_cleanup(&line);
		signal_install(&shell, 1);
	}
	return (shell_free(&shell), shell.last_status);
}
