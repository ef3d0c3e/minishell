/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   node_list.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgamba <linogamba@pundalik.org>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 11:59:40 by lgamba            #+#    #+#             */
/*   Updated: 2025/03/17 11:59:41 by lgamba           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include <shell/shell.h>

t_ast_node
	*make_list_node(void)
{
	t_ast_node	*node;

	node = xmalloc(sizeof(t_ast_node));
	node->type = NODE_LIST;
	node->list.cmds = NULL;
	node->list.ncmds = 0;
	return (node);
}

void
	free_cmdlist_node(t_ast_node *node)
{
	size_t	i;

	i = 0;
	while (i < node->list.ncmds)
		ast_free(node->list.cmds[i++]);
	free(node->list.cmds);
}

void
	print_cmdlist_node(size_t depth, const t_ast_node *node)
{
	size_t	i;

	print_pad(" | ", depth);
	ft_dprintf(2, "LIST\n");
	i = 0;
	while (i < node->list.ncmds)
		ast_print(depth + 1, node->list.cmds[i++]);
}
