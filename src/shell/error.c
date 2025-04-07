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
		.msg = stringbuf_from_owned(msg),
		.function = function,
	};
}
