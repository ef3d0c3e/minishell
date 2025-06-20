/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgamba <linogamba@pundalik.org>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 11:59:40 by lgamba            #+#    #+#             */
/*   Updated: 2025/06/19 17:39:22 by thschnei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include <shell/shell.h>

volatile sig_atomic_t	g_signal;

static void
	sighandler(int signum, siginfo_t *info, void *data)
{
	(void)info;
	(void)data;
	g_signal = signum;
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
