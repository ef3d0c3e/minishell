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
#include "builtins/builtin.h"
#include <shell/shell.h>


static void
	insert_builtin(t_shell *shell, const t_builtin *builtin)
{
	if (builtin->init)
		builtin->init(shell);
	rb_insert(&shell->reg_builtins, (void *)builtin->name, (void *)builtin);
}

void
	builtin_init(t_shell *shell)
{
	shell->reg_builtins = rb_new((int (*)(const void *, const void *))ft_strcmp,
			NULL, NULL);
	insert_builtin(shell, builtin_echo());
	insert_builtin(shell, builtin_fds());
	insert_builtin(shell, builtin_source());
	insert_builtin(shell, builtin_unset());
	insert_builtin(shell, builtin_export());
	insert_builtin(shell, builtin_cd());
	insert_builtin(shell, builtin_type());
}

/** @brief Calls the builtin's deinit function */
static void
	builtin_delete(size_t depth, t_rbnode *node, void *sh)
{
	t_shell *const	shell = sh;
	t_builtin		*builtin;

	(void)depth;
	if (!node->data)
		return ;
	builtin = node->data;
	if (builtin->deinit)
		builtin->deinit(shell);
}

void
	builtin_deinit(t_shell *shell)
{
	rb_apply(&shell->reg_builtins, builtin_delete, shell);
	rb_free(&shell->reg_builtins);
}
