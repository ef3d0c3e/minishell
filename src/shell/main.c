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

static void
	repl(t_shell *shell)
{
	t_string_buffer	prompt;
	t_eval_result	result;
	t_getline		line;
	char			*input;

	shell->cap = fetch_capability(NULL);
	line = repl_setup(shell);
	signal_install(shell, 0);
	profile_source(shell);
	rb_insert(&shell->atexit, &line, (void *)getline_cleanup);
	while (1)
	{
		if (g_signal == SIGINT)
			shell->last_status = 130;
		g_signal = 0;
		prompt = stringbuf_from(" \033[35m󰘧\033[0m ");
		//prompt = ctx_eval_string(shell, ft_strdup("prompt_left"), ft_strdup("Prompt")).content;
		input = getline_read(&line, stringbuf_cstr(&prompt));
		stringbuf_free(&prompt);
		if (!input && g_signal != SIGINT)
			break ;
		if (!input)
			continue ;
		if (should_add_to_history(input))
			getline_history_add(&line, ft_strdup(input), 0);
		result = ctx_eval_stdout(shell, input);
		if (result.type == RES_EXIT)
		{
			shell->last_status = result.param;
			break ;
		}
	}
	rb_delete(&shell->atexit, &line);
	getline_cleanup(&line);
	signal_install(shell, 1);
}

static void
	run_cmd(t_shell *shell, const char *cmd)
{
	ctx_eval_stdout(shell, ft_strdup(cmd));
}

typedef struct s_program_opts
{
	const char	*headless;
	unsigned	dbg:1;
	void		*opts;
}	t_program_opts;

static void
	headless(void *ptr, const char **av)
{
	t_program_opts	*opts;

	opts = ptr;
	opts->headless = av[1];
}

static void
	print_version(void *ptr, const char **av)
{
	(void)ptr;
	(void)av;

	ft_dprintf(2, "Minishell -- A simple C shell\n"
		"Copyright (c) 2025 Lino Gamba and Thomas Schneider\n"
		"Minishell is licensed under the GNU General Public License version 3"
		"(GPLv3),\nunder the terms of the Free Software Foundation "
		"<https://www.gnu.org/licenses/agpl-3.0.en.html>.\n\n"
		"Minishell Version: 0.1\n");
	exit(0);
}

static void
	*init(void *a)
{
	static char			use[] = "[OPTIONS...]";
	static t_opt		opts[] = {
		{"-c", "Evaluate argument in headless mode", headless, 1},
		{"-v", "Displays version", print_version, 0},
	};
	const t_behavior	flags = GET_CALLER;

	return (_set_behavior(_init_opt(use, opts, *(&opts + 1) - opts, a), flags));
}

int main(int ac, const char **av, const char **envp)
{
	t_shell			shell;
	t_program_opts	opts;
	int				ret;

	opts.headless = NULL;
	opts.opts = init(&opts);
	ret = _parse_args(opts.opts, ac, av);
	shell = shell_new(envp);
	if (opts.headless)
		run_cmd(&shell, opts.headless);
	else
		repl(&shell);
	shell_free(&shell);
	return (shell.last_status);
}
