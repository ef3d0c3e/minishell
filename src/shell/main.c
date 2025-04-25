#include "ft_printf.h"
#include "util/util.h"
#include <shell/shell.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>

int
int_cmp(const void *a, const void *b)
{
	return (int)((ptrdiff_t)a - (ptrdiff_t)b);
}

void
printfn(size_t de, t_rbnode *node, void *)
{
	for (size_t i = 0; i < de; ++i)
		ft_dprintf(2, " | ");
	ft_dprintf(2, "%d(%c)\n", (int)(intptr_t)node->key, "RB"[node->color]);
}

int main(int ac, char **av, const char **envp)
{
	t_shell		shell;

	shell = shell_new(envp);
	repl(&shell, ft_strdup(av[1]));
	shell_free(&shell);

	return (0);
}
