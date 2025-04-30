/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir_util.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgamba <linogamba@pundalik.org>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 11:59:40 by lgamba            #+#    #+#             */
/*   Updated: 2025/03/17 11:59:41 by lgamba           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "parser/args.h"
#include <shell/shell.h>

const t_redir_tok_type
	*redir_alternatives(
	const t_redir_tok_type *types,
	size_t len,
	const char *token)
{
	size_t	i;

	i = 0;
	while (i < len)
	{
		if (!ft_strcmp(types[i].tok, token))
			return (&types[i]);
		++i;
	}
	return (NULL);
}

int
	redir_dest_word(const t_redirection *redir)
{
	if (redir->type == R_CLOSE_THIS || redir->type == R_DUPLICATING_INPUT
		|| redir->type == R_DUPLICATING_OUTPUT || redir->type == R_MOVE_INPUT
		|| redir->type == R_MOVE_OUTPUT)
		return (0);
	return (1);
}

static const char
	*redir_name(enum e_redir_type type)
{
	static const char	*names[] = {
	[R_OUTPUT_DIRECTION] = "OUTPUT_DIRECTION",
	[R_INPUT_DIRECTION] = "INPUT_DIRECTION",
	[R_APPENDING_TO] = "APPENDING_TO",
	[R_READING_UNTIL] = "READING_UNTIL",
	[R_READING_STRING] = "READING_STRING",
	[R_DUPLICATING_INPUT] = "DUPLICATING_INPUT",
	[R_DUPLICATING_OUTPUT] = "DUPLICATING_OUTPUT",
	[R_DEBLANK_READING_UNTIL] = "DEBLANK_READING_UNTIL",
	[R_CLOSE_THIS] = "CLOSE_THIS",
	[R_ERR_AND_OUT] = "ERR_AND_OUT",
	[R_INPUT_OUTPUT] = "INPUT_OUTPUT",
	[R_OUTPUT_FORCE] = "OUTPUT_FORCE",
	[R_DUPLICATING_INPUT_WORD] = "DUPLICATING_INPUT_WORD",
	[R_DUPLICATING_OUTPUT_WORD] = "DUPLICATING_OUTPUT_WORD",
	[R_MOVE_INPUT] = "MOVE_INPUT",
	[R_MOVE_OUTPUT] = "MOVE_OUTPUT",
	[R_MOVE_INPUT_WORD] = "MOVE_INPUT_WORD",
	[R_MOVE_OUTPUT_WORD] = "MOVE_OUTPUT_WORD",
	[R_APPEND_ERR_AND_OUT] = "APPEND_ERR_AND_OUT",
	};

	return (names[type]);
}

void
	print_redir(
	const t_redirections *redirs,
	size_t depth)
{
	size_t				i;
	const t_redirection	*redir;

	if (!redirs->redirs_size)
		return ;
	for (size_t i = 1; i < depth; ++i)
		write(2, " | ", 3);
	ft_dprintf(2, "REDIRS:\n");
	i = 0;
	while (i < redirs->redirs_size)
	{
		for (size_t i = 0; i < depth; ++i)
			write(2, " | ", 3);
		redir = &redirs->redirs[i];
		if (redir_dest_word(redir))
		{
			ft_dprintf(2, "%s fl=%05o %d:", redir_name(redir->type), redir->flags, redir->redirector.fd);
			arg_print(0, &redir->redirectee.filename);
		}
		else
		{
			ft_dprintf(2, "%s fl=%05o %d:%d\n", redir_name(redir->type), redir->flags, redir->redirector.fd, redir->redirectee.fd);
		}
		++i;
	}
}
