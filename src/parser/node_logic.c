/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   node_logic.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgamba <linogamba@pundalik.org>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 11:59:40 by lgamba            #+#    #+#             */
/*   Updated: 2025/03/17 11:59:41 by lgamba           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include <shell/shell.h>

t_ast_node
	*make_logic_node(t_token op, t_ast_node *left, t_ast_node *right)
{
	t_ast_node	*node;

	node = xmalloc(sizeof(t_ast_node));
	node->type = NODE_BINARY;
	if (!ft_strcmp(op.reserved_word, "|") || !ft_strcmp(op.reserved_word, "|&"))
		node->type = NODE_PIPE;
	node->logic.token = op;
	node->logic.left = left;
	node->logic.right = right;
	return (node);
}

void
	free_logic_node(t_ast_node *node)
{
	ast_free(node->logic.left, 0);
	ast_free(node->logic.right, 0);
}

void
	print_logic_node(size_t depth, const t_ast_node *node)
{
	print_pad(" | ", depth);
	ft_dprintf(2, "LOGIC '%s'\n", node->logic.token.reserved_word);
	ast_print(depth + 1, node->logic.left);
	ast_print(depth + 1, node->logic.right);
}
