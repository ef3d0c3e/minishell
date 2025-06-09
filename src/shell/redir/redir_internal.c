/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir_internal.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgamba <linogamba@pundalik.org>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 11:59:40 by lgamba            #+#    #+#             */
/*   Updated: 2025/03/17 11:59:41 by lgamba           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include <shell/shell.h>

/** @brief Handles redirections to files */
static int
	redir_internal_files(
	t_shell *shell,
	t_redirs_stack *stack,
	t_redirection *redir)
{
	int		fd;
	char	*err;

	fd = redir_open(shell, redir);
	if (fd < 0)
		return (0);
	if (fd != redir->redirector.fd && redir_dup2(shell, stack, fd, redir->redirector.fd) < 0)
	{
		shell_close(shell, fd);
		ft_asprintf(&err, "Failed to dup2: %m");
		shell_error(shell, err, SRC_LOCATION);
		return (0);
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

/** @brief Handles move and duplicate redirections */
static int
	redir_internal_dup_move(
	t_shell *shell,
	t_redirs_stack *stack,
	t_redirection *redir)
{
	char	*err;
	int		status;

	if (redir->redirectee.fd == redir->redirector.fd)
		return (0);
	err = NULL;
	status = fd_check(shell, redir->redirectee.fd, O_RDONLY);
	if (status < 0)
		ft_asprintf(&err, "Dest fd %d is not valid", redir->redirectee.fd);
	else if (status > 0)
		ft_asprintf(&err, "Dest fd %d is not writeable", redir->redirectee.fd);
	if (err)
	{
		shell_error(shell, err, SRC_LOCATION);
		return (0);
	}
	status = fd_check(shell, redir->redirector.fd, O_WRONLY);
	if (status < 0)
		ft_asprintf(&err, "Source fd %d is not valid", redir->redirector.fd);
	else if (status > 0)
		ft_asprintf(&err, "Source fd %d is not readeable", redir->redirectee.fd);
	if (err)
	{
		shell_error(shell, err, SRC_LOCATION);
		return (0);
	}
	if (redir_dup2(shell, stack, redir->redirectee.fd, redir->redirector.fd) < 0)
	{
		ft_asprintf(&err, "Failed to dup2: %m");
		shell_error(shell, err, SRC_LOCATION);
		return (0);
	}
	if (redir->type == R_MOVE_INPUT || redir->type == R_MOVE_OUTPUT)
		shell_close(shell, redir->redirectee.fd);
	return (1);
}

static int
	redir_internal_close(
	t_shell *shell,
	t_redirs_stack *stack,
	t_redirection *redir)
{
	(void)stack;
	(void)redir;
	shell_fail(shell, "Unhandled redirection type", SRC_LOCATION);
	return (0);
}

static ssize_t
	write_all(int fd, const void *buf, size_t len)
{
	ssize_t r;

	r = 0;
	while (len)
	{
		r = write(fd, ((const unsigned char *)buf) + r, len);
		if (r < 0)
			return (r);
		len -= r;
	}
	return (r);
}

static int
	redir_internal_heredoc(
	t_shell *shell,
	t_redirs_stack *stack,
	t_redirection *redir)
{
	int			fds[2];

	if (shell_pipe(shell, fds) == -1)
		shell_perror(shell, "pipe() failed", SRC_LOCATION);
	if (write_all(fds[1], redir->heredoc.str, redir->heredoc.len) < 0)
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

int
	redir_internal(t_shell *shell, t_redirs_stack *stack, t_redirection *redir)
{
	if (redir->type == R_OUTPUT_DIRECTION || redir->type == R_APPENDING_TO
		|| redir->type == R_INPUT_DIRECTION || redir->type == R_ERR_AND_OUT
		|| redir->type == R_APPEND_ERR_AND_OUT || redir->type == R_INPUT_OUTPUT
		|| redir->type == R_OUTPUT_FORCE)
		return (redir_internal_files(shell, stack, redir));
	else if (redir->type == R_DUPLICATING_INPUT || redir->type == R_MOVE_INPUT
		|| redir->type == R_DUPLICATING_OUTPUT || redir->type == R_MOVE_OUTPUT)
		return (redir_internal_dup_move(shell, stack, redir));
	else if (redir->type == R_CLOSE_THIS)
		return (redir_internal_close(shell, stack, redir));
	else if (redir->type == R_DEBLANK_READING_UNTIL
		|| redir->type == R_READING_UNTIL)
		return (redir_internal_heredoc(shell, stack, redir));
	else
		shell_fail(shell, "Unhandled redirection type", SRC_LOCATION);
	return (0);
}
