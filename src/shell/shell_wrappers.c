/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell_wrappers.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgamba <linogamba@pundalik.org>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 11:59:40 by lgamba            #+#    #+#             */
/*   Updated: 2025/03/17 11:59:41 by lgamba           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include <shell/shell.h>

pid_t
	shell_fork(t_shell *shell, const char *function)
{
	pid_t	pid;
	char	*err;

	pid = fork();
	if (shell->is_child && pid == -1)
		shell_perror(shell, "fork() failed", function);
	else if (pid == -1)
	{
		ft_asprintf(&err, "fork() failed: %m");
		shell_error(shell, err, function);
	}
	if (!pid)
		shell->is_child = 1;
	return (pid);
}

void
	shell_exit(t_shell *shell, int status)
{
	if (!shell_error_flush(shell))
		status = -1;
	shell_free(shell);
	exit(status);
}
