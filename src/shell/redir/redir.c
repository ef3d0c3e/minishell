/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgamba <linogamba@pundalik.org>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 11:59:40 by lgamba            #+#    #+#             */
/*   Updated: 2025/03/17 11:59:41 by lgamba           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include <shell/redir/eval_redir.h>

// TODO: Add a way to keep track of opened fd, because we can't use `fcntl(F_GETFD)`
// Probably inside an rb tree
static void
	redir_internal(t_environ *env, t_redirs_stack *stack, t_redirection *redir)
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

void
	do_redir(t_environ *env, t_redirs_stack *stack, t_redirections *redirs)
{
	size_t	i;

	i = 0;
	while (i < redirs->redirs_size)
	{
		redir_internal(env, stack, &redirs->redirs[i]);
		++i;
	}
}

void
	undo_redir(t_environ *env, t_redirs_stack *stack)
{
	t_redir_fd	*saved;
	size_t		i;

	i = 0;
	while (i++ < stack->size)
	{
		saved = &stack->fds[stack->size - i];
		if (saved->fd != saved->original_fd)
			dup2(saved->fd, saved->original_fd);
		close(saved->fd);
	}
	free(stack->fds);
}
