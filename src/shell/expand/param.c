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
#include "shell/expand/expand.h"
#include "tokenizer/tokenizer.h"
#include "util/util.h"
#include <shell/shell.h>

static int
	param_special(
	t_shell *shell,
	t_fragment_list *list,
	struct s_arg_item *param)
{
	const t_stack_frame	*frame = &shell->eval_stack.frames
		[shell->eval_stack.size - 1];
	t_string_buffer		buf;

	// FIXME -- Incorrect argv handling
	if (!ft_strcmp(param->param.name, "?"))
	{
		stringbuf_init(&buf, 64);
		stringbuf_itoa(&buf, shell->last_status);
		fraglist_push(list, buf, param->flags);
		return (1);
	}
	else if (!ft_strcmp(param->param.name, "#") && shell->eval_stack.size)
	{
		stringbuf_init(&buf, 64);
		stringbuf_itoa(&buf, frame->nargs);
		fraglist_push(list, buf, param->flags);
		return (1);
	}
	/*
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
	*/
	return (0);
}

static int
	param_positional(
	t_shell *shell,
	t_fragment_list *list,
	struct s_arg_item *param)
{
	const t_stack_frame	*frame = &shell->eval_stack.frames
		[shell->eval_stack.size - 1];
	int		num;

	if (shell->eval_stack.size == 0)
		return (0);
	if (!atoi_checked(param->param.name, &num) || num < 0)
		return (0);
	if ((size_t)num >= frame->nargs)
		return (0);
	fraglist_push(list, stringbuf_from(frame->args[num]), param->flags);
	return (1);
}

static int
	param_local(
	t_shell *shell,
	t_fragment_list *list,
	struct s_arg_item *param)
{
	t_stack_frame *const	frame = &shell->eval_stack.frames
		[shell->eval_stack.size - 1];
	const char				*found;

	if (shell->eval_stack.size == 0)
		return (0);
	found = rb_find(&frame->locals, param->param.name);
	if (!found)
		return (0);
	fraglist_push(list, stringbuf_from(found), param->flags);
	return (1);
}

static int
	param_env(
	t_shell *shell,
	t_fragment_list *list,
	struct s_arg_item *param)
{
	const char				*found;

	found = rb_find(&shell->reg_env, param->param.name);
	if (!found)
		return (0);
	fraglist_push(list, stringbuf_from(found), param->flags);
	return (1);
}

// FIXME: This doesn't handle word splitting...
int
expand_param(
	t_shell *shell,
	t_fragment_list *list,
	struct s_arg_item *param)
{
	int				status;

	status = param_special(shell, list, param);
	if (!status)
		status = param_positional(shell, list, param);
	if (!status)
		status = param_local(shell, list, param);
	if (!status)
		status = param_env(shell, list, param);
	
	// TODO: Perform expansion and custom processing according to the parameter's rules and status
	return (status);
}
