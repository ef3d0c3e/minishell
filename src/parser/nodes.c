/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   nodes.c                                            :+:      :+:    :+:   */
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
	*make_logic_node(t_token op, t_ast_node *left, t_ast_node *right)
{
	t_ast_node	*node;

	node = xmalloc(sizeof(t_ast_node));
	node->type = NODE_LOGIC;
	node->logic.token = op;
	node->logic.left = left;
	node->logic.right = right;
	return (node);
}

t_ast_node
	*make_cmd_node()
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

t_ast_node
	*make_subshell_node(t_ast_node *inner)
{
	t_ast_node	*node;

	node = xmalloc(sizeof(t_ast_node));
	node->type = NODE_SUBSHELL;
	node->expr.head = inner;
	node->expr.redirs.redirs = NULL;
	node->expr.redirs.redirs_capacity = 0;
	node->expr.redirs.redirs_size = 0;
	return (node);
}
