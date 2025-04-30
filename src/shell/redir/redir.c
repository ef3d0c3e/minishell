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
#include "shell/expand/expand.h"
#include <shell/shell.h>

int
	do_redir(t_shell *shell, t_redirs_stack *stack, t_redirections *redirs)
{
	size_t	i;

	i = 0;
	while (i < redirs->redirs_size)
	{
		if (!redir_internal(shell, stack, &redirs->redirs[i]))
			return (0);
		++i;
	}
	return (1);
}

void
	undo_redir(t_shell *shell, t_redirs_stack *stack)
{
	t_redir_fd	*saved;
	size_t		i;

	i = 0;
	while (i++ < stack->size)
	{
		saved = &stack->fds[stack->size - i];
		if (saved->fd != saved->original_fd)
			shell_dup2(shell, saved->fd, saved->original_fd);
		shell_close(shell, saved->fd);
	}
	free(stack->fds);
}
