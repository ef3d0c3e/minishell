#include "ft_printf.h"
#include "parser/parser.h"
#include "shell/opts.h"
#include "tokenizer/tokenizer.h"
#include "util/util.h"
#include <expansion/expansion.h>
#include <shell/eval.h>
#include <stdio.h>

int main(int ac, char **av, char **envp)
{
	t_environ		environ;

	environ = env_new(envp);
	repl(&environ, ft_strdup(av[1]));
	env_free(&environ);
	return (0);
}
