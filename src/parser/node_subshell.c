/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   node_subshell.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgamba <linogamba@pundalik.org>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 11:59:40 by lgamba            #+#    #+#             */
/*   Updated: 2025/03/17 11:59:41 by lgamba           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include <shell/shell.h>

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

void
	free_subshell_node(t_ast_node *node)
{
	ast_free(node->expr.head);
	redirs_free(&node->expr.redirs);
}

void
	print_subshell_node(size_t depth, const t_ast_node *node)
{
	print_pad(" | ", depth);
	ft_dprintf(2, "SUBSHELL\n");
	ast_print(depth + 1, node->expr.head);
	print_redir(&node->expr.redirs, depth + 1);
}
