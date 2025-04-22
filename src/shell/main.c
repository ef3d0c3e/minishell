#include <shell/eval.h>

int main(int ac, char **av, const char **envp)
{
	t_environ		environ;

	environ = env_new(envp);
	repl(&environ, ft_strdup(av[1]));
	env_free(&environ);
	return (0);
}
