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

	shell = shell_new(envp);
	//ctx_eval_stdout(&shell, ft_strdup(av[1]));
	//rl_bind_key('\t', rl_complete);

	//// Enable history
	using_history();

	profile_source(&shell);
	//ctx_eval_stdout(&shell, ft_strdup("source default.sh"));
	while (1) {

		prompt = ctx_eval_string(&shell, ft_strdup("prompt_left"), ft_strdup("Prompt"));
		//prompt = stringbuf_from("> ");
		// Display prompt and read input
		char* input = readline(stringbuf_cstr(&prompt));
		stringbuf_free(&prompt);

		// Check for EOF.
		if (!input)
			break ;

		// Add input to readline history.
		add_history(input);

		ctx_eval_stdout(&shell, input);
	}
	shell_free(&shell);

	return (shell.last_status);
}
