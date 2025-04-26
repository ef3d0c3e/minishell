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
	node->sub.head = inner;
	node->sub.redirs.redirs = NULL;
	node->sub.redirs.redirs_capacity = 0;
	node->sub.redirs.redirs_size = 0;
	return (node);
}

void
	free_subshell_node(t_ast_node *node)
{
	ast_free(node->sub.head);
	redirs_free(&node->sub.redirs);
}

void
	print_subshell_node(size_t depth, const t_ast_node *node)
{
	print_pad(" | ", depth);
	ft_dprintf(2, "SUBSHELL\n");
	ast_print(depth + 1, node->sub.head);
	print_redir(&node->sub.redirs, depth + 1);
}
