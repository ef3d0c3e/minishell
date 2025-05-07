/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgamba <linogamba@pundalik.org>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 11:59:40 by lgamba            #+#    #+#             */
/*   Updated: 2025/03/17 11:59:41 by lgamba           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "ft_printf.h"
#include <readline/readline.h>
#include <shell/shell.h>
#include <signal.h>

int	g_signal;

static void
	sighandler(int signum, siginfo_t *info, void *data)
{
	int	status;

	if (waitpid(-1, &status, WNOHANG) == 0)
		return ;
	g_signal = signum;
	if (signum == SIGINT)
		ft_dprintf(2, "^C\n");
	else  if (signum == SIGQUIT)
		ft_dprintf(2, "^D\n");
	else
		ft_dprintf(2, "SIG=%d\n", signum);
	g_signal = signum;
    rl_on_new_line();
    rl_replace_line("", 0);
    rl_redisplay();
}

void
	signal_install(t_shell *shell, int uninstall)
{
	struct sigaction	sa;

	sigemptyset(&sa.sa_mask);
	if (uninstall)
	{
		sa.sa_flags = 0;
		sa.sa_handler = SIG_DFL;
		if (sigaction(SIGINT, &sa, NULL) == -1
			|| sigaction(SIGQUIT, &sa, NULL) == -1)
				shell_perror(shell, "Failed to uninstall signal handlers",
						SRC_LOCATION);
		return ;
	}
	sa.sa_flags = SA_SIGINFO;
	sa.sa_sigaction = &sighandler;
	if (sigaction(SIGINT, &sa, NULL) == -1
		|| sigaction(SIGQUIT, &sa, NULL) == -1)
		shell_perror(shell, "Failed to install signal handlers", SRC_LOCATION);
}
