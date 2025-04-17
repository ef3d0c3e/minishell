/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   eval_util.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgamba <linogamba@pundalik.org>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 11:59:40 by lgamba            #+#    #+#             */
/*   Updated: 2025/03/17 11:59:41 by lgamba           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "ft_printf.h"
#include <shell/eval.h>
#include <sys/types.h>

pid_t
	shell_fork(t_environ *env, const char *function)
{
	pid_t	pid;
	char	*err;

	pid = fork();
	if (env->is_child && pid == -1)
		shell_perror(env, EXIT_FAILURE, "fork() failed", function);
	else if (pid == -1)
	{
		ft_asprintf(&err, "fork() failed: %m");
		shell_error(env, err, function);
	}
	if (!pid)
		env->is_child = 1;
	return (pid);
}
