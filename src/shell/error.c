/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgamba <linogamba@pundalik.org>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 11:59:40 by lgamba            #+#    #+#             */
/*   Updated: 2025/03/17 11:59:41 by lgamba           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "eval.h"
#include <stdio.h>

void
	shell_error(t_environ *env, char *msg, const char *function)
{
	size_t	new_capacity;

	new_capacity = env->errors_capacity + !env->errors_capacity * 8;
	while (env->errors_size + 1 >= new_capacity)
		new_capacity *= 2;
	env->errors = ft_realloc(env->errors, env->errors_size * sizeof(t_error),
			new_capacity * sizeof(t_error));
	env->errors[env->errors_size++] = (t_error){
		.msg = msg,
		.function = function,
	};
}

int
	shell_error_flush(t_environ *env)
{
	int		has_error;
	size_t	i;

	has_error = 0;
	i = 0;
	while (i < env->errors_size)
	{
		has_error = 1;
		ft_dprintf(2, "Error(%s): %s\n", env->errors[i].function, env->errors[i].msg);
		free(env->errors[i].msg);
		++i;
	}
	env->errors_size = 0;
	return (has_error == 0);
}
