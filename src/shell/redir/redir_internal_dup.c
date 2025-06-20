/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir_internal_dup.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgamba <linogamba@pundalik.org>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 11:59:40 by lgamba            #+#    #+#             */
/*   Updated: 2025/03/17 11:59:41 by lgamba           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include <shell/shell.h>

static void
	format_err_dest(char **err, int status, int fd)
{
	if (status < 0)
		ft_asprintf(err, "Dest fd %d is not valid", fd);
	else if (status == 0)
		ft_asprintf(err, "Dest fd %d is not writeable", fd);
}

static void
	format_err_source(char **err, int status, int fd)
{
	if (status < 0)
		ft_asprintf(err, "Source fd %d is not valid", fd);
	else if (status == 0)
		ft_asprintf(err, "Source fd %d is not readable", fd);
}

int
	redir_internal_dup_word(
	t_shell *shell,
	t_redirs_stack *stack,
	t_redirection *redir)
{
	char	*err;
	int		status;
	int		redirectee;

	redirectee = redir_open(shell, redir);
	if (redirectee == redir->redirector.fd)
		return (1);
	err = NULL;
	status = fd_check(shell, redirectee, 0, 1);
	format_err_dest(&err, status, redirectee);
	if (err)
		return (shell_error(shell, err, SRC_LOCATION), 0);
	status = fd_check(shell, redir->redirector.fd, 1, 0);
	format_err_source(&err, status, redir->redirector.fd);
	if (err)
		return (shell_error(shell, err, SRC_LOCATION), 0);
	if (redir_dup2(shell, stack, redirectee, redir->redirector.fd) < 0)
		return (ft_asprintf(&err, "Failed to dup2: %m"),
			shell_error(shell, err, SRC_LOCATION), 0);
	return (1);
}

int
	redir_internal_dup_move(
	t_shell *shell,
	t_redirs_stack *stack,
	t_redirection *redir)
{
	char	*err;
	int		status;

	if (redir->redirectee.fd == redir->redirector.fd)
		return (1);
	err = NULL;
	status = fd_check(shell, redir->redirectee.fd, 0, 1);
	format_err_dest(&err, status, redir->redirectee.fd);
	if (err)
		return (shell_error(shell, err, SRC_LOCATION), 0);
	status = fd_check(shell, redir->redirector.fd, 1, 0);
	format_err_source(&err, status, redir->redirector.fd);
	if (err)
		return (shell_error(shell, err, SRC_LOCATION), 0);
	if (redir_dup2(shell, stack, redir->redirectee.fd, redir->redirector.fd)
		< 0)
		return (ft_asprintf(&err, "Failed to dup2: %m"),
			shell_error(shell, err, SRC_LOCATION), 0);
	if (redir->type == R_MOVE_INPUT || redir->type == R_MOVE_OUTPUT)
		shell_close(shell, redir->redirectee.fd);
	return (1);
}
