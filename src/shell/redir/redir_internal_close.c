/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir_internal_close.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgamba <linogamba@pundalik.org>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 11:59:40 by lgamba            #+#    #+#             */
/*   Updated: 2025/03/17 11:59:41 by lgamba           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include <shell/shell.h>

int
	redir_internal_close(
	t_shell *shell,
	t_redirs_stack *stack,
	t_redirection *redir)
{
	(void)stack;
	(void)redir;
	shell_error(shell, ft_strdup("Unhandled redirection type"), SRC_LOCATION);
	return (0);
}
