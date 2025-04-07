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

void
	eval(t_environ *env, t_ast_node* program)
{
	if (program->type == NODE_COMMAND)
	{
		t_string_buffer atom = program->cmd.args[0].atom;
		//char *p = find_path(env, (t_string){atom.str, atom.len});
		//ft_printf("Found: `%s`\n", p);
		//free(p);
	}
}
