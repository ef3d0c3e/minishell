/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir_internal_heredoc.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgamba <linogamba@pundalik.org>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 11:59:40 by lgamba            #+#    #+#             */
/*   Updated: 2025/03/17 11:59:41 by lgamba           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include <shell/shell.h>

int
	redir_internal_heredoc(
	t_shell *shell,
	t_redirs_stack *stack,
	t_redirection *redir)
{
	int			fds[2];

	if (shell_pipe(shell, fds) == -1)
		shell_perror(shell, "pipe() failed", SRC_LOCATION);
	if (redir_write_all(fds[1], redir->heredoc.str, redir->heredoc.len) < 0)
	{
		shell_close(shell, fds[0]);
		shell_close(shell, fds[1]);
		shell_perror(shell, "write() failed", SRC_LOCATION);
	}
	shell_close(shell, fds[1]);
	if (redir_dup2(shell, stack, fds[0], STDIN_FILENO) == -1)
		shell_perror(shell, "dup2() failed", SRC_LOCATION);
	shell_close(shell, fds[0]);
	return (1);
}
