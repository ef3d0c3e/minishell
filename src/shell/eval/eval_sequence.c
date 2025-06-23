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

t_eval_result
	eval_binary(t_shell *shell, t_ast_node *program)
{
	t_eval_result	result;

	result = eval(shell, program->logic.left);
	if (result.type != RES_NONE)
		return (result);
	if (!ft_strcmp(program->logic.token.reserved_word, "&&")
		&& shell->last_status == 0)
		result = eval(shell, program->logic.right);
	else if (!ft_strcmp(program->logic.token.reserved_word, "||")
		&& shell->last_status != 0)
		result = eval(shell, program->logic.right);
	return (result);
}
