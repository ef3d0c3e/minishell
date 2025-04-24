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
#include "tokenizer/tokenizer.h"
#include <shell/eval.h>

void
	eval_sequence(t_shell *shell, t_ast_node *program)
{
	eval(env, program->logic.left);
	if (program->logic.token.type == TOK_NEWLINE
		|| !ft_strcmp(program->logic.token.reserved_word, ";"))
		eval(env, program->logic.right);
	if (!ft_strcmp(program->logic.token.reserved_word, "&&")
		&& !env->last_status)
		eval(env, program->logic.right);
	else if (!ft_strcmp(program->logic.token.reserved_word, "||")
		&& env->last_status)
		eval(env, program->logic.right);
}
