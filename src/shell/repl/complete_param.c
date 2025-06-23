/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   complete_param.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgamba <linogamba@pundalik.org>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 11:59:40 by lgamba            #+#    #+#             */
/*   Updated: 2025/06/19 06:48:54 by thschnei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include <shell/shell.h>

static void
	var_traverse(size_t depth, t_rbnode *node, void *cookie)
{
	const char				*filter = ((char **)cookie)[0];
	t_complete_buf *const	buf = ((t_complete_buf **)cookie)[1];
	const t_shell_var		*var = node->data;
	char					*fmt;

	(void)depth;
	if (filter && filter[0] == '$')
		++filter;
	if (!ft_strstr(var->name, filter))
		return ;
	ft_asprintf(&fmt, "$%s", var->name);
	complete_buf_push(buf, (t_complete_item){
		.kind = COMPLETE_VARIABLE,
		.name = fmt,
		.desc = ft_strdup("Environment Variable"),
	});
}

void
	repl_complete_params(
	t_shell *shell,
	const t_repl_data *data,
	t_complete_buf *items)
{
	if ((!data->filter || data->filter[0] != '$') && !(data->kind & COMP_PARAM))
		return ;
	rb_apply(&shell->reg_env, var_traverse,
		(const void *[2]){data->filter, items});
}
