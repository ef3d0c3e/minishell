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
#include <shell/shell.h>

void
	shell_error(t_shell *shell, char *msg, const char *function)
{
	size_t	new_capacity;

	new_capacity = shell->errors.capacity + !shell->errors.capacity * 8;
	while (shell->errors.size + 1 >= new_capacity)
		new_capacity *= 2;
	shell->errors.errors = ft_realloc(shell->errors.errors,
		shell->errors.size * sizeof(t_error), new_capacity * sizeof(t_error));
	shell->errors.errors[shell->errors.size++] = (t_error){
		.msg = msg,
		.function = function,
	};
}

int
	shell_error_flush(t_shell *shell)
{
	t_error	*errors = shell->errors.errors;
	int		has_error;
	size_t	i;

	has_error = 0;
	i = 0;
	while (i < shell->errors.size)
	{
		has_error = 1;
		ft_dprintf(2, "Error(%s): %s\n", errors[i].function, errors[i].msg);
		free(errors[i].msg);
		++i;
	}
	shell->errors.size = 0;
	return (!has_error);
}

void
	shell_fail(t_shell *shell, const char *msg, const char *func)
{
	shell_error(shell, ft_strdup(msg), func);
	shell_exit(shell, EXIT_FAILURE);
}

void
	shell_perror(t_shell *shell, const char *msg, const char *func)
{
	char	*err;

	ft_asprintf(&err, "%s: %m", msg);
	shell_error(shell, err, func);
	shell_exit(shell, EXIT_FAILURE);
}
