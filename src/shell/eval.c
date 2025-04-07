/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   eval.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgamba <linogamba@pundalik.org>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 11:59:40 by lgamba            #+#    #+#             */
/*   Updated: 2025/03/17 11:59:41 by lgamba           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "eval.h"
#include "util/util.h"

void
	eval(t_environ *env, t_ast_node* program)
{
	if (program->type == NODE_COMMAND)
	{
		char *name = stringbuf_cstr(&program->cmd.args[0].atom);
		char *p = rb_find(&env->path_program, name);
		//free(name);
		ft_printf("Found: `%s`\n", p);
		//free(p);
	}
}
