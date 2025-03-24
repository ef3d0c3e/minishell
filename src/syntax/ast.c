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
#include "syntax/tokenizer.h"
#include <stdio.h>


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
	else if (head->type == NODE_ATOM)
	{
		stringbuf_free(&head->atom);
		return ;
	}
	else if (head->type == NODE_SUBEXPR)
	{
		ast_free(head->expr.head);
		return ;
	}
	else if (head->type == NODE_COMMAND)
	{
		for (size_t i = 0; i < head->cmd.nargs; ++i)
			ast_free(&head->cmd.args[i]);
		free(head->cmd.args);
		for (size_t i = 0; i < head->cmd.redirs_size; ++i)
			stringbuf_free(&head->cmd.redirs[i].word);
		free(head->cmd.redirs);
	}
	free(head);
}

void
	ast_print_debug(
		t_string input,
		t_ast_node *head,
		size_t depth)
{
	if (!head)
		return ;
	for (size_t i = 0; i < depth; ++i)
		write(2, " | ", 3);
	if (head->type == NODE_ATOM)
		dprintf(2, "ATOM(`%.*s`)\n", (int)head->atom.len, head->atom.str);
	else if (head->type == NODE_SUBEXPR)
	{
		dprintf(2, "SUBEXPR\n");
		ast_print_debug(head->expr.input, head->expr.head, depth + 1);
	}
	else if (head->type == NODE_COMMAND)
	{
		dprintf(2, "COMMAND [%zu]\n", head->cmd.nargs);
		for (size_t i = 0; i < head->cmd.nargs; ++i)
			ast_print_debug(input, &head->cmd.args[i], depth + 1);
		if (head->cmd.redirs)
		{
			dprintf(2, " + REDIRS: ");
			for (size_t i = 0; i < head->cmd.redirs_size; ++i)
			{
				dprintf(2, "%d:'%.*s' ",  head->cmd.redirs[i].fd, (int)head->cmd.redirs[i].word.len, head->cmd.redirs[i].word.str);
			}
		}
	}
	else if (head->type == NODE_LOGIC)
	{
		dprintf(2, "LOGIC `%s`\n", head->logic.token.reserved_word);
		ast_print_debug(input, head->logic.left, depth + 1);
		ast_print_debug(input, head->logic.right, depth + 1);
	}
}
