/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   node_function.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgamba <linogamba@pundalik.org>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 11:59:40 by lgamba            #+#    #+#             */
/*   Updated: 2025/03/17 11:59:41 by lgamba           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include <shell/shell.h>

t_ast_node
	*make_function_node(t_string_buffer name, t_ast_node *body)
{
	t_ast_node	*node;

	node = xmalloc(sizeof(t_ast_node));
	node->type = NODE_FUNCTION;
	node->function.name = name;
	node->function.body = body;
	node->function.registered = 0;
	return (node);
}

void
	free_function_node(t_ast_node *node)
{
	stringbuf_free(&node->function.name);
	ast_free(node->function.body);
}

void
	print_function_node(size_t depth, const t_ast_node *node)
{
	print_pad(" | ", depth);
	ft_dprintf(2, "FUNCTION '%.*s'\n", (int)node->function.name.len,
		node->function.name.str);
	ast_print(depth + 1, node->function.body);
}
