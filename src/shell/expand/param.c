/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   param.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgamba <linogamba@pundalik.org>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 11:59:40 by lgamba            #+#    #+#             */
/*   Updated: 2025/06/19 07:36:08 by thschnei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include <shell/shell.h>

static int
	param_special_glob(
	t_fragment_list *list,
	struct s_atom *param,
	const t_stack_frame *frame,
	const char *ifs)
{
	t_string_buffer	buf;
	size_t			i;

	i = 1;
	if (!ft_strcmp(param->param.name, "*"))
	{
		stringbuf_init(&buf, 64);
		while (i++ < frame->nargs)
		{
			if (i - 1 > 1)
				stringbuf_append(&buf, (t_string){ifs, 1});
			stringbuf_append(&buf, (t_string){frame->args[i - 1],
				ft_strlen(frame->args[i - 1])});
		}
		return (fraglist_push(list, buf, param->flags), 1);
	}
	else if (ft_strcmp(param->param.name, "@"))
		return (0);
	while (i < frame->nargs)
	{
		fraglist_push(list, stringbuf_from_range(frame->args[i],
				frame->args[i] + ft_strlen(frame->args[i])), param->flags);
		list->fragments[list->size - 1].force_split |= (i++ > 1);
	}
	return (1);
}

static int
	param_special(
	t_shell *shell,
	t_fragment_list *list,
	struct s_atom *param,
	const char *ifs)
{
	const t_stack_frame	*frame = &shell->eval_stack.frames
	[shell->eval_stack.size - 1];
	t_string_buffer		buf;

	if (!ft_strcmp(param->param.name, "?"))
	{
		stringbuf_init(&buf, 64);
		stringbuf_append_i(&buf, shell->last_status);
		fraglist_push(list, buf, param->flags);
		return (1);
	}
	else if (!ft_strcmp(param->param.name, "#") && shell->eval_stack.size)
	{
		stringbuf_init(&buf, 64);
		stringbuf_append_i(&buf, frame->nargs);
		fraglist_push(list, buf, param->flags);
		return (1);
	}
	else if (shell->eval_stack.size)
		return (param_special_glob(list, param, frame, ifs));
	return (0);
}

static int
	param_positional(
	t_shell *shell,
	t_fragment_list *list,
	struct s_atom *param)
{
	const t_stack_frame	*frame = &shell->eval_stack.frames
	[shell->eval_stack.size - 1];
	int					num;

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
	param_local_env(
	t_shell *shell,
	t_fragment_list *list,
	struct s_atom *param)
{
	char	*found;

	found = get_variable_value(shell, param->param.name);
	if (!found)
		return (0);
	fraglist_push(list, stringbuf_from(found), param->flags);
	return (1);
}

int
	expand_param(
	t_shell *shell,
	t_fragment_list *list,
	struct s_atom *param,
	const char *ifs)
{
	int		status;
	char	*err;

	status = param_special(shell, list, param, ifs);
	if (!status)
		status = param_positional(shell, list, param);
	if (!status)
		status = param_local_env(shell, list, param);
	if (!status && option_value(shell, "experr"))
	{
		ft_asprintf(&err, "Error: %s: Unbound variable", param->param.name);
		shell_error(shell, err, SRC_LOCATION);
		status = -1;
	}
	return (status);
}
