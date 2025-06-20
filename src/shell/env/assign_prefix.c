/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   assign_prefix.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgamba <linogamba@pundalik.org>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 11:59:40 by lgamba            #+#    #+#             */
/*   Updated: 2025/03/17 11:59:41 by lgamba           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include <shell/shell.h>

void
	prefix_stack_init(t_shell *shell)
{
	shell->prefix_stack.variables = NULL;
	shell->prefix_stack.capacity = 0;
	shell->prefix_stack.size = 0;
}

void
	prefix_stack_deinit(t_shell *shell)
{
	while (shell->prefix_stack.size)
		prefix_stack_pop(shell);
	free(shell->prefix_stack.variables);
}
