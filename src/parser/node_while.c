/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   node_while.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgamba <linogamba@pundalik.org>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 11:59:40 by lgamba            #+#    #+#             */
/*   Updated: 2025/03/17 11:59:41 by lgamba           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include <shell/shell.h>

t_ast_node
	*make_while_node(t_ast_node *cond, t_ast_node *body)
{
	t_ast_node	*node;

	node = xmalloc(sizeof(t_ast_node));
	node->type = NODE_WHILE;
	node->st_while.cond = cond;
	node->st_while.body = body;
	return (node);
}

void
	free_while_node(t_ast_node *node)
{
	ast_free(node->st_while.cond);
	ast_free(node->st_while.body);
}

void
	print_while_node(size_t depth, const t_ast_node *node)
{
	print_pad(" | ", depth);
	ft_dprintf(2, "WHILE\n");
	print_pad(" | ", depth);
	ft_dprintf(2, "(COND)\n");
	ast_print(depth + 1, node->st_while.cond);
	print_pad(" | ", depth);
	ft_dprintf(2, "(BODY)\n");
	ast_print(depth + 1, node->st_while.body);
}
