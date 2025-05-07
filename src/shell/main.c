#include "ft_printf.h"
#include "shell/ctx/ctx.h"
#include "shell/env/env.h"
#include "shell/eval/eval.h"
#include "util/util.h"
#include <shell/shell.h>
#include <readline/readline.h>
#include <readline/history.h>

#include <termios.h>
#include <unistd.h>

int main(int ac, char **av, const char **envp)
{
	t_shell			shell;
	t_string_buffer	prompt;
	t_eval_result	result;

	rl_catch_signals = 0;
	shell = shell_new(envp);
	using_history();
	signal_install(&shell, 0);
	profile_source(&shell);
	while (1) {

		prompt = ctx_eval_string(&shell, ft_strdup("prompt_left"), ft_strdup("Prompt"));
		char* input = readline(stringbuf_cstr(&prompt));
		stringbuf_free(&prompt);
		if (!input)
			break ;
		add_history(input);
		result = ctx_eval_stdout(&shell, input);
		if (result.type == RES_EXIT)
		{
			shell.last_status = result.param;
			break ;
		}
	}
	signal_install(&shell, 1);
	shell_free(&shell);
	return (shell.last_status);
}
