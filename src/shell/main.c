#include <shell/shell.h>

int main(int ac, char **av, const char **envp)
{
	t_shell		shell;

	shell = shell_new(envp);
	repl(&shell, ft_strdup(av[1]));
	shell_free(&shell);
	return (0);
}
