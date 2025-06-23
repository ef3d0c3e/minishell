/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   complete_cmd.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgamba <linogamba@pundalik.org>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 11:59:40 by lgamba            #+#    #+#             */
/*   Updated: 2025/06/19 06:48:54 by thschnei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include <shell/shell.h>

static void
	path_traversal(size_t depth, t_rbnode *node, void *cookie)
{
	t_comp_cmd_tr *const	tr = cookie;

	(void)depth;
	if (tr->filter && !complete_match(tr->filter, node->key))
		return ;
	complete_buf_push(tr->items, (t_complete_item){
		.kind = COMPLETE_WORD,
		.name = ft_strdup(node->key),
		.desc = ft_strdup("Executable"),
	});
}

static void
	fn_traversal(size_t depth, t_rbnode *node, void *cookie)
{
	t_comp_cmd_tr *const	tr = cookie;

	(void)depth;
	if (tr->filter && !complete_match(tr->filter, node->key))
		return ;
	complete_buf_push(tr->items, (t_complete_item){
		.kind = COMPLETE_WORD,
		.name = ft_strdup(node->key),
		.desc = ft_strdup("Function"),
	});
}

static void
	builtin_traversal(size_t depth, t_rbnode *node, void *cookie)
{
	t_comp_cmd_tr *const	tr = cookie;

	(void)depth;
	if (tr->filter && !complete_match(tr->filter, node->key))
		return ;
	complete_buf_push(tr->items, (t_complete_item){
		.kind = COMPLETE_WORD,
		.name = ft_strdup(node->key),
		.desc = ft_strdup("Builtin"),
	});
}

void
	repl_complete_cmd(
	t_shell *shell,
	const t_repl_data *data,
	t_complete_buf *items)
{
	t_comp_cmd_tr	tr;

	if (!(data->kind & COMP_CMD))
		return ;
	tr.filter = data->filter;
	tr.items = items;
	rb_apply(&shell->path_cache, path_traversal, &tr);
	rb_apply(&shell->reg_fns, fn_traversal, &tr);
	rb_apply(&shell->reg_builtins, builtin_traversal, &tr);
}
