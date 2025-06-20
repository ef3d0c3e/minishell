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

ssize_t
	redir_write_all(int fd, const void *buf, size_t len)
{
	ssize_t	r;

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

int
	redir_internal(t_shell *shell, t_redirs_stack *stack, t_redirection *redir)
{
	if (redir->type == R_OUTPUT_DIRECTION || redir->type == R_APPENDING_TO
		|| redir->type == R_INPUT_DIRECTION || redir->type == R_ERR_AND_OUT
		|| redir->type == R_APPEND_ERR_AND_OUT || redir->type == R_INPUT_OUTPUT
		|| redir->type == R_OUTPUT_FORCE)
		return (redir_internal_file(shell, stack, redir));
	else if (redir->type == R_DUPLICATING_INPUT || redir->type == R_MOVE_INPUT
		|| redir->type == R_DUPLICATING_OUTPUT || redir->type == R_MOVE_OUTPUT)
		return (redir_internal_dup_move(shell, stack, redir));
	else if (redir->type == R_DUPLICATING_INPUT_WORD
		|| redir->type == R_DUPLICATING_OUTPUT_WORD)
		return (redir_internal_dup_word(shell, stack, redir));
	else if (redir->type == R_DEBLANK_READING_UNTIL
		|| redir->type == R_READING_UNTIL)
		return (redir_internal_heredoc(shell, stack, redir));
	else if (redir->type == R_CLOSE_THIS)
		return (redir_internal_close(shell, stack, redir));
	else
		shell_error(shell, ft_strdup("Unhandled redirection type"),
			SRC_LOCATION);
	return (0);
}
