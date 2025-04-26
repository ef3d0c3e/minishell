/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   nodes_stmt.c                                       :+:      :+:    :+:   */
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
	*make_funcdef_node(t_string_buffer name, t_ast_node *body)
{
	t_ast_node	*node;

	node = xmalloc(sizeof(t_ast_node));
	node->type = NODE_FUNCTION;
	node->function.name = name;
	node->function.body = body;
	return (node);
}

t_ast_node
	*make_if_node(void)
{
	t_ast_node	*node;

	node = xmalloc(sizeof(t_ast_node));
	node->type = NODE_IF;
	node->st_if.conds = NULL;
	node->st_if.nconds = 0;
	node->st_if.bodies = NULL;
	node->st_if.nbodies = 0;
	return (node);
}
