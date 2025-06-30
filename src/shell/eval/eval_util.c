/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   eval_util.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgamba <linogamba@pundalik.org>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 11:59:40 by lgamba            #+#    #+#             */
/*   Updated: 2025/03/17 11:59:41 by lgamba           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include <shell/shell.h>

const char
*eval_signal_name(int sig)
{
	static const char *names[] = {
		[SIGHUP] = "SIGHUP",
		[SIGINT] = "SIGINT",
		[SIGQUIT] = "SIGQUIT",
		[SIGILL] = "SIGILL",
		[SIGTRAP] = "SIGTRAP",
		[SIGABRT] = "SIGABRT",
		[SIGBUS] = "SIGBUS",
		[SIGFPE] = "SIGFPE",
		[SIGKILL] = "SIGKILL",
		[SIGUSR1] = "SIGUSR1",
		[SIGSEGV] = "SIGSEGV",
		[SIGUSR2] = "SIGUSR2",
		[SIGPIPE] = "SIGPIPE",
		[SIGALRM] = "SIGALRM",
		[SIGTERM] = "SIGTERM",
		[SIGSTKFLT] = "SIGSTKFLT",
		[SIGCHLD] = "SIGCHLD",
		[SIGCONT] = "SIGCONT",
		[SIGSTOP] = "SIGSTOP",
		[SIGTSTP] = "SIGTSTP",
		[SIGTTIN] = "SIGTTIN",
		[SIGTTOU] = "SIGTTOU",
		[SIGURG] = "SIGURG",
		[SIGXCPU] = "SIGXCPU",
		[SIGXFSZ] = "SIGXFSZ",
		[SIGVTALRM] = "SIGVTALRM",
		[SIGPROF] = "SIGPROF",
		[SIGWINCH] = "SIGWINCH",
		[SIGPOLL] = "SIGPOLL",
		[SIGPWR] = "SIGPWR",
		[SIGSYS] = "SIGSYS",
	};
	if (sig <= 0 || sig > SIGSYS)
		return ("Unknown signal");
	return (names[sig]);
}
