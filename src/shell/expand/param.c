/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   param.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgamba <linogamba@pundalik.org>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 11:59:40 by lgamba            #+#    #+#             */
/*   Updated: 2025/03/17 11:59:41 by lgamba           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include <shell/shell.h>

static int
param_special(
	t_shell *shell,
	struct s_arg_param *param,
	char **argv,
	size_t *size)
{
	/*
	const t_stack_frame	*frame = &shell->eval_stack.frames
		[shell->eval_stack.size - 1];
	t_string_buffer		buf;
	size_t				i;

	stringbuf_init(&buf, 64);
	if (!ft_strcmp(name, "?"))
		stringbuf_itoa(&buf, shell->last_status);
	else if (ft_strcmp(name, "#") && shell->eval_stack.size)
		stringbuf_itoa(&buf,
			shell->eval_stack.frames[shell->eval_stack.size - 1].nargs);
	else if (ft_strcmp(name, "@") && shell->eval_stack.size)
	{
		i = 1;
		while (i < frame->nargs)
		{
			if (i > 1)
				stringbuf_append(&buf, (t_string){" ", 1});
			stringbuf_append(&buf, (t_string){
					frame->args[i], ft_strlen(frame->args[i])});
			++i;
		}
	}
	return (stringbuf_cstr(&buf));
	*/
}

int
expand_param(
	t_shell *shell,
	struct s_arg_param *param,
	char **argv,
	size_t *size)
{
	int	status;

	status = param_special(shell, param, argv, size);
	
	return (status);
}
