/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   undo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgamba <linogamba@pundalik.org>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 11:59:40 by lgamba            #+#    #+#             */
/*   Updated: 2025/03/17 11:59:41 by lgamba           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include <shell/shell.h>

void
	redir_stack_init(t_redirs_stack *stack)
{
	stack->size = 0;
	stack->capacity = 0;
	stack->fds = NULL;
}

static void
	stack_push(t_redirs_stack *stack, int fd, int original_fd)
{
	size_t	new_cap;

	new_cap = stack->capacity + !stack->capacity;
	while (new_cap <= stack->size + 1)
		new_cap *= 2;
	if (new_cap != stack->capacity)
		stack->fds = ft_realloc(stack->fds, stack->size * sizeof(t_redir_fd),
				new_cap * sizeof(t_redir_fd));
	stack->capacity = new_cap;
	stack->fds[stack->size++] = (t_redir_fd){
		.fd = fd,
		.original_fd = original_fd,
	};
}

int
	redir_dup2(t_shell *shell, t_redirs_stack *stack, int fd1, int fd2)
{
	int	saved_fd;

	saved_fd = shell_dup(shell, fd2);
	if (saved_fd < 0)
		shell_perror(shell, "dup failed", SRC_LOCATION);
	stack_push(stack, saved_fd, fd2);
	return (shell_dup2(shell, fd1, fd2));
}
