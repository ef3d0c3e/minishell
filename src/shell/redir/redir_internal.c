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
#include <shell/redir/eval_redir.h>

/** @brief Handles redirections to files */
static void
	redir_internal_files(
	t_shell *shell,
	t_redirs_stack *stack,
	t_redirection *redir)
{
	int		fd;
	char	*err;

	fd = redir_open(env, redir);
	if (fd < 0)
		return ;
	if (fd != redir->redirector.fd && redir_dup(env, stack, fd, redir->redirector.fd) < 0)
	{
		close(fd);
		ft_asprintf(&err, "Failed to dup2: %m");
		shell_error(env, err, SRC_LOCATION);
		return ;
	}
	if (fd != redir->redirector.fd)
		close(fd);
	if (redir->type == R_ERR_AND_OUT || redir->type == R_APPEND_ERR_AND_OUT)
	{
		if (redir_dup(env, stack, STDOUT_FILENO, STDERR_FILENO) < 0)
		{
			ft_asprintf(&err, "Failed to dup2: %m");
			shell_error(env, err, SRC_LOCATION);
			return ;
		}
	}
}

/** @brief Handles move and duplicate redirections */
static void
	redir_internal_dup_move(
	t_shell *shell,
	t_redirs_stack *stack,
	t_redirection *redir)
{
	char	*err;

	if (redir->redirectee.fd == redir->redirector.fd)
		return ;
	if (redir_dup(env, stack, redir->redirectee.fd, redir->redirector.fd) < 0)
	{
		ft_asprintf(&err, "Failed to dup2: %m");
		shell_error(env, err, SRC_LOCATION);
		return ;
	}
	if (redir->type == R_MOVE_INPUT || redir->type == R_MOVE_OUTPUT)
		close(redir->redirectee.fd);
}

static void
	redir_internal_close(
	t_shell *shell,
	t_redirs_stack *stack,
	t_redirection *redir)
{
	(void)stack;
	(void)redir;
	shell_fail(env, "Unhandled redirection type", SRC_LOCATION);
}

// TODO: Add a way to keep track of opened fd, because we can't use `fcntl(F_GETFD)`
// Probably inside an rb tree
void
	redir_internal(t_shell *shell, t_redirs_stack *stack, t_redirection *redir)
{
	if (redir->type == R_OUTPUT_DIRECTION || redir->type == R_APPENDING_TO
		|| redir->type == R_INPUT_DIRECTION || redir->type == R_ERR_AND_OUT
		|| redir->type == R_APPEND_ERR_AND_OUT || redir->type == R_INPUT_OUTPUT
		|| redir->type == R_OUTPUT_FORCE)
		redir_internal_files(env, stack, redir);
	else if (redir->type == R_DUPLICATING_INPUT || redir->type == R_MOVE_INPUT
		|| redir->type == R_DUPLICATING_OUTPUT || redir->type == R_MOVE_OUTPUT)
		redir_internal_dup_move(env, stack, redir);
	else if (redir->type == R_CLOSE_THIS)
		redir_internal_close(env, stack, redir);
	else
		shell_fail(env, "Unhandled redirection type", SRC_LOCATION);
	// TODO: HERESTRING/DOC
}
