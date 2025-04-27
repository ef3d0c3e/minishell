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
#include "ft_printf.h"
#include "parser/parser.h"
#include "util/util.h"
#include <shell/shell.h>

t_ast_node
	*make_list_node(void)
{
	t_ast_node	*node;

	node = xmalloc(sizeof(t_ast_node));
	node->type = NODE_LIST;
	node->list.cmds = NULL;
	node->list.seps = NULL;
	node->list.ncmds = 0;
	return (node);
}

void
	free_list_node(t_ast_node *node)
{
	size_t	i;

	i = 0;
	while (i < node->list.ncmds)
		ast_free(node->list.cmds[i++]);
	free(node->list.seps);
	free(node->list.cmds);
}

void
	print_list_node(size_t depth, const t_ast_node *node)
{
	size_t	i;

	print_pad(" | ", depth);
	ft_dprintf(2, "LIST\n");
	i = 0;
	while (i < node->list.ncmds)
	{
		ast_print(depth + 1, node->list.cmds[i++]);
	}
}

void
	list_node_push(t_ast_node *list, t_ast_node *cmd, char sep)
{
	list->list.cmds = ft_realloc(list->list.cmds,
			sizeof(t_ast_node **) * list->list.ncmds,
			sizeof(t_ast_node **) * (list->list.ncmds + 1));
	list->list.seps = ft_realloc(list->list.seps,
			sizeof(t_token) * list->list.ncmds,
			sizeof(t_token) * (list->list.ncmds + 1));
	list->list.seps[list->list.ncmds] = sep;
	list->list.cmds[list->list.ncmds++] = cmd;
}
