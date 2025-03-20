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
#include "parser.h"


void
	ast_free(t_ast_node *head)
{
	if (!head)
		return ;
	if (head->type == NODE_LOGIC)
	{
		ast_free(head->logic.left);
		ast_free(head->logic.right);
	}
	else if (head->type == NODE_UNARY)
	{
		// TODO
	}
	else if (head->type == NODE_COMMAND)
	{
		for (size_t i = 0; i < head->cmd.nargs; ++i)
			stringbuf_free(&head->cmd.args[i]);
		free(head->cmd.args);
	}
	free(head);
}

void
	ast_print_debug(t_string input, t_ast_node *head, size_t depth)
{
	for (size_t i = 0; i < depth; ++i)
		write(2, "\t", 1);
	if (head->type == NODE_COMMAND)
	{
		dprintf(2, "COMMAND [%zu]: ", head->cmd.nargs);
		for (size_t i = 0; i < head->cmd.nargs; ++i)
			dprintf(2, "`%.*s` ", head->cmd.args[i].len, head->cmd.args[i].str);
		dprintf(2, "\n");
	}
	else if (head->type == NODE_LOGIC)
	{
		dprintf(2, "LOGIC `%s`\n", head->logic.token.reserved_word);
		ast_print_debug(input, head->logic.left, depth + 1);
		ast_print_debug(input, head->logic.right, depth + 1);
	}
}
