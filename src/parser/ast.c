/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgamba <linogamba@pundalik.org>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 11:59:40 by lgamba            #+#    #+#             */
/*   Updated: 2025/03/17 11:59:41 by lgamba           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "parser/parser.h"
#include <shell/shell.h>

void
	ast_free(t_ast_node *node, int cleanup)
{
	static void(*cleaners[])(t_ast_node *) = {
	[NODE_BLOCK] = free_block_node,
	[NODE_SUBSHELL] = free_subshell_node,
	[NODE_COMMAND] = free_cmd_node,
	[NODE_LIST] = free_list_node,
	[NODE_LOGIC] = free_logic_node,
	[NODE_FUNCTION] = free_function_node,
	[NODE_IF] = free_if_node,
	[NODE_WHILE] = free_while_node,
	[NODE_FOR] = free_for_node,
	};

	if (!node)
		return ;
	if (!cleanup && node->type == NODE_FUNCTION && node->function.registered)
		return ;
	cleaners[node->type](node);
	free(node);
}

void
	ast_print(
	size_t depth,
	t_ast_node *node)
{
	static void(*printers[])(size_t, const t_ast_node *) = {
	[NODE_BLOCK] = print_block_node,
	[NODE_SUBSHELL] = print_subshell_node,
	[NODE_COMMAND] = print_cmd_node,
	[NODE_LIST] = print_list_node,
	[NODE_LOGIC] = print_logic_node,
	[NODE_FUNCTION] = print_function_node,
	[NODE_IF] = print_if_node,
	[NODE_WHILE] = print_while_node,
	[NODE_FOR] = print_for_node,
	};

	if (node)
		printers[node->type](depth, node);
}
