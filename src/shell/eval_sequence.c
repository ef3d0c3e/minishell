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
#include <shell/eval.h>

void
	eval_sequence(t_environ *env, t_ast_node *program)
{
	eval_cmd(env, program->logic.left);
	if (!ft_strcmp(program->logic.token.reserved_word, "&&")
		&& !env->last_status)
		eval(env, program->logic.right);
	else if (!ft_strcmp(program->logic.token.reserved_word, "||")
		&& env->last_status)
		eval(env, program->logic.right);
}
