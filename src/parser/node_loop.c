/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   node_loop.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgamba <linogamba@pundalik.org>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 11:59:40 by lgamba            #+#    #+#             */
/*   Updated: 2025/03/17 11:59:41 by lgamba           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include <shell/shell.h>

t_ast_node
	*make_loop_node(t_ast_node *cond, t_ast_node *body, int until)
{
	t_ast_node	*node;

	node = xmalloc(sizeof(t_ast_node));
	node->type = NODE_LOOP;
	node->st_loop.until = until;
	node->st_loop.cond = cond;
	node->st_loop.body = body;
	return (node);
}

void
	free_loop_node(t_ast_node *node)
{
	ast_free(node->st_loop.cond, 0);
	ast_free(node->st_loop.body, 0);
}

void
	print_loop_node(size_t depth, const t_ast_node *node)
{
	print_pad(" | ", depth);
	ft_dprintf(2, "%s\n", &"WHILE\0UNTIL"[node->st_loop.until * 6]);
	print_pad(" | ", depth);
	ft_dprintf(2, "(COND)\n");
	ast_print(depth + 1, node->st_loop.cond);
	print_pad(" | ", depth);
	ft_dprintf(2, "(BODY)\n");
	ast_print(depth + 1, node->st_loop.body);
}
