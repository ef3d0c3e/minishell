/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   node_block.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgamba <linogamba@pundalik.org>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 11:59:40 by lgamba            #+#    #+#             */
/*   Updated: 2025/03/17 11:59:41 by lgamba           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include <shell/shell.h>

t_ast_node
	*make_block_node(t_ast_node *inner)
{
	t_ast_node	*node;

	node = xmalloc(sizeof(t_ast_node));
	node->type = NODE_BLOCK;
	node->block.inner = inner;
	return (node);
}

void
	free_block_node(t_ast_node *node)
{
	ast_free(node->block.inner, 0);
}

void
	print_block_node(size_t depth, const t_ast_node *node)
{
	print_pad(" | ", depth);
	ft_dprintf(2, "BLOCK\n");
	ast_print(depth + 1, node->block.inner);
}
