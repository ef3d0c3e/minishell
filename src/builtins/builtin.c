/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgamba <linogamba@pundalik.org>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 11:59:40 by lgamba            #+#    #+#             */
/*   Updated: 2025/03/17 11:59:41 by lgamba           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include <shell/shell.h>

static void
	noop(void *data)
{
	(void)data;
}

static void
	insert_builtin(t_shell *shell, const t_builtin *builtin)
{
	rb_insert(&shell->reg_builtins, (void *)builtin->name, (void *)builtin);
}

void
	builtin_init(t_shell *shell)
{
	shell->reg_builtins = rb_new((int (*)(const void *, const void *))ft_strcmp,
			noop, noop);
	insert_builtin(shell, builtin_echo());
	insert_builtin(shell, builtin_fds());
}
