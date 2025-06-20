/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir_internal_files.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgamba <linogamba@pundalik.org>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 11:59:40 by lgamba            #+#    #+#             */
/*   Updated: 2025/03/17 11:59:41 by lgamba           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include <shell/shell.h>

int
	redir_internal_file(
	t_shell *shell,
	t_redirs_stack *stack,
	t_redirection *redir)
{
	int		fd;
	char	*err;

	fd = redir_open(shell, redir);
	if (fd < 0)
		return (0);
	if (fd != redir->redirector.fd && redir_dup2(shell, stack, fd,
			redir->redirector.fd) < 0)
	{
		shell_close(shell, fd);
		ft_asprintf(&err, "Failed to dup2: %m");
		return (shell_error(shell, err, SRC_LOCATION), 0);
	}
	if (fd != redir->redirector.fd)
		shell_close(shell, fd);
	if (redir->type == R_ERR_AND_OUT || redir->type == R_APPEND_ERR_AND_OUT)
	{
		if (redir_dup2(shell, stack, STDOUT_FILENO, STDERR_FILENO) < 0)
		{
			ft_asprintf(&err, "Failed to dup2: %m");
			shell_error(shell, err, SRC_LOCATION);
			return (0);
		}
	}
	return (1);
}
