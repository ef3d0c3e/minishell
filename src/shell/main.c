#include "ft_printf.h"
#include "shell/ctx/ctx.h"
#include "shell/env/env.h"
#include "shell/eval/eval.h"
#include "util/util.h"
#include <shell/shell.h>
#include <readline/readline.h>
#include <readline/history.h>

#include <signal.h>
#include <stdio.h>
#include <termios.h>
#include <unistd.h>

#include <libopts.h>
static int my_getc(FILE *stream) {
  char c;
  ssize_t n = read(STDIN_FILENO, &c, 1);
  if (n == 1)            return (unsigned char)c;
  if (n == 0)            return EOF;
  if (errno == EINTR)    return -1;    /* tell readline to abort */
  return EOF;
}

static void
	repl(t_shell *shell)
{
	t_string_buffer	prompt;
	t_eval_result	result;

	readline_setup();
	signal_install(shell, 0);
	profile_source(shell);
	while (1)
	{
		if (g_signal == SIGINT)
			shell->last_status = 130;
		g_signal = 0;
	//	prompt = stringbuf_from("> ");
		prompt = ctx_eval_string(shell, ft_strdup("prompt_left"), ft_strdup("Prompt")).stdout;
		char* input = readline(stringbuf_cstr(&prompt));
		stringbuf_free(&prompt);
		if (!input)
		{
			if (g_signal == SIGINT || g_signal == SIGQUIT)
			{
				continue ;
			}
			break ;
		}
		add_history(input);
		result = ctx_eval_stdout(shell, input);
		if (result.type == RES_EXIT)
		{
			shell->last_status = result.param;
			break ;
		}
	}
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
	*init(void *a)
{
	static char			use[] = "[OPTIONS...]";
	static t_opt		opts[] = {
		{"-c", "Evaluate argument in headless mode", headless, 1},
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
