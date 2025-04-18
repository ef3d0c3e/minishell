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
#include <shell/eval.h>

static void
	noop(void *data)
{
	(void)data;
}

static void
	insert_builtin(t_environ *env, const t_builtin *builtin)
{
	rb_insert(&env->builtins, (void *)builtin->name, (void *)builtin);
}

void
	init_builtin(t_environ *env)
{
	env->builtins = rb_new((int (*)(const void *, const void *))ft_strcmp,
			noop, noop);
	insert_builtin(env, builtin_echo());
}
