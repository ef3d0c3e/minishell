/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   eval_sequence.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgamba <linogamba@pundalik.org>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 11:59:40 by lgamba            #+#    #+#             */
/*   Updated: 2025/03/17 11:59:41 by lgamba           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include <shell/shell.h>

void
	eval_sequence(t_shell *shell, t_ast_node *program)
{
	eval(shell, program->logic.left);
	if (!ft_strcmp(program->logic.token.reserved_word, "&&")
		&& !shell->last_status)
		eval(shell, program->logic.right);
	else if (!ft_strcmp(program->logic.token.reserved_word, "||")
		&& shell->last_status)
		eval(shell, program->logic.right);
}
