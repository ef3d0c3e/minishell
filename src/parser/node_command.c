/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   node_command.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgamba <linogamba@pundalik.org>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 11:59:40 by lgamba            #+#    #+#             */
/*   Updated: 2025/03/17 11:59:41 by lgamba           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "parser/parser.h"
#include <shell/shell.h>

t_ast_node
	*make_cmd_node(void)
{
	t_ast_node	*node;

	node = xmalloc(sizeof(t_ast_node));
	node->type = NODE_COMMAND;
	node->cmd.args = NULL;
	node->cmd.nargs = 0;
	node->cmd.assigns = NULL;
	node->cmd.nassigns = 0;
	node->cmd.redirs.redirs = NULL;
	node->cmd.redirs.redirs_capacity = 0;
	node->cmd.redirs.redirs_size = 0;
	return (node);
}

void
	free_cmd_node(t_ast_node *node)
{
	arglist_free(node->cmd.args, node->cmd.nargs);
	redirs_free(&node->cmd.redirs);
}

void
	print_cmd_node(size_t depth, const t_ast_node *node)
{
	print_pad(" | ", depth);
	ft_dprintf(2, "COMMAND\n");
	arglist_print(depth + 1, node->cmd.args, node->cmd.nargs);
	print_redir(&node->cmd.redirs, depth + 1);
}
