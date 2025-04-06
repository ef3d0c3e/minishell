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

char *
	find_path(t_environ *env, t_string name)
{
	const char	*path = rb_find(&env->env, "PATH");
	
	if (!path)
		return (NULL);
}



void
	eval(t_environ *env, t_ast_node* program)
{
	if (program->type == NODE_COMMAND)
	{

	}
}
