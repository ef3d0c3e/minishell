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
#include "util/util.h"
#include <shell/shell.h>
#include <stdio.h>

static int
	param_special(
	t_shell *shell,
	struct s_arg_param *param,
	char ***argv,
	size_t *size)
{
	const t_stack_frame	*frame = &shell->eval_stack.frames
		[shell->eval_stack.size - 1];
	t_string_buffer		buf;
	size_t				i;

	// FIXME -- Incorrect argv handling
	stringbuf_init(&buf, 64);
	if (!ft_strcmp(param->name, "?"))
	{
		stringbuf_itoa(&buf, shell->last_status);
		argv_push(argv, (*size)++, stringbuf_cstr(&buf));
		return (1);
	}
	else if (!ft_strcmp(param->name, "#") && shell->eval_stack.size)
	{
		stringbuf_itoa(&buf,
				shell->eval_stack.frames[shell->eval_stack.size - 1].nargs);
		argv_push(argv, (*size)++, stringbuf_cstr(&buf));
		return (1);
	}
	else if (!ft_strcmp(param->name, "@") && shell->eval_stack.size)
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
		argv_push(argv, (*size)++, stringbuf_cstr(&buf));
		return (1);
	}
	if (!ft_strcmp(param->name, "*") && shell->eval_stack.size)
	{
		i = 1;
		while (i < frame->nargs)
		{
			stringbuf_append(&buf, (t_string){
					frame->args[i], ft_strlen(frame->args[i])});
			argv_push(argv, (*size)++, stringbuf_cstr(&buf));
			stringbuf_init(&buf, 64);
			++i;
		}
		return (1);
	}
	stringbuf_free(&buf);
	return (0);
}

static int
	param_positional(
	t_shell *shell,
	struct s_arg_param *param,
	char ***argv,
	size_t *size)
{
	const t_stack_frame	*frame = &shell->eval_stack.frames
		[shell->eval_stack.size - 1];
	int		num;
	size_t	i;

	if (shell->eval_stack.size == 0)
		return (0);
	i = 0;
	while (param->name[i] >= '0' && param->name[i] <= '9')
		++i;
	if (param->name[i])
		return (0);
	if (!atoi_checked(param->name, &num))
		return (0);
	if ((size_t)num >= frame->nargs)
		return (0);
	argv_append((*argv) + *size, frame->args[num]);
	return (1);
}

static int
	param_local(
	t_shell *shell,
	struct s_arg_param *param,
	char ***argv,
	size_t *size)
{
	t_stack_frame *const	frame = &shell->eval_stack.frames
		[shell->eval_stack.size - 1];
	const char				*found;

	if (shell->eval_stack.size == 0)
		return (0);
	found = rb_find(&frame->locals, param->name);
	if (!found)
		return (0);
	argv_append((*argv) + *size, found);
	return (1);
}

static int
	param_env(
	t_shell *shell,
	struct s_arg_param *param,
	char ***argv,
	size_t *size)
{
	const char				*found;

	found = rb_find(&shell->reg_env, param->name);
	if (!found)
		return (0);
	argv_append((*argv) + *size, found);
	return (1);
}

int
expand_param(
	t_shell *shell,
	struct s_arg_param *param,
	char ***argv,
	size_t *size)
{
	const size_t	start_size = *size;
	int				status;

	status = param_special(shell, param, argv, size);
	if (!status)
		status = param_positional(shell, param, argv, size);
	if (!status)
		status = param_local(shell, param, argv, size);
	if (!status)
		status = param_env(shell, param, argv, size);
	
	// TODO: Perform expansion and custom processing according to the parameter's rules and status
	return (status);
}
